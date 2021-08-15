import os
import json

RECIPE_FILE_PATH = "./code_recipes.json"
OUTPUT_PATH = "./output"

TYPE_DICT = {
    "F": {"suffix": "F", "type_name": "float", "enum_name": "DT_FLOAT"},
    "D": {"suffix": "D", "type_name": "double", "enum_name": "DT_DOUBLE"},
    "S8": {"suffix": "S8", "type_name": "int8_t", "enum_name": "DT_INT8"},
    "S16": {"suffix": "S16", "type_name": "int16_t", "enum_name": "DT_INT16"},
    "S32": {"suffix": "S32", "type_name": "int32_t", "enum_name": "DT_INT32"},
    "S64": {"suffix": "S64", "type_name": "int64_t", "enum_name": "DT_INT64"},
    "U8": {"suffix": "U8", "type_name": "uint8_t", "enum_name": "DT_UINT8"},
    "U16": {"suffix": "U16", "type_name": "uint16_t", "enum_name": "DT_UINT16"},
    "U32": {"suffix": "U32", "type_name": "uint32_t", "enum_name": "DT_UINT32"},
    "U64": {"suffix": "U64", "type_name": "uint64_t", "enum_name": "DT_UINT64"},
}

TYPE_SUFFIXES = list(TYPE_DICT.keys())


def parse_function(function_name, function_data):

    file_output_path = os.path.join(OUTPUT_PATH, function_name + ".h")

    if (len(function_data["meta_types"]) > 1) and ("mix_types" not in function_data.keys()):
        raise Exception(
            "If there are more than 1 meta type, the key \"mix_types\" must be in the dict!")

    types = TYPE_SUFFIXES if function_data["types"] == "all" else function_data["types"]
    meta_types = function_data["meta_types"]

    new_lines = []

    if len(function_data["meta_types"]) > 1:
        a = 1
        if function_data["mix_types"]:
            pass
        else:
            for l in function_data["body"]:
                a = 1
    else:
        for l in function_data["body"]:
            for t in types:
                a = 1


if __name__ == "__main__":

    if not os.path.exists(OUTPUT_PATH):
        os.makedirs(OUTPUT_PATH)

    with open(RECIPE_FILE_PATH, "r") as f:
        recipe_data = json.load(f)

    for func in recipe_data["functions"]:
        parse_function(func, recipe_data["functions"][func])

    a = 1
