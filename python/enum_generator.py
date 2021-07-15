import os
import subprocess

ENUM_FILE_PATH = "./interfaces/cpp/dvs/enumerations.h"
PYTHON_OUTPUT_PATH = "./python/generated_enums.py"
TRANSMISSION_DATA_TYPE_STRING = "_TRANSMISSION_DATA_TYPE"


def read_file_lines(file_path):
    with open(file_path, "r") as f:
        file_lines = f.readlines()

    return file_lines


class ParsedEnum:
    def __init__(self, enum_lines):

        self._has_enum_type = ":" in enum_lines[0]
        if self._has_enum_type:
            self._enum_type = enum_lines[0].split(
                ":")[-1].lstrip(" ").rstrip(" ").rstrip("_t")
        else:
            self._enum_type = None
        self._parse_enum_name(enum_lines[0])
        self._parse_enum_members(enum_lines)

    def _parse_enum_members(self, enum_lines):

        idx = 0

        while "{" not in enum_lines[idx]:
            idx += 1
        first_member_line_number = idx + 1

        while "};" not in enum_lines[idx]:
            idx += 1
        last_member_line_number = idx - 1

        self._members = enum_lines[first_member_line_number:(
            last_member_line_number + 1)]
        self._members = [m.lstrip(" ").rstrip(" ").rstrip(
            ",").rstrip(" ") for m in self._members]
        self._members = [m for m in self._members if m]

    def _parse_enum_name(self, first_line):
        first_line.rstrip(" ").lstrip(" ")
        if self._has_enum_type:
            elems = [e.rstrip(" ").lstrip(" ")
                     for e in first_line.split(":")[0].split(" ")]
        else:
            elems = [e.rstrip(" ").lstrip(" ") for e in first_line.split(" ")]

        if elems[0] == "enum" and elems[1] == "class":
            self._enum_name = elems[2]
        elif elems[0] == "enum":
            self._enum_name = elems[1]
        else:
            raise Exception("Something went wrong!")

    def enum_name(self):
        return self._enum_name

    def members(self):
        return self._members

    def enum_type(self):
        return self._enum_type

    def has_enum_type(self):
        return self._has_enum_type


def parse_to_python_enum(parsed_enum):
    lines = []
    lines.append("class " + parsed_enum.enum_name() + "(Enum):")

    idx = 0
    for m in parsed_enum.members():
        lines.append("    " + m + " = " + str(idx))
        idx += 1
    if parsed_enum.has_enum_type():
        lines.append("    " + TRANSMISSION_DATA_TYPE_STRING +
                     " = np." + parsed_enum.enum_type())

    return lines


def find_enum_lines(lines):
    enum_lines = []
    num_lines = len(lines)
    idx = 0

    while idx < num_lines:

        if "enum " in lines[idx]:
            new_enum_lines = []
            while "};" not in lines[idx]:
                new_enum_lines.append(lines[idx])
                idx += 1
            new_enum_lines.append(lines[idx])
            idx += 1
            enum_lines.append(new_enum_lines)
        else:
            idx += 1
    return enum_lines


def to_python_file(lines, output_path):

    all_lines = ["from enum import Enum\n",
                 "import numpy as np\n",
                 "\n",
                 "\n"]

    for subset_lines in lines:
        for l in subset_lines:
            all_lines.append(l + "\n")
        all_lines.append("\n")
        all_lines.append("\n")

    with open(output_path, "w") as f:
        f.writelines(all_lines)


if __name__ == "__main__":

    repo_dir = subprocess.check_output(
        ["git", "rev-parse", "--show-toplevel"]).decode("utf-8").rstrip("\n")
    lines = read_file_lines(os.path.join(repo_dir, ENUM_FILE_PATH))

    lines = [l.rstrip('\n') for l in lines if l != '\n']

    enum_lines = find_enum_lines(lines)
    python_output_lines = []

    for el in enum_lines:
        parsed_enum = ParsedEnum(el)
        python_output_lines.append(parse_to_python_enum(parsed_enum))

    to_python_file(python_output_lines, os.path.join(
        repo_dir, PYTHON_OUTPUT_PATH))

    a = 1
