
def print_elemet(current_element_number,  width, height, x, y):
    base_space = "                        "
    tab_space = "    "
    left_bracket = "{"
    right_bracket = "}"

    element_name = "p" + str(current_element_number)

    print(f'{base_space}{left_bracket}')

    print(f'{base_space}{tab_space}"handle_string": "{element_name}",')
    print(f'{base_space}{tab_space}"height": {height},')
    print(f'{base_space}{tab_space}"width": {width},')
    print(f'{base_space}{tab_space}"type": "PLOT_PANE",')
    print(f'{base_space}{tab_space}"x": {x},')
    print(f'{base_space}{tab_space}"y": {y},')

    print(f'{base_space}{tab_space}"element_specific_settings": {left_bracket}')
    print(f'{base_space}{tab_space}    "background_color": {left_bracket}')
    print(f'{base_space}{tab_space}        "b": 0.0,')
    print(f'{base_space}{tab_space}        "g": 0.0,')
    print(f'{base_space}{tab_space}        "r": 0.0')
    print(f'{base_space}{tab_space}    {right_bracket},')
    print(f'{base_space}{tab_space}    "axes_letters_on": false,')
    print(f'{base_space}{tab_space}    "axes_numbers_on": false,')
    print(f'{base_space}{tab_space}    "grid_on": false,')
    print(f'{base_space}{tab_space}    "plot_box_on": false,')
    print(f'{base_space}{tab_space}    "clipping_on": false')
    print(f'{base_space}{tab_space}{right_bracket}')

    print(f'{base_space}{right_bracket},')

if __name__ == "__main__":
    current_element_number = 0
    x = 0
    y = 0
    width = 1.0
    height = 0.5

    current_direction = "DOWN"
    n_elements = 15

    for i in range(0, n_elements):
        
        print_elemet(current_element_number, width, height, x, y)

        if current_direction == "DOWN":
            y = y + height
            width = width / 2
        
            current_direction = "RIGHT"

        elif current_direction == "RIGHT":
            x = x + width
            height = height / 2
        
            current_direction = "DOWN"

        else:
            raise Exception("No!")

        current_element_number = current_element_number + 1


