#ifndef LAYOUT_TOOLS_WINDOW_H_
#define LAYOUT_TOOLS_WINDOW_H_

#include <wx/button.h>
#include <wx/frame.h>
#include <wx/notebook.h>
#include <wx/textctrl.h>
#include <wx/timer.h>
#include <wx/menu.h>
#include <wx/wx.h>

#include <mutex>
#include <string>
#include <map>
#include <thread>
#include <tuple>
#include <utility>
#include <vector>

class LayoutToolsWindow : public wxFrame
{
private:

public:

    LayoutToolsWindow() = default;
    LayoutToolsWindow(wxFrame* main_window, wxPoint pos, wxSize size) : wxFrame(main_window, wxID_ANY, "Layout tools", pos, size)
    {
        this->SetMinSize(size);
        this->SetMaxSize(size);

        wxStaticBox* num_cells_x_box = new wxStaticBox(this, wxID_ANY, "Num cells x");
        wxStaticBox* num_cells_y_box = new wxStaticBox(this, wxID_ANY, "Num cells y");
        wxStaticBox* spacing_left_right_box = new wxStaticBox(this, wxID_ANY, "Spacing left right");
        wxStaticBox* spacing_top_bottom_box = new wxStaticBox(this, wxID_ANY, "Spacing top bottom");
        wxStaticBox* right_box = new wxStaticBox(this, wxID_ANY, "Shapes");

        wxBoxSizer* left_sizer = new wxBoxSizer(wxVERTICAL);
        left_sizer->Add(num_cells_x_box, 1, wxEXPAND);
        left_sizer->Add(num_cells_y_box, 1, wxEXPAND);
        left_sizer->Add(spacing_left_right_box, 1, wxEXPAND);
        left_sizer->Add(spacing_top_bottom_box, 1, wxEXPAND);

        wxBoxSizer* global_sizer = new wxBoxSizer(wxHORIZONTAL);
        global_sizer->Add(left_sizer, 1);
        global_sizer->Add(right_box, 1);

        {
            wxBoxSizer* sizer_inside = new wxBoxSizer(wxHORIZONTAL);
            wxButton* num_rows_dec = new wxButton(num_cells_x_box, wxID_ANY, "-", wxPoint(0, 0), wxSize(30, 20));
            wxButton* num_rows_inc = new wxButton(num_cells_x_box, wxID_ANY, "+", wxPoint(0, 0), wxSize(30, 20));

            wxStaticText* num_rows_text = new wxStaticText(num_cells_x_box, wxID_ANY, "5", wxPoint(0, 0), wxSize(30, 20));

            sizer_inside->Add(num_rows_dec, 0, wxALIGN_CENTER_HORIZONTAL);
            sizer_inside->Add(num_rows_text, 0, wxALIGN_CENTER_HORIZONTAL);
            sizer_inside->Add(num_rows_inc, 0, wxALIGN_CENTER_HORIZONTAL);
            num_cells_x_box->SetSizer(sizer_inside);
        }

        {
            wxBoxSizer* sizer_inside = new wxBoxSizer(wxHORIZONTAL);
            wxButton* num_cols_dec = new wxButton(num_cells_y_box, wxID_ANY, "-", wxPoint(0, 0), wxSize(30, 20));
            wxButton* num_cols_inc = new wxButton(num_cells_y_box, wxID_ANY, "+", wxPoint(0, 0), wxSize(30, 20));

            wxStaticText* num_cols_text = new wxStaticText(num_cells_y_box, wxID_ANY, "5", wxPoint(0, 0), wxSize(30, 20));

            sizer_inside->Add(num_cols_dec, 0, wxALIGN_CENTER_HORIZONTAL);
            sizer_inside->Add(num_cols_text, 0, wxALIGN_CENTER_HORIZONTAL);
            sizer_inside->Add(num_cols_inc, 0, wxALIGN_CENTER_HORIZONTAL);

            num_cells_y_box->SetSizer(sizer_inside);
        }


        this->SetSizer(global_sizer);
    }

    void setPosAndSize(wxPoint pos, wxSize size)
    {
        this->SetPosition(pos);
        this->SetSize(size);
    }
};

#endif
