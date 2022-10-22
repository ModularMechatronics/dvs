from asyncore import write
import os
import subprocess

FOLDERS = [{"name": "src/main_application", "strip_prefix": "src"},
           {"name": "src/interfaces/cpp", "strip_prefix": "cpp"},
           {"name": "src/interfaces/c", "strip_prefix": "interfaces/c"}]

EXCLUDE_FOLDERS = ["src/externals", "src/main_application/old"]

def get_repo_root():
    res = subprocess.run(["git", "rev-parse", "--show-toplevel"], capture_output=True)
    return res.stdout.decode("utf-8").rstrip("\n")

def get_file_lines(file_path):

    with open(file_path, "r") as f:
        read_lines = f.readlines()

    return read_lines

def write_to_file(file_path, file_lines):

    with open(file_path, "w") as f:
        f.writelines(file_lines)

def modify_file(file_path, strip_prefix):
    file_lines = get_file_lines(file_path)

    header_guard_string = file_path[file_path.find(strip_prefix) + len(strip_prefix) + 1:].replace("/", "_").replace(".", "_").upper() + "_"

    ifndef_idx = -1
    define_idx = -1
    endif_idx = -1

    for idx, fl in enumerate(file_lines):
        if (("#ifndef" in fl) or ("#IFNDEF" in fl)) and (ifndef_idx == -1):
            ifndef_idx = idx
        if (("#define" in fl) or ("#DEFINE" in fl)) and (define_idx == -1):
            define_idx = idx
        if ("#endif" in fl) or ("#ENDIF" in fl):
            endif_idx = idx

    if (ifndef_idx == -1) or (define_idx == -1) or (endif_idx == -1):
        raise Exception("Failed to find indices!")

    if ifndef_idx != (define_idx - 1):
        raise Exception("ifndef_idx is not define_idx - 1! #define is expected right after #ifndef!")

    ifndef_line = "#ifndef " + header_guard_string + "\n"
    define_line = "#define " + header_guard_string + "\n"
    endif_line = "#endif  // " + header_guard_string + "\n"

    file_lines[ifndef_idx] = ifndef_line
    file_lines[define_idx] = define_line
    file_lines[endif_idx] = endif_line

    write_to_file(file_path, file_lines)

if __name__ == "__main__":

    if get_repo_root() != os.getcwd():
        raise Exception("You should be in repo root to run this script!")

    repo_root = get_repo_root()
    for folder in FOLDERS:
        os.chdir(repo_root)
        os.chdir(folder["name"])
        current_path = os.getcwd()

        header_files = [os.path.join(dp, f) for dp, dn, filenames in os.walk(current_path) for f in filenames if os.path.splitext(f)[1] == '.h']

        for f in header_files:
            is_valid_file = not any([p in f for p in EXCLUDE_FOLDERS])

            if is_valid_file:
                modify_file(f, folder["strip_prefix"])
