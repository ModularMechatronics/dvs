#include "layout_tools_window.h"

#include "main_window.h"

void LayoutToolsWindow::onButtonClickX(wxCommandEvent& event)
{
    std::cout << "Button pressed!!" << std::endl;
}

LayoutToolsWindow::LayoutToolsWindow(wxFrame* main_window, wxPoint pos, wxSize size) : wxFrame(main_window, wxID_ANY, "Layout tools", pos, size)
{
    this->SetMinSize(size);
    this->SetMaxSize(size);

    MainWindow* main_window_ptr = dynamic_cast<MainWindow*>(main_window);

    wxStaticBox* num_cells_x_box = new wxStaticBox(this, wxID_ANY, "Number of cells x");
    wxStaticBox* num_cells_y_box = new wxStaticBox(this, wxID_ANY, "Number of cells y");
    wxStaticBox* margin_left_right_box = new wxStaticBox(this, wxID_ANY, "Margin left right");
    wxStaticBox* margin_top_bottom_box = new wxStaticBox(this, wxID_ANY, "Margin top bottom");
    wxStaticBox* right_box = new wxStaticBox(this, wxID_ANY, "Shapes");

    wxBoxSizer* left_sizer = new wxBoxSizer(wxVERTICAL);
    left_sizer->Add(num_cells_x_box, 1, wxEXPAND);
    left_sizer->Add(num_cells_y_box, 1, wxEXPAND);
    left_sizer->Add(margin_left_right_box, 1, wxEXPAND);
    left_sizer->Add(margin_top_bottom_box, 1, wxEXPAND);

    wxBoxSizer* global_sizer = new wxBoxSizer(wxHORIZONTAL);
    global_sizer->Add(left_sizer, 1);
    global_sizer->Add(right_box, 1);

    {
        wxBoxSizer* sizer_inside = new wxBoxSizer(wxHORIZONTAL);
        wxButton* num_cells_x_dec = new wxButton(num_cells_x_box, wxID_ANY, "-", wxPoint(0, 0), wxSize(30, 20));
        wxButton* num_cells_x_inc = new wxButton(num_cells_x_box, wxID_ANY, "+", wxPoint(0, 0), wxSize(30, 20));
        num_cells_x_inc->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::numCellsXInc, main_window_ptr);
        num_cells_x_dec->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::numCellsXDec, main_window_ptr);

        wxStaticText* num_cells_x_text = new wxStaticText(num_cells_x_box, wxID_ANY, "5", wxPoint(0, 0), wxSize(30, 20));

        sizer_inside->Add(num_cells_x_dec, 0, wxALIGN_CENTER_HORIZONTAL);
        sizer_inside->Add(num_cells_x_text, 0, wxALIGN_CENTER_HORIZONTAL);
        sizer_inside->Add(num_cells_x_inc, 0, wxALIGN_CENTER_HORIZONTAL);
        num_cells_x_box->SetSizer(sizer_inside);
    }

    {
        wxBoxSizer* sizer_inside = new wxBoxSizer(wxHORIZONTAL);
        wxButton* num_cells_y_dec = new wxButton(num_cells_y_box, wxID_ANY, "-", wxPoint(0, 0), wxSize(30, 20));
        wxButton* num_cells_y_inc = new wxButton(num_cells_y_box, wxID_ANY, "+", wxPoint(0, 0), wxSize(30, 20));
        num_cells_y_inc->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::numCellsYInc, main_window_ptr);
        num_cells_y_dec->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::numCellsYDec, main_window_ptr);

        wxStaticText* num_cells_y_text = new wxStaticText(num_cells_y_box, wxID_ANY, "5", wxPoint(0, 0), wxSize(30, 20));

        sizer_inside->Add(num_cells_y_dec, 0, wxALIGN_CENTER_HORIZONTAL);
        sizer_inside->Add(num_cells_y_text, 0, wxALIGN_CENTER_HORIZONTAL);
        sizer_inside->Add(num_cells_y_inc, 0, wxALIGN_CENTER_HORIZONTAL);
        num_cells_y_box->SetSizer(sizer_inside);
    }

    {
        wxBoxSizer* sizer_inside = new wxBoxSizer(wxHORIZONTAL);
        wxButton* margin_left_right_dec = new wxButton(margin_left_right_box, wxID_ANY, "-", wxPoint(0, 0), wxSize(30, 20));
        wxButton* margin_left_right_inc = new wxButton(margin_left_right_box, wxID_ANY, "+", wxPoint(0, 0), wxSize(30, 20));
        margin_left_right_inc->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::marginXInc, main_window_ptr);
        margin_left_right_dec->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::marginXDec, main_window_ptr);

        wxStaticText* margin_left_right_text = new wxStaticText(margin_left_right_box, wxID_ANY, "5", wxPoint(0, 0), wxSize(30, 20));

        sizer_inside->Add(margin_left_right_dec, 0, wxALIGN_CENTER_HORIZONTAL);
        sizer_inside->Add(margin_left_right_text, 0, wxALIGN_CENTER_HORIZONTAL);
        sizer_inside->Add(margin_left_right_inc, 0, wxALIGN_CENTER_HORIZONTAL);

        margin_left_right_box->SetSizer(sizer_inside);
    }

    {
        wxBoxSizer* sizer_inside = new wxBoxSizer(wxHORIZONTAL);
        wxButton* margin_top_bottom_dec = new wxButton(margin_top_bottom_box, wxID_ANY, "-", wxPoint(0, 0), wxSize(30, 20));
        wxButton* margin_top_bottom_inc = new wxButton(margin_top_bottom_box, wxID_ANY, "+", wxPoint(0, 0), wxSize(30, 20));
        margin_top_bottom_inc->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::marginYInc, main_window_ptr);
        margin_top_bottom_dec->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::marginYDec, main_window_ptr);

        wxStaticText* margin_top_bottom_text = new wxStaticText(margin_top_bottom_box, wxID_ANY, "5", wxPoint(0, 0), wxSize(30, 20));

        sizer_inside->Add(margin_top_bottom_dec, 0, wxALIGN_CENTER_HORIZONTAL);
        sizer_inside->Add(margin_top_bottom_text, 0, wxALIGN_CENTER_HORIZONTAL);
        sizer_inside->Add(margin_top_bottom_inc, 0, wxALIGN_CENTER_HORIZONTAL);

        margin_top_bottom_box->SetSizer(sizer_inside);
    }


    this->SetSizer(global_sizer);
}

void LayoutToolsWindow::setPosAndSize(wxPoint pos, wxSize size)
{
    this->SetPosition(pos);
    this->SetSize(size);
}
