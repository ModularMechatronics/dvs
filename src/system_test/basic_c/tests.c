#include "basic_c/tests.h"

#include <stdio.h>

#include "dvs/dvs.h"

void testPlot()
{
    const size_t num_elements = 100;
    VectorF x = vector_linspaceFromPointsAndCountF(0.1, 10.8, num_elements);
    VectorF y = vector_sinF(x);

    setCurrentElement("p_view_0");
    clearView();
    const Vec3d min_vec = {-1.0, -1.0, -1.0};
    const Vec3d max_vec = {1.0, 1.0, 2.0};
    axis(min_vec, max_vec);

    plot(x, y, Color(0, 0, 255), LineWidth(53));
}

void testPlot3()
{
    const size_t num_elements = 100;
    VectorF x = vector_linspaceFromPointsAndCountF(0.1, 10.8, num_elements);
    VectorF y = vector_sinF(x);
    VectorF z = vector_cosF(x);

    setCurrentElement("p_view_0");
    clearView();
    plot3(x, y, z, Color(255, 0, 11));
}

void testScatter()
{
    const size_t num_elements = 100;
    VectorF x = vector_linspaceFromPointsAndCountF(0.1, 10.8, num_elements);
    VectorF y = vector_sinF(x);

    setCurrentElement("p_view_0");
    clearView();
    scatter(x, y, Color(0, 0, 255), PointSize(10));
}

void testScatter3()
{
    const size_t num_elements = 100;
    VectorF x = vector_linspaceFromPointsAndCountF(0.1, 10.8, num_elements);
    VectorF y = vector_sinF(x);
    VectorF z = vector_cosF(x);

    setCurrentElement("p_view_0");
    clearView();
    scatter3(x, y, z, Color(255, 0, 11));
}

void testSurf()
{
    const size_t num_rows = 100, num_cols = 80;
    const MatrixPairD mp = meshgridD(0.0, 4.0, 1.0, 5.0, num_cols, num_rows);

    const MatrixD x = mp.m0, y = mp.m1;

    const MatrixD rx = matrix_sinD(x);
    const MatrixD ry = matrix_cosD(y);
    const MatrixD z = matrix_elementWiseMultiplyD(rx, ry);

    setCurrentElement("p_view_0");
    clearView();

    surf(x, y, z, Alpha(0.7f));

    free(x.data);
    free(y.data);
    free(z.data);

    free(rx.data);
    free(ry.data);
}

/*void testDrawPolygonFrom4Points()
{
    Point3d p0 = {0.0, 0.0, 0.0};
    Point3d p1 = {0.0, 1.0, 1.0};
    Point3d p2 = {1.0, 1.0, 2.0};
    Point3d p3 = {0.0, 2.0, 0.6};

    setCurrentElement("view_02");
    clearView();

    drawPolygonFrom4Points(p0, p1, p2, p3, Color(255, 0, 11));
}

void testDrawLine3D()
{
    const Point3d p0 = createPoint3d(0.0, 0.0, 0.0);
    const Point3d p1 = createPoint3d(1.0, 1.0, 1.0);

    const Line3DD line = {p0, p1};

    setCurrentElement("p_view_0");
    clearView();
    drawLine3D(line, -1.3, 3.4, Color(255, 0, 11));
}

void testDrawLine2D()
{
    const Point2d p0 = {0.0, 0.0};
    const Point2d p1 = {1.0, 1.5};

    const PLine2DD line = {p0, p1};

    setCurrentElement("p_view_0");
    clearView();
    drawLine2D(line, -1.3, 3.4, Color(255, 0, 11));
}

void testDrawPlaneXY()
{
    const PlaneD plane = {0.0, 0.2, 1.0, 0.5};
    const PointXYd p0 = {0.0, 0.0};
    const PointXYd p1 = {1.0, 1.0};

    setCurrentElement("p_view_0");
    clearView();
    drawPlaneXY(plane, p0, p1, Color(255, 0, 11));
}

void testDrawPlaneXZ()
{
    const PlaneD plane = {0.0, 1.0, 0.2, 0.5};
    const PointXZd p0 = {0.0, 0.0};
    const PointXZd p1 = {1.0, 1.0};

    setCurrentElement("view_01");
    clearView();
    drawPlaneXZ(plane, p0, p1, Color(255, 0, 11));
}

void testDrawPlaneYZ()
{
    const PlaneD plane = {1.0, 0.0, 0.2, 0.5};
    const PointYZd p0 = {0.0, 0.0};
    const PointYZd p1 = {1.0, 1.0};

    setCurrentElement("view_02");
    clearView();
    drawPlaneYZ(plane, p0, p1, Color(255, 0, 11));
}*/

void testImShow()
{
    const uint32_t num_rows = 800, num_cols = 800;
    ImageC3F img3 = createImageC3F(num_rows, num_cols);
    ImageC1F img1 = createImageC1F(num_rows, num_cols);

    const double max_val = num_rows * num_cols;
    const size_t num_elements = num_rows * num_cols;

    for (uint32_t r = 0; r < num_rows; r++)
    {
        for (uint32_t c = 0; c < num_cols; c++)
        {
            const float xr = 3.0f * ((float)c - 300.5f) / 800.0f;
            const float yr = 3.0f * ((float)r - 400.5f) / 800.0f;
            const float rr = sqrt(xr * xr + yr * yr);

            const float xg = 2.0f * ((float)c - 500.5f) / 800.0f;
            const float yg = 2.0f * ((float)r - 350.5f) / 800.0f;
            const float rg = sqrt(xg * xg + yg * yg);

            const float xb = 4.0f * ((float)c - 200.5f) / 800.0f;
            const float yb = 4.0f * ((float)r - 600.5f) / 800.0f;
            const float rb = sqrt(xb * xb + yb * yb);

            img3.data[r * img3.num_cols + c] = (sin(rb) / rr + 1.0f) * 0.5f;
            img3.data[num_elements + r * img3.num_cols + c] = (sin(rb) / rg + 1.0f) * 0.5f;
            img3.data[2 * num_elements + r * img3.num_cols + c] = (sin(rb) / rb + 1.0f) * 0.5f;

            // img1.data[num_elements + r * img3.num_cols + c] = (sin(rb) / rb + 1.0f) * 0.5f;
        }
    }

    setCurrentElement("p_view_0");
    clearView();
    imShow(img3, Color(255, 0, 11));

    // TODO:
    // setCurrentElement("view_01");
    // clearView();
    // imShow(img1, Color(255, 0, 11));
}

/*void testDrawLineBetweenPoints()
{
    const Point3d p0 = {0.1, 0.1, 0.1};
    const Point3d p1 = {1.0, 2.0, 3.0};

    const Point2d p0_2 = {p0.x, p0.y};
    const Point2d p1_2 = {p1.x, p1.y};

    setCurrentElement("p_view_0");  // TODO: <- Move to setup
    clearView();
    drawLineBetweenPoints(p0, p1, Color(255, 0, 11));
    drawLineBetween2DPoints(p0_2, p1_2, Color(255, 0, 11));
}*/

// GUI

void slider0Callback(const SliderHandle gui_element_handle)
{
    const SliderHandle slider = getSliderHandle("slider0");

    printf("Callback function: \"%s\": %i\n", GET_HANDLE_STRING(gui_element_handle), getSliderValue(slider));

    const TextLabelHandle text_label = getTextLabelHandle("text_label0");

    char text[100];
    sprintf(text, "Slider value: %i\n", getSliderValue(slider));

    setTextLabelValue(text_label, text);
}

void button0Callback(const ButtonHandle gui_element_handle)
{
    printf("Callback function: \"%s\"\n", GET_HANDLE_STRING(gui_element_handle));
}

void listBoxCallback(const ListBoxHandle gui_element_handle)
{
    printf("Callback function: \"%s\"\n", GET_HANDLE_STRING(gui_element_handle));
    printf("Selected element: %s\n", getListBoxSelectedElement(gui_element_handle));

    for (size_t k = 0U; k < getListBoxElements(gui_element_handle).size; k++)
    {
        printf("Element %zu: %s\n", k, getListBoxElements(gui_element_handle).elements[k]);
    }
}

void dropDownMenuCallback(const DropDownMenuHandle gui_element_handle)
{
    printf("Callback function: \"%s\"\n", GET_HANDLE_STRING(gui_element_handle));
    printf("Selected element: %s\n", getDropDownMenuSelectedElement(gui_element_handle));

    for (size_t k = 0U; k < getDropDownMenuElements(gui_element_handle).size; k++)
    {
        printf("Element %zu: %s\n", k, getDropDownMenuElements(gui_element_handle).elements[k]);
    }
}

void radioButtonsCallback(const RadioButtonGroupHandle gui_element_handle)
{
    printf("Callback function: \"%s\"\n", GET_HANDLE_STRING(gui_element_handle));
    printf("Selected element idx: %i\n", getRadioButtonGroupSelectedButtonIdx(gui_element_handle));

    for (size_t k = 0U; k < getRadioButtonGroupButtonNames(gui_element_handle).size; k++)
    {
        printf("Element %zu: %s\n", k, getRadioButtonGroupButtonNames(gui_element_handle).elements[k]);
    }
}
void editableTextCallback(const EditableTextHandle gui_element_handle)
{
    printf("Callback function: \"%s\"\n", GET_HANDLE_STRING(gui_element_handle));
    printf("Text: %s\n", getEditableTextValue(gui_element_handle));
}

void checkboxCallback(const CheckboxHandle gui_element_handle)
{
    printf("Callback function: \"%s\"\n", GET_HANDLE_STRING(gui_element_handle));
    printf("Checked: %i\n", getIsCheckBoxChecked(gui_element_handle));
}

void printValues()
{
    const SliderHandle slider = getSliderHandle("slider0");
    const ListBoxHandle list_box = getListBoxHandle("listbox0");
    const DropDownMenuHandle drop_down_menu = getDropDownMenuHandle("ddm0");

    const TextLabelHandle text_label = getTextLabelHandle("text_label0");

    printf("Text label: %s\n", getTextLabelValue(text_label));

    printf("Listbox size: %zu\n", getListBoxElements(list_box).size);

    for (size_t k = 0U; k < getListBoxElements(list_box).size; k++)
    {
        printf("Element %zu: %s\n", k, getListBoxElements(list_box).elements[k]);
    }
    printf("Selected element: %s\n", getListBoxSelectedElement(list_box));

    printf("Slider value: %i\n", getSliderValue(slider));

    const RadioButtonGroupHandle rb_h = getRadioButtonGroupHandle("rbg0");
    printf("Selected element idx: %i\n", getRadioButtonGroupSelectedButtonIdx(rb_h));

    for (size_t k = 0U; k < getRadioButtonGroupButtonNames(rb_h).size; k++)
    {
        printf("Button %zu: %s\n", k, getRadioButtonGroupButtonNames(rb_h).elements[k]);
    }

    const EditableTextHandle et_h = getEditableTextHandle("text_entry");
    printf("Text: %s\n", getEditableTextValue(et_h));

    const CheckboxHandle cb_h = getCheckboxHandle("checkbox0");
    printf("Checked: %i\n", getIsCheckBoxChecked(cb_h));

    printf("DropDownMenu size: %zu\n", getDropDownMenuElements(drop_down_menu).size);

    for (size_t k = 0U; k < getDropDownMenuElements(drop_down_menu).size; k++)
    {
        printf("Element %zu: %s\n", k, getDropDownMenuElements(drop_down_menu).elements[k]);
    }

    printf("Selected element: %s\n", getDropDownMenuSelectedElement(drop_down_menu));
}

void testGUIBasic()
{
    startGuiReceiveThread();

    registerButtonCallback("button0", button0Callback);
    registerSliderCallback("slider0", slider0Callback);

    registerListBoxCallback("listbox0", listBoxCallback);
    registerDropDownMenuCallback("ddm0", dropDownMenuCallback);
    registerCheckboxCallback("checkbox0", checkboxCallback);
    registerRadioButtonGroupCallback("rbg0", radioButtonsCallback);
    registerEditableTextCallback("text_entry", editableTextCallback);

    char input_array[1000];

    while (true)
    {
        scanf("%s", input_array);

        if (strcmp(input_array, "q") == 0)
        {
            break;
        }
        else if (strcmp(input_array, "v") == 0)
        {
            printValues();
        }
    }
}
