import main_tests
import sys

FUNCTION_NAMES = [q for q in main_tests.__dir__() if q.startswith("test_")]


def print_help():
    print("Usage:")
    print(
        f'python3 main.py [{", ".join(FUNCTION_NAMES + ["all"])}]')


if __name__ == "__main__":

    if len(sys.argv) == 1:
        print(f"No enough input arguments: {sys.argv}")
        print_help()
    elif len(sys.argv) == 2:
        fn_name = sys.argv[1]

        if fn_name in ["-h", "--help", "help"]:
            print_help()
        else:
            if fn_name == "all":
                for fn in FUNCTION_NAMES:
                    eval("main_tests.test_" + fn + "()")
            elif ("test_" + fn_name) in FUNCTION_NAMES:
                eval("main_tests.test_" + fn_name + "()")
            else:
                print(f"Function \"{fn_name}\" not in available functions!")
                print_help()
    else:
        print(f"Too many input arguments: {sys.argv}")
        print_help()
