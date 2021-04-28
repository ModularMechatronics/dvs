#include "layout_tools_window.h"

#include "main_window.h"

LayoutToolsWindow::LayoutToolsWindow(wxFrame* main_window, wxPoint pos, wxSize size) : wxFrame(main_window, wxID_ANY, "Layout tools", pos, size)
{
    this->SetMinSize(size);
    this->SetMaxSize(size);

    MainWindow* main_window_ptr = dynamic_cast<MainWindow*>(main_window);

    wxStaticBox* cells_size_box = new wxStaticBox(this, wxID_ANY, "Cell size");
    wxStaticBox* margin_left_right_box = new wxStaticBox(this, wxID_ANY, "Margin left right");
    wxStaticBox* margin_top_bottom_box = new wxStaticBox(this, wxID_ANY, "Margin top bottom");
    wxStaticBox* right_box = new wxStaticBox(this, wxID_ANY, "Shapes");

    wxBoxSizer* left_sizer = new wxBoxSizer(wxVERTICAL);
    left_sizer->Add(cells_size_box, 1, wxEXPAND);
    left_sizer->Add(margin_left_right_box, 1, wxEXPAND);
    left_sizer->Add(margin_top_bottom_box, 1, wxEXPAND);

    wxBoxSizer* global_sizer = new wxBoxSizer(wxHORIZONTAL);
    global_sizer->Add(left_sizer, 1);
    global_sizer->Add(right_box, 1);

    {
        wxBoxSizer* sizer_inside = new wxBoxSizer(wxHORIZONTAL);
        wxButton* cell_size_dec = new wxButton(cells_size_box, wxID_ANY, "-", wxPoint(0, 0), wxSize(30, 20));
        wxButton* cell_size_inc = new wxButton(cells_size_box, wxID_ANY, "+", wxPoint(0, 0), wxSize(30, 20));
        cell_size_inc->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::cellSizeInc, main_window_ptr);
        cell_size_dec->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::cellSizeDec, main_window_ptr);

        wxStaticText* cell_size_text = new wxStaticText(cells_size_box, wxID_ANY, "5", wxPoint(0, 0), wxSize(30, 20));

        sizer_inside->Add(cell_size_dec, 0, wxALIGN_CENTER_HORIZONTAL);
        sizer_inside->Add(cell_size_text, 0, wxALIGN_CENTER_HORIZONTAL);
        sizer_inside->Add(cell_size_inc, 0, wxALIGN_CENTER_HORIZONTAL);
        cells_size_box->SetSizer(sizer_inside);
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
