#include "gui_window.h"

#include <ctime>
#include <iomanip>

#include "dvs/internal.h"
#include "events.h"
#include "globals.h"
#include "main_window.h"
#include "plot_pane.h"

namespace element_number_counter
{
int getNextFreeElementNumber();
}

GuiWindow::GuiWindow(
    wxFrame* main_window,
    const WindowSettings& window_settings,
    const std::string& project_name,
    const int callback_id,
    const bool project_is_saved,
    const std::function<void(const char key)>& notify_main_window_key_pressed,
    const std::function<void(const char key)>& notify_main_window_key_released,
    const std::function<std::vector<std::string>(void)>& get_all_element_names,
    const std::function<void(const std::string&)>& notify_main_window_element_deleted,
    const std::function<void(const std::string&, const std::string&)>& notify_main_window_element_name_changed,
    const std::function<void(const std::string&, const std::string&)>& notify_main_window_name_changed,
    const std::function<void()>& notify_main_window_about_modification)
    : wxFrame(main_window, wxID_ANY, "Figure 1"),
      tab_buttons_{this,
                   window_settings,
                   [this](const std::string name) { tabChanged(name); },
                   [this](const wxPoint pos, const std::string& item_name) {
                       mouseRightPressed(pos, ClickSource::TAB_BUTTON, item_name);
                   }},
      notify_main_window_key_pressed_{notify_main_window_key_pressed},
      notify_main_window_key_released_{notify_main_window_key_released},
      notify_main_window_element_deleted_{notify_main_window_element_deleted},
      get_all_element_names_{get_all_element_names},
      notify_main_window_element_name_changed_{notify_main_window_element_name_changed},
      notify_main_window_name_changed_{notify_main_window_name_changed},
      notify_main_window_about_modification_{notify_main_window_about_modification},
      help_pane_{this, wxPoint(150, 150), wxSize(100, 100), this->GetSize()}
{
    project_name_ = project_name;
    main_window_ = main_window;
    current_tab_num_ = 0;
    callback_id_ = callback_id;
    dialog_color_ = RGBTripletf(0.5, 0.0, 0.0);
    project_is_saved_ = project_is_saved;

    notify_parent_window_right_mouse_pressed_ = [this](const wxPoint pos, const std::string& item_name) {
        mouseRightPressed(pos, ClickSource::GUI_ELEMENT, item_name);
    };

    name_ = window_settings.name;
    updateLabel();

    Show();

    if (window_settings.tabs.size() == 0)
    {
        element_x_offset_ = 0;
        TabSettings tab_settings;
        tab_settings.name = "Tab " + std::to_string(current_tab_num_);
        tabs_.push_back(new WindowTab(this,
                                      tab_settings,
                                      element_x_offset_,
                                      notify_main_window_key_pressed,
                                      notify_main_window_key_released,
                                      notify_parent_window_right_mouse_pressed_,
                                      notify_main_window_element_deleted_,
                                      notify_main_window_about_modification_));
        tab_buttons_.addNewTab(tab_settings.name);
        tab_buttons_.setSelection(tab_settings.name);
        SetBackgroundColour(wxColour(tab_settings.background_color.red * 255.0f,
                                     tab_settings.background_color.green * 255.0f,
                                     tab_settings.background_color.blue * 255.0f));
        current_tab_num_++;
    }
    else
    {
        if (window_settings.tabs.size() == 1)
        {
            element_x_offset_ = 0;
        }
        else
        {
            element_x_offset_ = 70;
        }

        for (size_t k = 0; k < window_settings.tabs.size(); k++)
        {
            tabs_.push_back(new WindowTab(this,
                                          window_settings.tabs[k],
                                          element_x_offset_,
                                          notify_main_window_key_pressed,
                                          notify_main_window_key_released,
                                          notify_parent_window_right_mouse_pressed_,
                                          notify_main_window_element_deleted_,
                                          notify_main_window_about_modification_));
            current_tab_num_++;
        }
    }

    if (window_settings.tabs.size() > 0)
    {
        for (const auto& tab : tabs_)
        {
            if (tab->getName() == window_settings.tabs[0].name)
            {
                tab->show();
                const auto c = tab->getBackgroundColor();
                SetBackgroundColour(wxColour(c.red * 255.0f, c.green * 255.0f, c.blue * 255.0f));
            }
            else
            {
                tab->hide();
            }
            tab->setMinXPos(element_x_offset_);
        }
    }

    popup_menu_window_ = new wxMenu(wxT(""));
    popup_menu_element_ = new wxMenu(wxT(""));
    popup_menu_tab_ = new wxMenu(wxT(""));

    popup_menu_window_->AppendSeparator();
    popup_menu_window_->Append(dvs_ids::EDIT_WINDOW_NAME, wxT("Edit window name"));
    popup_menu_window_->Append(callback_id_ + 1, wxT("Delete window"));
    popup_menu_window_->AppendSeparator();
    popup_menu_window_->AppendRadioItem(dvs_ids::TOGGLE_TO_ZOOM, wxT("Zoom"));
    popup_menu_window_->AppendRadioItem(dvs_ids::TOGGLE_TO_PAN, wxT("Pan"));
    popup_menu_window_->AppendRadioItem(dvs_ids::TOGGLE_TO_ROTATE, wxT("Rotate"));
    popup_menu_window_->AppendRadioItem(dvs_ids::TOGGLE_TO_SELECT, wxT("Select"));
    popup_menu_window_->AppendSeparator();
    popup_menu_window_->Append(callback_id_ + 2, wxT("New window"));
    popup_menu_window_->Append(dvs_ids::NEW_TAB, wxT("New tab"));
    popup_menu_window_->Append(dvs_ids::NEW_ELEMENT, wxT("New element"));

    popup_menu_element_->Append(dvs_ids::EDIT_ELEMENT_NAME, wxT("Edit element name"));
    popup_menu_element_->Append(dvs_ids::DELETE_ELEMENT, wxT("Delete element"));
    popup_menu_element_->Append(dvs_ids::TOGGLE_PROJECTION_MODE, wxT("Toggle projection mode"));
    popup_menu_element_->Append(dvs_ids::RAISE_ELEMENT, wxT("Raise"));
    popup_menu_element_->Append(dvs_ids::LOWER_ELEMENT, wxT("Lower"));
    popup_menu_element_->AppendSeparator();
    popup_menu_element_->AppendRadioItem(dvs_ids::TOGGLE_TO_ZOOM, wxT("Zoom"));
    popup_menu_element_->AppendRadioItem(dvs_ids::TOGGLE_TO_PAN, wxT("Pan"));
    popup_menu_element_->AppendRadioItem(dvs_ids::TOGGLE_TO_ROTATE, wxT("Rotate"));
    popup_menu_element_->AppendRadioItem(dvs_ids::TOGGLE_TO_SELECT, wxT("Select"));
    popup_menu_element_->AppendSeparator();
    popup_menu_element_->Append(dvs_ids::EDIT_WINDOW_NAME, wxT("Edit window name"));
    popup_menu_element_->Append(callback_id_ + 1, wxT("Delete window"));
    popup_menu_element_->AppendSeparator();
    popup_menu_element_->Append(callback_id_ + 2, wxT("New window"));
    popup_menu_element_->Append(dvs_ids::NEW_TAB, wxT("New tab"));
    popup_menu_element_->Append(dvs_ids::NEW_ELEMENT, wxT("New element"));

    popup_menu_tab_->Append(dvs_ids::EDIT_TAB_NAME, wxT("Edit tab name"));
    popup_menu_tab_->Append(dvs_ids::DELETE_TAB, wxT("Delete tab"));
    popup_menu_tab_->AppendSeparator();
    popup_menu_tab_->AppendRadioItem(dvs_ids::TOGGLE_TO_ZOOM, wxT("Zoom"));
    popup_menu_tab_->AppendRadioItem(dvs_ids::TOGGLE_TO_PAN, wxT("Pan"));
    popup_menu_tab_->AppendRadioItem(dvs_ids::TOGGLE_TO_ROTATE, wxT("Rotate"));
    popup_menu_tab_->AppendRadioItem(dvs_ids::TOGGLE_TO_SELECT, wxT("Select"));
    popup_menu_tab_->AppendSeparator();
    popup_menu_tab_->Append(dvs_ids::EDIT_WINDOW_NAME, wxT("Edit window name"));
    popup_menu_tab_->Append(callback_id_ + 1, wxT("Delete window"));
    popup_menu_tab_->AppendSeparator();
    popup_menu_tab_->Append(callback_id_ + 2, wxT("New window"));
    popup_menu_tab_->Append(dvs_ids::NEW_TAB, wxT("New tab"));
    popup_menu_tab_->Append(dvs_ids::NEW_ELEMENT, wxT("New element"));

    getMenuItemFromString(popup_menu_window_, "Rotate")->Check(true);
    getMenuItemFromString(popup_menu_element_, "Rotate")->Check(true);
    getMenuItemFromString(popup_menu_tab_, "Rotate")->Check(true);

    Bind(wxEVT_MENU, &GuiWindow::editWindowName, this, dvs_ids::EDIT_WINDOW_NAME);
    Bind(wxEVT_MENU, &MainWindow::deleteWindow, static_cast<MainWindow*>(main_window_), callback_id_ + 1);
    Bind(wxEVT_MENU, &MainWindow::newWindowCallback, static_cast<MainWindow*>(main_window_), callback_id_ + 2);
    Bind(wxEVT_MENU, &GuiWindow::newTab, this, dvs_ids::NEW_TAB);
    Bind(wxEVT_MENU, &GuiWindow::createNewPlotPaneCallbackFunction, this, dvs_ids::NEW_ELEMENT);
    Bind(wxEVT_MENU, &GuiWindow::editElementName, this, dvs_ids::EDIT_ELEMENT_NAME);
    Bind(wxEVT_MENU, &GuiWindow::deleteElement, this, dvs_ids::DELETE_ELEMENT);

    Bind(wxEVT_MENU, &GuiWindow::toggleToZoomCallback, this, dvs_ids::TOGGLE_TO_ZOOM);
    Bind(wxEVT_MENU, &GuiWindow::toggleToPanCallback, this, dvs_ids::TOGGLE_TO_PAN);
    Bind(wxEVT_MENU, &GuiWindow::toggleToRotateCallback, this, dvs_ids::TOGGLE_TO_ROTATE);
    Bind(wxEVT_MENU, &GuiWindow::toggleToSelectCallback, this, dvs_ids::TOGGLE_TO_SELECT);

    Bind(wxEVT_MENU, &GuiWindow::toggleProjectionMode, this, dvs_ids::TOGGLE_PROJECTION_MODE);
    Bind(wxEVT_MENU, &GuiWindow::raiseElement, this, dvs_ids::RAISE_ELEMENT);
    Bind(wxEVT_MENU, &GuiWindow::lowerElement, this, dvs_ids::LOWER_ELEMENT);
    Bind(wxEVT_MENU, &GuiWindow::editTabName, this, dvs_ids::EDIT_TAB_NAME);
    Bind(wxEVT_MENU, &GuiWindow::deleteTab, this, dvs_ids::DELETE_TAB);
    Bind(wxEVT_KEY_DOWN, &GuiWindow::keyPressedCallback, this);
    Bind(wxEVT_KEY_UP, &GuiWindow::keyReleasedCallback, this);

    Bind(wxEVT_CLOSE_WINDOW, &GuiWindow::OnClose, this);
    Bind(wxEVT_SIZE, &GuiWindow::OnSize, this);
    Bind(wxEVT_MOVE, &GuiWindow::OnMove, this);

    Bind(wxEVT_RIGHT_DOWN, &GuiWindow::mouseRightPressedCallback, this);

    tab_buttons_.windowWasResized(this->GetSize());

    this->SetPosition(wxPoint(window_settings.x, window_settings.y));
    this->SetSize(wxSize(window_settings.width, window_settings.height));
}

wxMenuItem* GuiWindow::getMenuItemFromString(const wxMenu* const menu, const std::string& menu_item_string) const
{
    for (size_t i = 0; i < menu->GetMenuItemCount(); ++i)
    {
        wxMenuItem* menu_item = menu->FindItemByPosition(i);
        if (menu_item->GetItemLabelText().ToStdString() == menu_item_string)
        {
            return menu_item;
        }
    }

    return nullptr;
}

GuiWindow::~GuiWindow()
{
    for (const auto& tab : tabs_)
    {
        delete tab;
    }
}

void GuiWindow::deleteAllTabs()
{
    for (const auto& tab : tabs_)
    {
        delete tab;
    }

    tabs_.clear();
}

void GuiWindow::setProjectName(const std::string& project_name)
{
    project_name_ = project_name;
}

std::vector<ApplicationGuiElement*> GuiWindow::getGuiElements() const
{
    std::vector<ApplicationGuiElement*> gui_elements;

    for (const auto& tab : tabs_)
    {
        std::vector<ApplicationGuiElement*> tab_gui_elements = tab->getGuiElements();
        gui_elements.insert(gui_elements.end(), tab_gui_elements.begin(), tab_gui_elements.end());
    }

    return gui_elements;
}

std::vector<ApplicationGuiElement*> GuiWindow::getPlotPanes() const
{
    std::vector<ApplicationGuiElement*> plot_panes;

    for (const auto& tab : tabs_)
    {
        std::vector<ApplicationGuiElement*> tab_plot_panes = tab->getPlotPanes();
        plot_panes.insert(plot_panes.end(), tab_plot_panes.begin(), tab_plot_panes.end());
    }

    return plot_panes;
}

std::vector<ApplicationGuiElement*> GuiWindow::getAllGuiElements() const
{
    std::vector<ApplicationGuiElement*> gui_elements;

    for (const auto& tab : tabs_)
    {
        std::vector<ApplicationGuiElement*> tab_gui_elements = tab->getAllGuiElements();
        gui_elements.insert(gui_elements.end(), tab_gui_elements.begin(), tab_gui_elements.end());
    }

    return gui_elements;
}

ApplicationGuiElement* GuiWindow::getGuiElement(const std::string& element_handle_string) const
{
    ApplicationGuiElement* ge;
    for (const auto& tab : tabs_)
    {
        ge = tab->getGuiElement(element_handle_string);
        if (ge != nullptr)
        {
            return ge;
        }
    }

    return nullptr;
}

void GuiWindow::keyPressedCallback(wxKeyEvent& evt)
{
    // TODO: This callback doesn't work
    const int key = evt.GetUnicodeKey();
    if (key == 'H')
    {
        help_pane_.show();
    }
    std::cout << "Key pressed" << std::endl;
}

void GuiWindow::keyReleasedCallback(wxKeyEvent& evt)
{
    // TODO: This callback doesn't work
    const int key = evt.GetUnicodeKey();
    if (key == 'H')
    {
        help_pane_.hide();
    }
}

void GuiWindow::notifyChildrenOnKeyPressed(const char key)
{
    if (key == 'H')
    {
        help_pane_.show();
        return;
    }

    if (key == 'r' || key == 'R')
    {
        getMenuItemFromString(popup_menu_window_, "Rotate")->Check(true);
        getMenuItemFromString(popup_menu_element_, "Rotate")->Check(true);
        getMenuItemFromString(popup_menu_tab_, "Rotate")->Check(true);
    }
    else if (key == 'z' || key == 'Z')
    {
        getMenuItemFromString(popup_menu_window_, "Zoom")->Check(true);
        getMenuItemFromString(popup_menu_element_, "Zoom")->Check(true);
        getMenuItemFromString(popup_menu_tab_, "Zoom")->Check(true);
    }
    else if (key == 'p' || key == 'P')
    {
        getMenuItemFromString(popup_menu_window_, "Pan")->Check(true);
        getMenuItemFromString(popup_menu_element_, "Pan")->Check(true);
        getMenuItemFromString(popup_menu_tab_, "Pan")->Check(true);
    }
    else if (key == 's' || key == 'S')
    {
        getMenuItemFromString(popup_menu_window_, "Select")->Check(true);
        getMenuItemFromString(popup_menu_element_, "Select")->Check(true);
        getMenuItemFromString(popup_menu_tab_, "Select")->Check(true);
    }

    for (const auto& tab : tabs_)
    {
        tab->notifyChildrenOnKeyPressed(key);
    }
}

void GuiWindow::notifyChildrenOnKeyReleased(const char key)
{
    if (key == 'H')
    {
        help_pane_.hide();
        return;
    }

    for (const auto& tab : tabs_)
    {
        tab->notifyChildrenOnKeyReleased(key);
    }
}

void GuiWindow::mouseRightPressed(const wxPoint pos, const ClickSource source, const std::string& item_name)
{
    last_clicked_item_ = item_name;

    wxMenu menu(wxT(""));

    if (source == ClickSource::GUI_ELEMENT)
    {
        PopupMenu(popup_menu_element_, pos);
    }
    else if (source == ClickSource::TAB_BUTTON)
    {
        PopupMenu(popup_menu_tab_, pos);
    }
    else
    {
        PopupMenu(popup_menu_window_, pos);
    }
}

void GuiWindow::mouseRightPressedCallback(wxMouseEvent& event)
{
    mouseRightPressed(event.GetPosition(), ClickSource::THIS, "");
}

void GuiWindow::tabChanged(const std::string name)
{
    for (const auto& tab : tabs_)
    {
        if (tab->getName() == name)
        {
            tab->show();
            const auto c = tab->getBackgroundColor();
            SetBackgroundColour(wxColour(c.red * 255.0f, c.green * 255.0f, c.blue * 255.0f));
            Refresh();
        }
        else
        {
            tab->hide();
        }
    }
}

void GuiWindow::OnMove(wxMoveEvent& event)
{
    notify_main_window_about_modification_();
}

void GuiWindow::OnSize(wxSizeEvent& event)
{
    wxFrame::OnSize(event);
    const wxSize new_size = event.GetSize();

    tab_buttons_.windowWasResized(new_size);

    for (const auto& tab : tabs_)
    {
        tab->updateSizeFromParent(new_size);
    }

    help_pane_.updateParentSize(new_size);

    notify_main_window_about_modification_();
}

int GuiWindow::getCallbackId() const
{
    return callback_id_;
}

void GuiWindow::screenshot(const std::string& base_path)
{
    Raise();
    wxScreenDC dc_screen;

    wxCoord screen_width, screen_height;
    dc_screen.GetSize(&screen_width, &screen_height);

    const wxSize window_size = this->GetSize();
    const wxPoint window_pos = this->GetPosition();

    wxBitmap screenshot(screen_width, screen_height, -1);

    wxMemoryDC mem_dc;
    mem_dc.SelectObject(screenshot);

    mem_dc.StretchBlit(0, 0, screen_width, screen_height, &dc_screen, 0, 0, screen_width, screen_height);
    mem_dc.SelectObject(wxNullBitmap);

    wxImage image = screenshot.ConvertToImage();

    const wxRect rect(window_pos.x, window_pos.y, window_size.GetWidth(), window_size.GetHeight());

    wxImage cropped_image = image.GetSubImage(rect);

    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::stringstream ss;
    ss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");

    const std::string file_name = base_path + "Screenshot_" + ss.str() + "_" + name_ + ".png";
    cropped_image.SaveFile(file_name, wxBITMAP_TYPE_PNG);
}

void GuiWindow::updateLabel()
{
    std::string save_string = "";
    if (!project_is_saved_)
    {
        save_string = "*";
    }
    this->SetLabel(name_ + " [" + project_name_ + save_string + "]");
}

void GuiWindow::setName(const std::string& new_name)
{
    notify_main_window_name_changed_(name_, new_name);
    name_ = new_name;
    updateLabel();
}

void GuiWindow::OnClose(wxCloseEvent& WXUNUSED(event))
{
    notify_main_window_about_modification_();
    Hide();
}

WindowSettings GuiWindow::getWindowSettings() const
{
    WindowSettings ws;
    const wxPoint pos = GetPosition();
    const wxSize size = GetSize();
    ws.name = name_;

    ws.x = pos.x;
    ws.y = pos.y;
    ws.width = size.GetWidth();
    ws.height = size.GetHeight();

    for (const auto& t : tabs_)
    {
        ws.tabs.push_back(t->getTabSettings());
    }

    return ws;
}

std::string GuiWindow::getName() const
{
    return name_;
}

void GuiWindow::editWindowName(wxCommandEvent& WXUNUSED(event))
{
    wxTextEntryDialog name_dialog(this, "Enter the name for the window", "Enter window name", name_);

    name_dialog.SetBackgroundColour(
        wxColour(dialog_color_.red * 255.0f, dialog_color_.green * 255.0f, dialog_color_.blue * 255.0f));

    if (name_dialog.ShowModal() == wxID_CANCEL)
    {
        return;
    }

    const std::string window_name{name_dialog.GetValue().mb_str()};

    if (window_name.length() > 0)
    {
        setName(window_name);
        notify_main_window_about_modification_();
    }
}

void GuiWindow::setIsFileSavedForLabel(const bool is_saved)
{
    project_is_saved_ = is_saved;
    updateLabel();
}

void GuiWindow::newTab(wxCommandEvent& WXUNUSED(event))
{
    TabSettings tab_settings;
    tab_settings.name = "Tab " + std::to_string(current_tab_num_);
    current_tab_num_++;
    element_x_offset_ = 70;

    tabs_.push_back(new WindowTab(this,
                                  tab_settings,
                                  element_x_offset_,
                                  notify_main_window_key_pressed_,
                                  notify_main_window_key_released_,
                                  notify_parent_window_right_mouse_pressed_,
                                  notify_main_window_element_deleted_,
                                  notify_main_window_about_modification_));
    tab_buttons_.addNewTab(tab_settings.name);

    for (const auto& t : tabs_)
    {
        t->setMinXPos(element_x_offset_);
    }
    notify_main_window_about_modification_();
}

void GuiWindow::createNewPlotPane()
{
    const std::string selected_tab = tab_buttons_.getNameOfSelectedTab();

    auto q = std::find_if(tabs_.begin(), tabs_.end(), [&selected_tab](const WindowTab* const tb) -> bool {
        return selected_tab == tb->getName();
    });

    if (q != tabs_.end())
    {
        std::string element_handle_string;

        while (true)
        {
            element_handle_string = "element-" + std::to_string(element_number_counter::getNextFreeElementNumber());
            const std::vector<std::string> all_element_names = get_all_element_names_();

            auto q = std::find(all_element_names.begin(), all_element_names.end(), element_handle_string);

            const bool element_absent = q == all_element_names.end();

            if (element_absent)
            {
                break;
            }
        }

        std::shared_ptr<PlotPaneSettings> pp_settings = std::make_shared<PlotPaneSettings>();
        pp_settings->x = 0.0;
        pp_settings->y = 0.0;
        pp_settings->width = 1.0;
        pp_settings->height = 1.0;
        pp_settings->handle_string = element_handle_string;
        pp_settings->title = element_handle_string;

        (*q)->createNewPlotPane(pp_settings);
        notify_main_window_about_modification_();
    }
}

void GuiWindow::createNewPlotPaneCallbackFunction(wxCommandEvent& WXUNUSED(event))
{
    const std::string selected_tab = tab_buttons_.getNameOfSelectedTab();

    auto q = std::find_if(tabs_.begin(), tabs_.end(), [&selected_tab](const WindowTab* const tb) -> bool {
        return selected_tab == tb->getName();
    });

    if (q != tabs_.end())
    {
        wxTextEntryDialog name_dialog(
            this, "Enter the name for the new element", "Enter element name", "<element-name>");

        name_dialog.SetBackgroundColour(
            wxColour(dialog_color_.red * 255.0f, dialog_color_.green * 255.0f, dialog_color_.blue * 255.0f));
        std::string element_handle_string;

        while (true)
        {
            if (name_dialog.ShowModal() == wxID_CANCEL)
            {
                return;
            }

            element_handle_string = name_dialog.GetValue().mb_str();

            if (element_handle_string.length() == 0)
            {
                wxMessageDialog dlg(&name_dialog, "Can't have an empty element name!", "Invalid name!");
                dlg.SetBackgroundColour(
                    wxColour(dialog_color_.red * 255.0f, dialog_color_.green * 255.0f, dialog_color_.blue * 255.0f));
                dlg.ShowModal();
                continue;
            }

            const std::vector<std::string> all_element_names = get_all_element_names_();

            auto q = std::find(all_element_names.begin(), all_element_names.end(), element_handle_string);

            const bool element_exists = q != all_element_names.end();

            if (element_exists)
            {
                wxMessageDialog dlg(
                    &name_dialog, "Choose a unique name", "Element name \"" + element_handle_string + "\" exists!");
                dlg.SetBackgroundColour(
                    wxColour(dialog_color_.red * 255.0f, dialog_color_.green * 255.0f, dialog_color_.blue * 255.0f));
                dlg.ShowModal();
            }
            else
            {
                break;
            }
        }

        (*q)->createNewPlotPane(element_handle_string);
        notify_main_window_about_modification_();
    }
}

std::vector<std::string> GuiWindow::getElementNames() const
{
    std::vector<std::string> element_names;
    for (const auto& t : tabs_)
    {
        const std::vector<std::string> tab_element_names = t->getElementNames();

        element_names.insert(element_names.end(), tab_element_names.begin(), tab_element_names.end());
    }

    return element_names;
}

void GuiWindow::editElementName(wxCommandEvent& WXUNUSED(event))
{
    wxTextEntryDialog name_dialog(this, "Enter the new name for the element", "Edit element name", last_clicked_item_);

    name_dialog.SetBackgroundColour(
        wxColour(dialog_color_.red * 255.0f, dialog_color_.green * 255.0f, dialog_color_.blue * 255.0f));

    std::string new_name;

    while (true)
    {
        if (name_dialog.ShowModal() == wxID_CANCEL)
        {
            return;
        }

        new_name = name_dialog.GetValue().mb_str();

        if (new_name == last_clicked_item_)
        {
            return;
        }
        else
        {
            new_name = name_dialog.GetValue().mb_str();

            if (new_name.length() == 0)
            {
                wxMessageDialog dlg(&name_dialog, "Can't have an empty element name!", "Invalid name!");
                dlg.SetBackgroundColour(
                    wxColour(dialog_color_.red * 255.0f, dialog_color_.green * 255.0f, dialog_color_.blue * 255.0f));
                dlg.ShowModal();
                continue;
            }

            const std::vector<std::string> all_element_names = get_all_element_names_();

            auto q = std::find(all_element_names.begin(), all_element_names.end(), new_name);

            const bool element_exists = q != all_element_names.end();

            if (element_exists)
            {
                wxMessageDialog dlg(&name_dialog, "Choose a unique name", "Element name \"" + new_name + "\" exists!");
                dlg.SetBackgroundColour(
                    wxColour(dialog_color_.red * 255.0f, dialog_color_.green * 255.0f, dialog_color_.blue * 255.0f));
                dlg.ShowModal();
            }
            else
            {
                break;
            }
        }
    }

    bool name_changed = false;
    for (const auto& t : tabs_)
    {
        name_changed = name_changed || t->changeNameOfElementIfElementExists(last_clicked_item_, new_name);
    }

    if (name_changed)
    {
        notify_main_window_element_name_changed_(last_clicked_item_, new_name);
    }
}

void GuiWindow::updateAllElements()
{
    for (auto& t : tabs_)
    {
        t->updateAllElements();
    }
}

void GuiWindow::deleteElement(wxCommandEvent& WXUNUSED(event))
{
    bool element_deleted = false;

    for (const auto& t : tabs_)
    {
        element_deleted = element_deleted || t->deleteElementIfItExists(last_clicked_item_);
    }

    if (element_deleted)
    {
        notify_main_window_about_modification_();
    }
}

void GuiWindow::toggleProjectionMode(wxCommandEvent& WXUNUSED(event))
{
    for (const auto& t : tabs_)
    {
        t->toggleProjectionMode(last_clicked_item_);
    }

    notify_main_window_about_modification_();
}

void GuiWindow::raiseElement(wxCommandEvent& WXUNUSED(event))
{
    for (const auto& t : tabs_)
    {
        t->raiseElement(last_clicked_item_);
    }

    notify_main_window_about_modification_();
}

void GuiWindow::lowerElement(wxCommandEvent& WXUNUSED(event))
{
    for (const auto& t : tabs_)
    {
        t->lowerElement(last_clicked_item_);
    }

    notify_main_window_about_modification_();
}

void GuiWindow::editTabName(wxCommandEvent& WXUNUSED(event))
{
    wxTextEntryDialog name_dialog(this, "Enter the new name for tab", "Enter tab name", last_clicked_item_);

    name_dialog.SetBackgroundColour(
        wxColour(dialog_color_.red * 255.0f, dialog_color_.green * 255.0f, dialog_color_.blue * 255.0f));

    if (name_dialog.ShowModal() == wxID_CANCEL)
    {
        return;
    }

    const std::string tab_name{name_dialog.GetValue().mb_str()};

    if (tab_name.length() == 0)
    {
        return;
    }

    auto q = std::find_if(tabs_.begin(), tabs_.end(), [this](const WindowTab* const tb) -> bool {
        return last_clicked_item_ == tb->getName();
    });

    if (q != tabs_.end())
    {
        (*q)->setName(tab_name);
        notify_main_window_about_modification_();
    }
    tab_buttons_.changeButtonName(last_clicked_item_, tab_name);
}

void GuiWindow::deleteTab(wxCommandEvent& WXUNUSED(event))
{
    auto q = std::find_if(tabs_.begin(), tabs_.end(), [this](const WindowTab* const tb) -> bool {
        return last_clicked_item_ == tb->getName();
    });

    if (q != tabs_.end())
    {
        delete (*q);
        tabs_.erase(q);
    }

    tab_buttons_.deleteTabButton(last_clicked_item_);

    if (tabs_.size() > 1)
    {
        const std::string selected_button = tab_buttons_.getNameOfSelectedTab();
        for (const auto& tab : tabs_)
        {
            if (tab->getName() == selected_button)
            {
                tab->show();
            }
            else
            {
                tab->hide();
            }
        }
    }
    else
    {
        element_x_offset_ = 0;
        tabs_[0]->setMinXPos(element_x_offset_);
        tabs_[0]->show();
    }

    notify_main_window_about_modification_();
}

void GuiWindow::toggleToZoomCallback(wxCommandEvent& WXUNUSED(event))
{
    for (const auto& t : tabs_)
    {
        t->setMouseInteractionType(MouseInteractionType::ZOOM);
    }
}

void GuiWindow::toggleToPanCallback(wxCommandEvent& WXUNUSED(event))
{
    for (const auto& t : tabs_)
    {
        t->setMouseInteractionType(MouseInteractionType::PAN);
    }
}

void GuiWindow::toggleToRotateCallback(wxCommandEvent& WXUNUSED(event))
{
    for (const auto& t : tabs_)
    {
        t->setMouseInteractionType(MouseInteractionType::ROTATE);
    }
}

void GuiWindow::toggleToSelectCallback(wxCommandEvent& WXUNUSED(event))
{
    for (const auto& t : tabs_)
    {
        t->setMouseInteractionType(MouseInteractionType::POINT_SELECTION);
    }
}
