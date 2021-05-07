#include "layout_tools_window.h"

#include "main_window.h"

LayoutToolsWindow::LayoutToolsWindow(wxFrame* main_window, wxPoint pos, wxSize size) : wxFrame(main_window, wxID_ANY, "Layout tools", pos, size)
{
    this->SetMinSize(size);
    this->SetMaxSize(size);
    main_window_ = main_window;

    MainWindow* main_window_ptr = dynamic_cast<MainWindow*>(main_window_);
    Bind(wxEVT_CLOSE_WINDOW, &LayoutToolsWindow::OnClose, this);

    shapes_box_ = new wxStaticBox(this, wxID_ANY, "Shapes");
    inspector_box_ = new wxStaticBox(this, wxID_ANY, "Inspector");

    wxBoxSizer* global_sizer = new wxBoxSizer(wxVERTICAL);
    global_sizer->Add(shapes_box_, 1);
    global_sizer->Add(inspector_box_, 1);

    setupInspector();
    setupShapes();

    this->SetSizer(global_sizer);
}

void LayoutToolsWindow::OnClose(wxCloseEvent &event)
{
    this->Hide();
    MainWindow* main_window_ptr = dynamic_cast<MainWindow*>(main_window_);
    main_window_ptr->disableEditing();
}

void LayoutToolsWindow::setupInspector()
{
    MainWindow* main_window_ptr = dynamic_cast<MainWindow*>(main_window_);

    {
        wxBoxSizer* sizer_inside = new wxBoxSizer(wxVERTICAL);
        wxStaticText* tab_name_label = new wxStaticText(inspector_box_, wxID_ANY, "Tab name", wxDefaultPosition, wxDefaultSize);
        tab_name_ctrl_ = new wxTextCtrl(inspector_box_, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
        tab_name_ctrl_->Bind(wxEVT_TEXT_ENTER, &MainWindow::changeCurrentTabName, main_window_ptr);

        sizer_inside->Add(tab_name_label, 1, wxALIGN_CENTER_HORIZONTAL);
        sizer_inside->Add(tab_name_ctrl_, 1, wxALIGN_CENTER_HORIZONTAL);
        inspector_box_->SetSizer(sizer_inside);
    }
}

void LayoutToolsWindow::setupShapes()
{
    MainWindow* main_window_ptr = dynamic_cast<MainWindow*>(main_window_);

    {
        wxBoxSizer* sizer_inside = new wxBoxSizer(wxVERTICAL);
        wxButton* new_element_button = new wxButton(shapes_box_, wxID_ANY, "New element", wxDefaultPosition);
        wxButton* delete_element_button = new wxButton(shapes_box_, wxID_ANY, "Delete element", wxDefaultPosition);
        new_element_button->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::newElement, main_window_ptr);
        delete_element_button->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::deleteElement, main_window_ptr);

        sizer_inside->Add(new_element_button, 0, wxALIGN_CENTER_HORIZONTAL);
        sizer_inside->Add(delete_element_button, 0, wxALIGN_CENTER_HORIZONTAL);
        shapes_box_->SetSizer(sizer_inside);
    }
}

void LayoutToolsWindow::setPosAndSize(wxPoint pos, wxSize size)
{
    this->SetPosition(pos);
    this->SetSize(size);
}
