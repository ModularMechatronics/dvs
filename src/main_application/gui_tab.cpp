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

    for (const auto& elem : tab_settings.elements)
    {
        GuiElement* const ge = new PlotPane(parent_window_,
                                            elem,
                                            background_color_,
                                            notify_main_window_key_pressed,
                                            notify_main_window_key_released,
                                            notify_parent_window_right_mouse_pressed,
                                            notify_main_window_about_modification);

        ge->setMinXPos(element_x_offset_);
        ge->updateSizeFromParent(parent_window_->GetSize());
        gui_elements_.push_back(ge);

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
        if (elem.z_order != -1)
        {
            z_order.push_back({elem.z_order, elem.handle_string});
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
    for (const auto& ge : gui_elements_)
    {
        notify_main_window_element_deleted_(ge->getHandleString());
        delete ge;
    }
}

std::vector<GuiElement*> WindowTab::getGuiElements() const
{
    return gui_elements_;
}

void WindowTab::updateAllElements()
{
    for (auto& ge : gui_elements_)
    {
        ge->update();
    }
}

void WindowTab::setMinXPos(const int min_x_pos)
{
    element_x_offset_ = min_x_pos;
    for (auto const& ge : gui_elements_)
    {
        ge->setMinXPos(min_x_pos);
    }
}

void WindowTab::newElement()
{
    ElementSettings elem_settings;
    elem_settings.x = 0.1;
    elem_settings.y = 0;
    elem_settings.width = 0.4;
    elem_settings.height = 0.4;
    elem_settings.handle_string = "element-" + std::to_string(current_element_idx_);
    elem_settings.title = elem_settings.handle_string;

    GuiElement* const ge = new PlotPane(parent_window_,
                                        elem_settings,
                                        background_color_,
                                        notify_main_window_key_pressed_,
                                        notify_main_window_key_released_,
                                        notify_parent_window_right_mouse_pressed_,
                                        notify_main_window_about_modification_);
    ge->setMinXPos(element_x_offset_);
    ge->updateSizeFromParent(parent_window_->GetSize());
    gui_elements_.push_back(ge);
    current_element_idx_++;
}

void WindowTab::newElement(const std::string& element_handle_string)
{
    ElementSettings element_settings;
    element_settings.x = 0.0;
    element_settings.y = 0.0;
    element_settings.width = 0.4;
    element_settings.height = 0.4;
    element_settings.handle_string = element_handle_string;
    element_settings.title = element_handle_string;

    newElement(element_settings);
}

void WindowTab::newElement(const ElementSettings& element_settings)
{
    GuiElement* const ge = new PlotPane(parent_window_,
                                        element_settings,
                                        background_color_,
                                        notify_main_window_key_pressed_,
                                        notify_main_window_key_released_,
                                        notify_parent_window_right_mouse_pressed_,
                                        notify_main_window_about_modification_);
    ge->setMinXPos(element_x_offset_);
    ge->updateSizeFromParent(parent_window_->GetSize());
    gui_elements_.push_back(ge);
    current_element_idx_++;
}

void WindowTab::show()
{
    for (auto const& ge : gui_elements_)
    {
        ge->show();
    }
}

void WindowTab::hide()
{
    for (auto const& ge : gui_elements_)
    {
        ge->hide();
    }
}

void WindowTab::updateSizeFromParent(const wxSize new_size) const
{
    for (auto const& ge : gui_elements_)
    {
        ge->updateSizeFromParent(new_size);
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

    for (const auto& ge : gui_elements_)
    {
        ElementSettings es = ge->getElementSettings();
        const std::string element_handle_string = ge->getHandleString();
        es.z_order = z_order_queue_.getOrderOfElement(element_handle_string);
        ts.elements.push_back(es);
    }

    return ts;
}

GuiElement* WindowTab::getGuiElement(const std::string& element_handle_string) const
{
    auto q = std::find_if(
        gui_elements_.begin(), gui_elements_.end(), [&element_handle_string](const GuiElement* const elem) -> bool {
            return elem->getHandleString() == element_handle_string;
        });

    if (gui_elements_.end() != q)
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
    for (auto& ge : gui_elements_)
    {
        ge->setMouseInteractionType(mit);
    }
}

void WindowTab::notifyChildrenOnKeyPressed(const char key)
{
    for (const auto& ge : gui_elements_)
    {
        ge->keyPressed(key);
    }
}

void WindowTab::notifyChildrenOnKeyReleased(const char key)
{
    for (const auto& ge : gui_elements_)
    {
        ge->keyReleased(key);
    }
}

bool WindowTab::deleteElementIfItExists(const std::string& element_handle_string)
{
    auto q = std::find_if(
        gui_elements_.begin(), gui_elements_.end(), [&element_handle_string](const GuiElement* const elem) -> bool {
            return elem->getHandleString() == element_handle_string;
        });

    if (gui_elements_.end() != q)
    {
        delete (*q);
        z_order_queue_.eraseElement(element_handle_string);
        gui_elements_.erase(q);
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
    auto q = std::find_if(
        gui_elements_.begin(), gui_elements_.end(), [&element_handle_string](const GuiElement* const elem) -> bool {
            return elem->getHandleString() == element_handle_string;
        });

    if (gui_elements_.end() != q)
    {
        (*q)->toggleProjectionMode();
    }
}

bool WindowTab::elementWithNameExists(const std::string& element_handle_string)
{
    auto q = std::find_if(
        gui_elements_.begin(), gui_elements_.end(), [&element_handle_string](const GuiElement* const& elem) -> bool {
            return elem->getHandleString() == element_handle_string;
        });

    if (gui_elements_.end() != q)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool WindowTab::changeNameOfElementIfElementExists(const std::string& element_handle_string,
                                                   const std::string& new_name)
{
    auto q = std::find_if(
        gui_elements_.begin(), gui_elements_.end(), [&element_handle_string](const GuiElement* const elem) -> bool {
            return elem->getHandleString() == element_handle_string;
        });

    if (gui_elements_.end() != q)
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
    auto q = std::find_if(
        gui_elements_.begin(), gui_elements_.end(), [&element_handle_string](const GuiElement* const elem) -> bool {
            return elem->getHandleString() == element_handle_string;
        });

    if (gui_elements_.end() != q)
    {
        (*q)->raise();
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
    auto q = std::find_if(
        gui_elements_.begin(), gui_elements_.end(), [&element_handle_string](const GuiElement* const elem) -> bool {
            return elem->getHandleString() == element_handle_string;
        });

    if (gui_elements_.end() != q)
    {
        (*q)->lower();
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

    for (const auto& ge : gui_elements_)
    {
        element_names.push_back(ge->getHandleString());
    }

    return element_names;
}

void WindowTab::setName(const std::string& new_name)
{
    name_ = new_name;
}
