#include "gui_tab.h"

// #################################################################
// ########################## ZOrderQueue ##########################
// #################################################################

bool ZOrderQueue::elementExistsInQueue(const std::string& element_handle_string) const
{
    const auto q = std::find(elements_.begin(), elements_.end(), element_handle_string);

    return q != elements_.end();
}

int ZOrderQueue::getOrderOfElement(const std::string& element_handle_string) const
{
    if (elementExistsInQueue(element_handle_string))
    {
        // 0 is "lowest"
        // {n-1} is "at the top"
        const auto q = find(elements_.begin(), elements_.end(), element_handle_string);
        const int index = q - elements_.begin();
        return index;
    }
    else
    {
        return -1;
    }
}

void ZOrderQueue::raise(const std::string& element_handle_string)
{
    eraseElement(element_handle_string);
    elements_.push_back(element_handle_string);
}

void ZOrderQueue::lower(const std::string& element_handle_string)
{
    eraseElement(element_handle_string);
    elements_.insert(elements_.begin(), element_handle_string);
}

void ZOrderQueue::eraseElement(const std::string& element_handle_string)
{
    const auto q = std::find(elements_.begin(), elements_.end(), element_handle_string);

    if (q != elements_.end())
    {
        elements_.erase(q);
    }
}

std::tuple<wxPoint, wxSize> getPosAndSizeInPixelCoords(const wxSize current_window_size,
                                                       const ElementSettings* const element_settings)
{
    const float x_pos_in_pixels = element_settings->x * current_window_size.GetWidth();
    const float y_pos_in_pixels = element_settings->y * current_window_size.GetHeight();

    const float width_in_pixels = element_settings->width * current_window_size.GetWidth();
    const float height_in_pixels = element_settings->height * current_window_size.GetHeight();

    return std::make_tuple(wxPoint(x_pos_in_pixels, y_pos_in_pixels), wxSize(width_in_pixels, height_in_pixels));
}

// ###############################################################
// ########################## WindowTab ##########################
// ###############################################################

WindowTab::WindowTab(wxFrame* parent_window,
                     const TabSettings& tab_settings,
                     const int element_x_offset,
                     const std::function<void(const char key)>& notify_main_window_key_pressed,
                     const std::function<void(const char key)>& notify_main_window_key_released,
                     const std::function<void(const wxPoint pos, const std::string& elem_name)>&
                         notify_parent_window_right_mouse_pressed,
                     const std::function<void(const std::string&)>& notify_main_window_element_deleted,
                     const std::function<void()>& notify_main_window_about_modification)
    : name_{tab_settings.name},
      notify_main_window_key_pressed_{notify_main_window_key_pressed},
      notify_main_window_key_released_{notify_main_window_key_released},
      notify_parent_window_right_mouse_pressed_{notify_parent_window_right_mouse_pressed},
      notify_main_window_element_deleted_{notify_main_window_element_deleted},
      notify_main_window_about_modification_{notify_main_window_about_modification}
{
    parent_window_ = parent_window;
    current_element_idx_ = 0;
    element_x_offset_ = element_x_offset;

    background_color_ = tab_settings.background_color;
    button_normal_color_ = tab_settings.button_normal_color;
    button_clicked_color_ = tab_settings.button_clicked_color;
    button_selected_color_ = tab_settings.button_selected_color;
    button_text_color_ = tab_settings.button_text_color;

    for (const std::shared_ptr<ElementSettings> elem_settings : tab_settings.elements)
    {
        if (elem_settings->type == dvs::GuiElementType::PlotPane)
        {
            PlotPane* const pp = new PlotPane(parent_window_,
                                              elem_settings,
                                              background_color_,
                                              notify_main_window_key_pressed,
                                              notify_main_window_key_released,
                                              notify_parent_window_right_mouse_pressed,
                                              notify_main_window_about_modification);

            pp->setMinXPos(element_x_offset_);
            pp->updateSizeFromParent(parent_window_->GetSize());

            plot_panes_.push_back(pp);
        }
        else if (elem_settings->type == dvs::GuiElementType::Button)
        {
            auto const [elem_pos, elem_size] =
                getPosAndSizeInPixelCoords(parent_window_->GetSize(), elem_settings.get());

            ButtonGuiElement* button_ = new ButtonGuiElement(parent_window_,
                                                             elem_settings,
                                                             notify_main_window_key_pressed,
                                                             notify_main_window_key_released,
                                                             notify_parent_window_right_mouse_pressed,
                                                             notify_main_window_about_modification,
                                                             elem_pos,
                                                             elem_size);

            button_->setMinXPos(element_x_offset_);
            button_->updateSizeFromParent(parent_window_->GetSize());
            gui_elements_.push_back(button_);
        }
        else if (elem_settings->type == dvs::GuiElementType::Slider)
        {
            auto const [elem_pos, elem_size] =
                getPosAndSizeInPixelCoords(parent_window_->GetSize(), elem_settings.get());

            SliderGuiElement* slider_ = new SliderGuiElement(parent_window_,
                                                             elem_settings,
                                                             notify_main_window_key_pressed,
                                                             notify_main_window_key_released,
                                                             notify_parent_window_right_mouse_pressed,
                                                             notify_main_window_about_modification,
                                                             elem_pos,
                                                             elem_size);

            slider_->setMinXPos(element_x_offset_);
            slider_->updateSizeFromParent(parent_window_->GetSize());
            gui_elements_.push_back(slider_);
        }
        else if (elem_settings->type == dvs::GuiElementType::CheckBox)
        {
            auto const [elem_pos, elem_size] =
                getPosAndSizeInPixelCoords(parent_window_->GetSize(), elem_settings.get());

            CheckboxGuiElement* check_box_ = new CheckboxGuiElement(parent_window_,
                                                                    elem_settings,
                                                                    notify_main_window_key_pressed,
                                                                    notify_main_window_key_released,
                                                                    notify_parent_window_right_mouse_pressed,
                                                                    notify_main_window_about_modification,
                                                                    elem_pos,
                                                                    elem_size);

            check_box_->setMinXPos(element_x_offset_);
            check_box_->updateSizeFromParent(parent_window_->GetSize());
            gui_elements_.push_back(check_box_);
        }

        current_element_idx_++;
    }

    initializeZOrder(tab_settings);
}

void WindowTab::initializeZOrder(const TabSettings& tab_settings)
{
    struct ZOrderPair
    {
        int order;
        std::string handle_string;
    };

    std::vector<ZOrderPair> z_order;

    for (const auto& elem : tab_settings.elements)
    {
        if (elem->z_order != -1)
        {
            z_order.push_back({elem->z_order, elem->handle_string});
        }
    }

    // Sort the list so that the element which is at the bottom (lowest
    // z order number) ends up first in the list
    std::sort(z_order.begin(), z_order.end(), [](const ZOrderPair& p0, const ZOrderPair& p1) -> bool {
        return p0.order < p1.order;
    });

    for (size_t k = 0; k < z_order.size(); k++)
    {
        raiseElement(z_order[k].handle_string);
    }
}

WindowTab::~WindowTab()
{
    for (const auto& pp : plot_panes_)
    {
        notify_main_window_element_deleted_(pp->getHandleString());
        delete pp;
    }

    for (const auto& elem : gui_elements_)
    {
        notify_main_window_element_deleted_(elem->getHandleString());
        delete elem;
    }
}

std::vector<ApplicationGuiElement*> WindowTab::getGuiElements() const
{
    return gui_elements_;
}

std::vector<ApplicationGuiElement*> WindowTab::getPlotPanes() const
{
    std::vector<ApplicationGuiElement*> gui_elements;

    for (const auto& pp : plot_panes_)
    {
        gui_elements.push_back(pp);
    }

    return gui_elements;
}

std::vector<ApplicationGuiElement*> WindowTab::getAllGuiElements() const
{
    std::vector<ApplicationGuiElement*> gui_elements = gui_elements_;

    for (const auto& pp : plot_panes_)
    {
        gui_elements.push_back(pp);
    }

    return gui_elements;
}

void WindowTab::updateAllElements()
{
    for (auto& pp : plot_panes_)
    {
        pp->update();
    }
}

void WindowTab::setMinXPos(const int min_x_pos)
{
    element_x_offset_ = min_x_pos;
    for (auto const& pp : plot_panes_)
    {
        pp->setMinXPos(min_x_pos);
    }
}

void WindowTab::createNewPlotPane()
{
    std::shared_ptr<PlotPaneSettings> pp_settings = std::make_shared<PlotPaneSettings>();
    pp_settings->x = 0.1;
    pp_settings->y = 0;
    pp_settings->width = 0.4;
    pp_settings->height = 0.4;
    pp_settings->handle_string = "element-" + std::to_string(current_element_idx_);
    pp_settings->title = pp_settings->handle_string;

    PlotPane* const pp = new PlotPane(parent_window_,
                                      pp_settings,
                                      background_color_,
                                      notify_main_window_key_pressed_,
                                      notify_main_window_key_released_,
                                      notify_parent_window_right_mouse_pressed_,
                                      notify_main_window_about_modification_);
    pp->setMinXPos(element_x_offset_);
    pp->updateSizeFromParent(parent_window_->GetSize());
    plot_panes_.push_back(pp);
    current_element_idx_++;
}

void WindowTab::createNewPlotPane(const std::string& element_handle_string)
{
    std::shared_ptr<PlotPaneSettings> pp_settings = std::make_shared<PlotPaneSettings>();
    pp_settings->x = 0.0;
    pp_settings->y = 0.0;
    pp_settings->width = 0.4;
    pp_settings->height = 0.4;
    pp_settings->handle_string = element_handle_string;
    pp_settings->title = element_handle_string;

    createNewPlotPane(pp_settings);
}

void WindowTab::createNewPlotPane(const std::shared_ptr<ElementSettings>& element_settings)
{
    PlotPane* const pp = new PlotPane(parent_window_,
                                      element_settings,
                                      background_color_,
                                      notify_main_window_key_pressed_,
                                      notify_main_window_key_released_,
                                      notify_parent_window_right_mouse_pressed_,
                                      notify_main_window_about_modification_);
    pp->setMinXPos(element_x_offset_);
    pp->updateSizeFromParent(parent_window_->GetSize());
    plot_panes_.push_back(pp);
    current_element_idx_++;
}

void WindowTab::show()
{
    for (auto const& pp : plot_panes_)
    {
        pp->Show();
    }
}

void WindowTab::hide()
{
    for (auto const& pp : plot_panes_)
    {
        pp->Hide();
    }
}

void WindowTab::updateSizeFromParent(const wxSize new_size) const
{
    for (auto const& pp : plot_panes_)
    {
        pp->updateSizeFromParent(new_size);
    }

    for (auto const& elem : gui_elements_)
    {
        elem->updateSizeFromParent(new_size);
    }
}

RGBTripletf WindowTab::getBackgroundColor() const
{
    return background_color_;
}

std::string WindowTab::getName() const
{
    return name_;
}

TabSettings WindowTab::getTabSettings() const
{
    TabSettings ts;

    ts.name = name_;
    ts.background_color = background_color_;
    ts.button_normal_color = button_normal_color_;
    ts.button_clicked_color = button_clicked_color_;
    ts.button_selected_color = button_selected_color_;
    ts.button_text_color = button_text_color_;

    for (const auto& pp : plot_panes_)
    {
        std::shared_ptr<ElementSettings> es = pp->getElementSettings();
        const std::string element_handle_string = pp->getHandleString();
        es->z_order = z_order_queue_.getOrderOfElement(element_handle_string);
        ts.elements.push_back(es);
    }

    for (const auto& ge : gui_elements_)
    {
        std::shared_ptr<ElementSettings> es = ge->getElementSettings();
        const std::string element_handle_string = ge->getHandleString();
        es->z_order = z_order_queue_.getOrderOfElement(element_handle_string);
        ts.elements.push_back(es);
    }

    return ts;
}

ApplicationGuiElement* WindowTab::getGuiElement(const std::string& element_handle_string) const
{
    auto q = std::find_if(plot_panes_.begin(),
                          plot_panes_.end(),
                          [&element_handle_string](const ApplicationGuiElement* const elem) -> bool {
                              return elem->getHandleString() == element_handle_string;
                          });

    if (plot_panes_.end() != q)
    {
        return (*q);
    }
    else
    {
        return nullptr;
    }
}

void WindowTab::setMouseInteractionType(const MouseInteractionType mit)
{
    for (auto& pp : plot_panes_)
    {
        pp->setMouseInteractionType(mit);
    }
}

void WindowTab::notifyChildrenOnKeyPressed(const char key)
{
    for (const auto& pp : plot_panes_)
    {
        pp->keyPressed(key);
    }

    for (const auto& ge : gui_elements_)
    {
        ge->keyPressed(key);
    }
}

void WindowTab::notifyChildrenOnKeyReleased(const char key)
{
    for (const auto& pp : plot_panes_)
    {
        pp->keyReleased(key);
    }

    for (const auto& ge : gui_elements_)
    {
        ge->keyReleased(key);
    }
}

bool WindowTab::deleteElementIfItExists(const std::string& element_handle_string)
{
    auto q = std::find_if(plot_panes_.begin(),
                          plot_panes_.end(),
                          [&element_handle_string](const ApplicationGuiElement* const elem) -> bool {
                              return elem->getHandleString() == element_handle_string;
                          });

    if (plot_panes_.end() != q)
    {
        delete (*q);
        z_order_queue_.eraseElement(element_handle_string);
        plot_panes_.erase(q);
        notify_main_window_element_deleted_(element_handle_string);
        return true;
    }
    else
    {
        return false;
    }
}

void WindowTab::toggleProjectionMode(const std::string& element_handle_string)
{
    auto q = std::find_if(plot_panes_.begin(),
                          plot_panes_.end(),
                          [&element_handle_string](const ApplicationGuiElement* const elem) -> bool {
                              return elem->getHandleString() == element_handle_string;
                          });

    if (plot_panes_.end() != q)
    {
        (*q)->toggleProjectionMode();
    }
}

bool WindowTab::elementWithNameExists(const std::string& element_handle_string)
{
    auto q = std::find_if(plot_panes_.begin(),
                          plot_panes_.end(),
                          [&element_handle_string](const ApplicationGuiElement* const& elem) -> bool {
                              return elem->getHandleString() == element_handle_string;
                          });

    return plot_panes_.end() != q;
}

bool WindowTab::changeNameOfElementIfElementExists(const std::string& element_handle_string,
                                                   const std::string& new_name)
{
    auto q = std::find_if(plot_panes_.begin(),
                          plot_panes_.end(),
                          [&element_handle_string](const ApplicationGuiElement* const elem) -> bool {
                              return elem->getHandleString() == element_handle_string;
                          });

    if (plot_panes_.end() != q)
    {
        (*q)->setHandleString(new_name);
        return true;
    }
    else
    {
        return false;
    }
}

bool WindowTab::raiseElement(const std::string& element_handle_string)
{
    auto q = std::find_if(plot_panes_.begin(),
                          plot_panes_.end(),
                          [&element_handle_string](const ApplicationGuiElement* const elem) -> bool {
                              return elem->getHandleString() == element_handle_string;
                          });

    if (plot_panes_.end() != q)
    {
        (*q)->Raise();
        z_order_queue_.raise(element_handle_string);
        return true;
    }
    else
    {
        return false;
    }
}

bool WindowTab::lowerElement(const std::string& element_handle_string)
{
    auto q = std::find_if(plot_panes_.begin(),
                          plot_panes_.end(),
                          [&element_handle_string](const ApplicationGuiElement* const elem) -> bool {
                              return elem->getHandleString() == element_handle_string;
                          });

    if (plot_panes_.end() != q)
    {
        (*q)->Lower();
        z_order_queue_.lower(element_handle_string);
        return true;
    }
    else
    {
        return false;
    }
}

std::vector<std::string> WindowTab::getElementNames() const
{
    std::vector<std::string> element_names;

    for (const auto& pp : plot_panes_)
    {
        element_names.push_back(pp->getHandleString());
    }

    return element_names;
}

void WindowTab::setName(const std::string& new_name)
{
    name_ = new_name;
}
