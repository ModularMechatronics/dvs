import os

type_dict = {
    "float": {"suffix": "F", "type_name": "float", "enum_name": "DT_FLOAT"},
    "double": {"suffix": "D", "type_name": "double", "enum_name": "DT_DOUBLE"},
    "int8": {"suffix": "S8", "type_name": "int8_t", "enum_name": "DT_INT8"},
    "int16": {"suffix": "S16", "type_name": "int16_t", "enum_name": "DT_INT16"},
    "int32": {"suffix": "S32", "type_name": "int32_t", "enum_name": "DT_INT32"},
    "int64": {"suffix": "S64", "type_name": "int64_t", "enum_name": "DT_INT64"},
    "uint8": {"suffix": "U8", "type_name": "uint8_t", "enum_name": "DT_UINT8"},
    "uint16": {"suffix": "U16", "type_name": "uint16_t", "enum_name": "DT_UINT16"},
    "uint32": {"suffix": "U32", "type_name": "uint32_t", "enum_name": "DT_UINT32"},
    "uint64": {"suffix": "U64", "type_name": "uint64_t", "enum_name": "DT_UINT64"},
}

types = list(type_dict.keys())
structures = ["Vector", "Matrix"]  # , "Image"

vector_cast_string = """__OUTPUT_TYPE__ cast__INPUT_TYPE__To__OUTPUT_TYPE__(const __INPUT_TYPE__ input)
{
    __OUTPUT_TYPE__ output = create__OUTPUT_TYPE__(input.num_elements);
    for(size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}"""

matrix_cast_string = """__OUTPUT_TYPE__ cast__INPUT_TYPE__To__OUTPUT_TYPE__(const __INPUT_TYPE__ input)
{
    __OUTPUT_TYPE__ output = create__OUTPUT_TYPE__(input.num_rows, input.num_cols);
    for(size_t r = 0; r < input.num_rows; r++)
    {
        for(size_t r = 0; r < input.num_rows; r++)
        {
            const size_t idx = r * input.num_cols + c;
            output.data[idx] = input.data[idx];
        }
    }

    return output;
}"""

vector_elementwise_multiply_string = """__INPUT_TYPE__ elementWiseMultiply__INPUT_TYPE__(const __INPUT_TYPE__ v0, const __INPUT_TYPE__ v1)
{
    __INPUT_TYPE__ output = create__INPUT_TYPE__(v0.num_elements);
    for(size_t k = 0; k < v0.num_elements; k++)
    {
        output.data[k] = v0.data[k] * v1.data[k];
    }

    return output;
}"""

vector_divide_string = """__INPUT_TYPE__ divide__INPUT_TYPE__(const __INPUT_TYPE__ v0, const __INPUT_TYPE__ v1)
{
    __INPUT_TYPE__ output = create__INPUT_TYPE__(v0.num_elements);
    for(size_t k = 0; k < v0.num_elements; k++)
    {
        output.data[k] = v0.data[k] / v1.data[k];
    }

    return output;
}"""

vector_add_string = """__INPUT_TYPE__ add__INPUT_TYPE__(const __INPUT_TYPE__ v0, const __INPUT_TYPE__ v1)
{
    __INPUT_TYPE__ output = create__INPUT_TYPE__(v0.num_elements);
    for(size_t k = 0; k < v0.num_elements; k++)
    {
        output.data[k] = v0.data[k] + v1.data[k];
    }

    return output;
}"""

vector_subtract_string = """__INPUT_TYPE__ subtract__INPUT_TYPE__(const __INPUT_TYPE__ v0, const __INPUT_TYPE__ v1)
{
    __INPUT_TYPE__ output = create__INPUT_TYPE__(v0.num_elements);
    for(size_t k = 0; k < v0.num_elements; k++)
    {
        output.data[k] = v0.data[k] - v1.data[k];
    }

    return output;
}"""


def create_typed_input_function(function_string, input_type):
    function_string_lines = function_string.split("\n")
    typed_lines = []
    for l in function_string_lines:
        typed_lines.append(l.replace("__INPUT_TYPE__", input_type))

    typed_lines.append("")
    return typed_lines


def create_typed_input_output_function(function_string, input_type, output_type):
    function_string_lines = function_string.split("\n")
    typed_lines = []
    for l in function_string_lines:
        typed_lines.append(l.replace("__INPUT_TYPE__", input_type).replace(
            "__OUTPUT_TYPE__", output_type))

    typed_lines.append("")
    return typed_lines


two_input_functions = {
    "cast_vector": {"file_name": "vector_cast.h", "function_string": vector_cast_string, "type_name": "Vector"},
    "cast_matrix": {"file_name": "matrix_cast.h", "function_string": matrix_cast_string, "type_name": "Matrix"}
}

one_input_functions = {
    "vector_multiply": {"file_name": "vector_multiply.h", "function_string": vector_elementwise_multiply_string, "type_name": "Vector"},
    "vector_divide": {"file_name": "vector_divide.h", "function_string": vector_divide_string, "type_name": "Vector"},
    "vector_add": {"file_name": "vector_add.h", "function_string": vector_add_string, "type_name": "Vector"},
    "vector_subtract": {"file_name": "vector_subtract.h", "function_string": vector_subtract_string, "type_name": "Vector"}
}


if __name__ == "__main__":

    for fcn in two_input_functions:
        data = two_input_functions[fcn]

        all_lines = []

        for input_type in types:
            for output_type in types:
                if input_type != output_type:

                    input_type_name = data["type_name"] + \
                        type_dict[input_type]["suffix"]
                    output_type_name = data["type_name"] + \
                        type_dict[output_type]["suffix"]

                    typed_function_lines = create_typed_input_output_function(
                        data["function_string"], input_type_name, output_type_name)

                    all_lines.extend(typed_function_lines)

        all_lines = [f + "\n" for f in all_lines]

        output_file_path = data["file_name"]

        with open(output_file_path, "w") as f:
            f.writelines(all_lines)

    for fcn in one_input_functions:
        data = one_input_functions[fcn]

        all_lines = []

        for input_type in types:

            input_type_name = data["type_name"] + \
                type_dict[input_type]["suffix"]

            typed_function_lines = create_typed_input_function(
                data["function_string"], input_type_name)

            all_lines.extend(typed_function_lines)

        all_lines = [f + "\n" for f in all_lines]

        output_file_path = data["file_name"]

        with open(output_file_path, "w") as f:
            f.writelines(all_lines)
