#include "layout_tools_window.h"

#include "events.h"
#include "main_window.h"

LayoutToolsWindow::LayoutToolsWindow(wxFrame* main_window, wxPoint pos, wxSize size)
    : wxFrame(main_window, wxID_ANY, "Layout tools", pos, size)
{
    this->SetMinSize(size);
    this->SetMaxSize(size);
    main_window_ = main_window;

    Bind(wxEVT_CLOSE_WINDOW, &LayoutToolsWindow::OnClose, this);

    shapes_box_ = new wxStaticBox(this, wxID_ANY, "Shapes");
    inspector_box_ = new wxStaticBox(this, wxID_ANY, "Inspector");

    wxBoxSizer* global_sizer = new wxBoxSizer(wxVERTICAL);
    global_sizer->Add(shapes_box_, 1);
    global_sizer->Add(inspector_box_, 1);

    setupInspector();
    setupShapes();

    this->SetSizer(global_sizer);

    // Bind(MY_EVENT, &LayoutToolsWindow::currentElementNameChanged, this);
}

void LayoutToolsWindow::OnClose(wxCloseEvent& WXUNUSED(event))
{
    this->Hide();
    MainWindow* main_window_ptr = dynamic_cast<MainWindow*>(main_window_);
    main_window_ptr->disableEditing();
}

void LayoutToolsWindow::setupInspector()
{
    MainWindow* main_window_ptr = dynamic_cast<MainWindow*>(main_window_);

    wxBoxSizer* global_sizer = new wxBoxSizer(wxVERTICAL);

    {
        wxBoxSizer* sizer_inside = new wxBoxSizer(wxVERTICAL);
        wxStaticText* tab_name_label =
            new wxStaticText(inspector_box_, wxID_ANY, "Tab name", wxDefaultPosition, wxDefaultSize);
        tab_name_ctrl_ =
            new wxTextCtrl(inspector_box_, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
        tab_name_ctrl_->Bind(wxEVT_COMMAND_TEXT_UPDATED, &MainWindow::changeCurrentTabName, main_window_ptr);

        sizer_inside->Add(tab_name_label, 1, wxALIGN_CENTER_HORIZONTAL);
        sizer_inside->Add(tab_name_ctrl_, 1, wxALIGN_CENTER_HORIZONTAL);
        global_sizer->Add(sizer_inside);
    }
    {
        wxBoxSizer* sizer_inside = new wxBoxSizer(wxVERTICAL);
        wxStaticText* element_name_label =
            new wxStaticText(inspector_box_, wxID_ANY, "Element name", wxDefaultPosition, wxDefaultSize);
        element_name_ctrl_ =
            new wxTextCtrl(inspector_box_, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
        element_name_ctrl_->Bind(wxEVT_COMMAND_TEXT_UPDATED, &MainWindow::changeCurrentElementName, main_window_ptr);

        sizer_inside->Add(element_name_label, 1, wxALIGN_CENTER_HORIZONTAL);
        sizer_inside->Add(element_name_ctrl_, 1, wxALIGN_CENTER_HORIZONTAL);
        global_sizer->Add(sizer_inside);
    }
    inspector_box_->SetSizer(global_sizer);
}

void LayoutToolsWindow::setupShapes()
{
    MainWindow* main_window_ptr = dynamic_cast<MainWindow*>(main_window_);

    wxBoxSizer* global_sizer = new wxBoxSizer(wxVERTICAL);

    {
        wxBoxSizer* sizer_inside = new wxBoxSizer(wxVERTICAL);
        wxButton* new_tab_button = new wxButton(shapes_box_, wxID_ANY, "New tab", wxDefaultPosition);
        wxButton* delete_tab_button = new wxButton(shapes_box_, wxID_ANY, "Delete tab", wxDefaultPosition);
        new_tab_button->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::addNewTabCallback, main_window_ptr);
        delete_tab_button->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::deleteTab, main_window_ptr);

        sizer_inside->Add(new_tab_button, 0, wxALIGN_CENTER_HORIZONTAL);
        sizer_inside->Add(delete_tab_button, 0, wxALIGN_CENTER_HORIZONTAL);
        global_sizer->Add(sizer_inside);
    }
    {
        wxBoxSizer* sizer_inside = new wxBoxSizer(wxVERTICAL);
        wxButton* new_window_button = new wxButton(shapes_box_, wxID_ANY, "New window", wxDefaultPosition);
        wxButton* delete_window_button = new wxButton(shapes_box_, wxID_ANY, "Delete window", wxDefaultPosition);
        new_window_button->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::addNewWindowCallback, main_window_ptr);
        delete_window_button->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::deleteWindow, main_window_ptr);

        sizer_inside->Add(new_window_button, 0, wxALIGN_CENTER_HORIZONTAL);
        sizer_inside->Add(delete_window_button, 0, wxALIGN_CENTER_HORIZONTAL);
        global_sizer->Add(sizer_inside);
    }
    {
        wxBoxSizer* sizer_inside = new wxBoxSizer(wxVERTICAL);
        wxButton* new_element_button = new wxButton(shapes_box_, wxID_ANY, "New element", wxDefaultPosition);
        wxButton* delete_element_button = new wxButton(shapes_box_, wxID_ANY, "Delete element", wxDefaultPosition);
        new_element_button->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::newElement, main_window_ptr);
        delete_element_button->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::deleteElement, main_window_ptr);

        sizer_inside->Add(new_element_button, 0, wxALIGN_CENTER_HORIZONTAL);
        sizer_inside->Add(delete_element_button, 0, wxALIGN_CENTER_HORIZONTAL);
        global_sizer->Add(sizer_inside);
    }
    shapes_box_->SetSizer(global_sizer);
}

void LayoutToolsWindow::currentTabChanged(const std::string& tab_name)
{
    tab_name_ctrl_->SetEvtHandlerEnabled(false);
    tab_name_ctrl_->SetValue(tab_name);
    tab_name_ctrl_->SetEvtHandlerEnabled(true);
}

void LayoutToolsWindow::currentElementSelectionChanged(const std::string& name_of_selected)
{
    element_name_ctrl_->SetEvtHandlerEnabled(false);
    element_name_ctrl_->SetValue(name_of_selected);
    element_name_ctrl_->SetEvtHandlerEnabled(true);
}

void LayoutToolsWindow::currentElementNameChanged(wxCommandEvent& event)
{
    const wxString value = event.GetString();

    element_name_ctrl_->SetEvtHandlerEnabled(false);
    element_name_ctrl_->SetValue(value);
    element_name_ctrl_->SetEvtHandlerEnabled(true);
}

void LayoutToolsWindow::setPosAndSize(wxPoint pos, wxSize size)
{
    this->SetPosition(pos);
    this->SetSize(size);
}
