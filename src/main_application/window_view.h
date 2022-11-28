#ifndef MAIN_APPLICATION_WINDOW_VIEW_H_
#define MAIN_APPLICATION_WINDOW_VIEW_H_

#include <wx/button.h>
#include <wx/frame.h>
#include <wx/menu.h>
#include <wx/notebook.h>
#include <wx/textctrl.h>
#include <wx/timer.h>
#include <wx/wx.h>

#include <map>
#include <mutex>
#include <string>
#include <thread>
#include <tuple>
#include <utility>
#include <vector>

#include "axes/axes.h"
#include "gui_element.h"
#include "help_pane.h"
#include "plot_pane.h"
#include "project_state/project_settings.h"
#include "tab_buttons.h"

class ZOrderQueue
{
private:
    std::vector<std::string> elements_;
    // [element{0}, element{1}, ..., element{n-1}]
    // First element in vector (element{n-1}) is the
    // element with the highest Z order (in front)

    bool elementExistsInQueue(const std::string& element_name) const
    {
        const auto q = std::find(elements_.begin(), elements_.end(), element_name);

        return q != elements_.end();
    }

public:
    ZOrderQueue() {}

    int getOrderOfElement(const std::string& element_name) const
    {
        if (elementExistsInQueue(element_name))
        {
            // 0 is "lowest"
            // {n-1} is "at the top"
            const auto q = find(elements_.begin(), elements_.end(), element_name);
            const int index = q - elements_.begin();
            return index;
        }
        else
        {
            return -1;
        }
    }

    void raise(const std::string& element_name)
    {
        eraseElement(element_name);
        elements_.push_back(element_name);
    }

    void lower(const std::string& element_name)
    {
        eraseElement(element_name);
        elements_.insert(elements_.begin(), element_name);
    }

    void eraseElement(const std::string& element_name)
    {
        const auto q = std::find(elements_.begin(), elements_.end(), element_name);

        if (q != elements_.end())
        {
            elements_.erase(q);
        }
    }
};

class WindowTab
{
private:
    std::string name_;
    std::vector<GuiElement*> gui_elements_;
    wxFrame* parent_window_;
    std::function<void(const char key)> notify_main_window_key_pressed_;
    std::function<void(const char key)> notify_main_window_key_released_;
    std::function<void(const wxPoint pos, const std::string& elem_name)> notify_parent_window_right_mouse_pressed_;
    std::function<void(const GuiElement* const)> notify_main_window_element_deleted_;
    int current_element_idx_;
    RGBTripletf background_color_;
    RGBTripletf button_normal_color_;
    RGBTripletf button_clicked_color_;
    RGBTripletf button_selected_color_;
    RGBTripletf button_text_color_;
    ZOrderQueue z_order_queue_;

public:
    WindowTab(wxFrame* parent_window,
              const TabSettings& tab_settings,
              const std::function<void(const char key)>& notify_main_window_key_pressed,
              const std::function<void(const char key)>& notify_main_window_key_released,
              const std::function<void(const wxPoint pos, const std::string& elem_name)>&
                  notify_parent_window_right_mouse_pressed,
              const std::function<void(const GuiElement* const)>& notify_main_window_element_deleted)
        : name_{tab_settings.name},
          notify_main_window_key_pressed_{notify_main_window_key_pressed},
          notify_main_window_key_released_{notify_main_window_key_released},
          notify_parent_window_right_mouse_pressed_{notify_parent_window_right_mouse_pressed},
          notify_main_window_element_deleted_{notify_main_window_element_deleted}
    {
        parent_window_ = parent_window;
        current_element_idx_ = 0;

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
                                                notify_main_window_element_deleted_);

            ge->updateSizeFromParent(parent_window_->GetSize());
            gui_elements_.push_back(ge);

            current_element_idx_++;
        }

        initializeZOrder(tab_settings);
    }

    void initializeZOrder(const TabSettings& tab_settings)
    {
        struct ZOrderPair
        {
            int order;
            std::string name;
        };

        std::vector<ZOrderPair> z_order;

        for (const auto& elem : tab_settings.elements)
        {
            if (elem.z_order != -1)
            {
                z_order.push_back({elem.z_order, elem.name});
            }
        }

        // Sort the list so that the element which is at the bottom (lowest
        // z order number) ends up first in the list
        std::sort(z_order.begin(), z_order.end(), [](const ZOrderPair& p0, const ZOrderPair& p1) -> bool {
            return p0.order < p1.order;
        });

        for (size_t k = 0; k < z_order.size(); k++)
        {
            raiseElement(z_order[k].name);
        }
    }

    ~WindowTab()
    {
        for (auto const& ge : gui_elements_)
        {
            delete ge;
        }
    }

    void setMinXPos(const int min_x_pos)
    {
        for (auto const& ge : gui_elements_)
        {
            ge->setMinXPos(min_x_pos);
        }
    }

    void newElement()
    {
        ElementSettings elem_settings;
        elem_settings.x = 0.1;
        elem_settings.y = 0;
        elem_settings.width = 0.4;
        elem_settings.height = 0.4;
        elem_settings.name = "element-" + std::to_string(current_element_idx_);

        GuiElement* const ge = new PlotPane(parent_window_,
                                            elem_settings,
                                            background_color_,
                                            notify_main_window_key_pressed_,
                                            notify_main_window_key_released_,
                                            notify_parent_window_right_mouse_pressed_,
                                            notify_main_window_element_deleted_);

        ge->updateSizeFromParent(parent_window_->GetSize());
        gui_elements_.push_back(ge);
        current_element_idx_++;
    }

    void newElement(const std::string& element_name)
    {
        ElementSettings elem_settings;
        elem_settings.x = 0.1;
        elem_settings.y = 0;
        elem_settings.width = 0.4;
        elem_settings.height = 0.4;
        elem_settings.name = element_name;

        GuiElement* const ge = new PlotPane(parent_window_,
                                            elem_settings,
                                            background_color_,
                                            notify_main_window_key_pressed_,
                                            notify_main_window_key_released_,
                                            notify_parent_window_right_mouse_pressed_,
                                            notify_main_window_element_deleted_);

        ge->updateSizeFromParent(parent_window_->GetSize());
        gui_elements_.push_back(ge);
        current_element_idx_++;
    }

    void show()
    {
        for (auto const& ge : gui_elements_)
        {
            ge->show();
        }
        layoutZOrder();
    }

    void hide()
    {
        for (auto const& ge : gui_elements_)
        {
            ge->hide();
        }
    }

    void updateSizeFromParent(const wxSize new_size) const
    {
        for (auto const& ge : gui_elements_)
        {
            ge->updateSizeFromParent(new_size);
        }
    }

    RGBTripletf getBackgroundColor() const
    {
        return background_color_;
    }

    std::string getName() const
    {
        return name_;
    }

    TabSettings getTabSettings() const
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
            const std::string element_name = ge->getName();
            es.z_order = z_order_queue_.getOrderOfElement(element_name);
            ts.elements.push_back(es);
        }

        return ts;
    }

    void layoutZOrder() {}

    GuiElement* getGuiElement(const std::string& element_name) const
    {
        auto q = std::find_if(
            gui_elements_.begin(), gui_elements_.end(), [&element_name](const GuiElement* const elem) -> bool {
                return elem->getName() == element_name;
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

    void notifyChildrenOnKeyPressed(const char key)
    {
        for (const auto& ge : gui_elements_)
        {
            ge->keyPressed(key);
        }
    }

    void notifyChildrenOnKeyReleased(const char key)
    {
        for (const auto& ge : gui_elements_)
        {
            ge->keyReleased(key);
        }
    }

    void deleteElementIfItExists(const std::string& element_name)
    {
        auto q = std::find_if(
            gui_elements_.begin(), gui_elements_.end(), [&element_name](const GuiElement* const elem) -> bool {
                return elem->getName() == element_name;
            });

        if (gui_elements_.end() != q)
        {
            delete (*q);
            z_order_queue_.eraseElement(element_name);
            gui_elements_.erase(q);
        }
    }

    bool elementWithNameExists(const std::string& element_name)
    {
        auto q = std::find_if(
            gui_elements_.begin(), gui_elements_.end(), [&element_name](const GuiElement* const& elem) -> bool {
                return elem->getName() == element_name;
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

    bool changeNameOfElementIfElementExists(const std::string& element_name, const std::string& new_name)
    {
        auto q = std::find_if(
            gui_elements_.begin(), gui_elements_.end(), [&element_name](const GuiElement* const elem) -> bool {
                return elem->getName() == element_name;
            });

        if (gui_elements_.end() != q)
        {
            (*q)->setName(new_name);
            return true;
        }
        else
        {
            return false;
        }
    }

    bool raiseElement(const std::string& element_name)
    {
        auto q = std::find_if(
            gui_elements_.begin(), gui_elements_.end(), [&element_name](const GuiElement* const elem) -> bool {
                return elem->getName() == element_name;
            });

        if (gui_elements_.end() != q)
        {
            (*q)->raise();
            z_order_queue_.raise(element_name);
            return true;
        }
        else
        {
            return false;
        }
    }

    bool lowerElement(const std::string& element_name)
    {
        auto q = std::find_if(
            gui_elements_.begin(), gui_elements_.end(), [&element_name](const GuiElement* const elem) -> bool {
                return elem->getName() == element_name;
            });

        if (gui_elements_.end() != q)
        {
            (*q)->lower();
            z_order_queue_.lower(element_name);
            return true;
        }
        else
        {
            return false;
        }
    }

    std::vector<std::string> getElementNames() const
    {
        std::vector<std::string> element_names;

        for (const auto& ge : gui_elements_)
        {
            element_names.push_back(ge->getName());
        }

        return element_names;
    }

    void setName(const std::string& new_name)
    {
        name_ = new_name;
    }
};

class WindowView : public wxFrame
{
private:
    enum ClickSource
    {
        TAB_BUTTON,
        GUI_ELEMENT,
        THIS
    };

    TabButtons tab_buttons_;
    std::vector<WindowTab*> tabs_;
    int element_x_offset_;
    int callback_id_;
    AxesSettings axes_settings_{};
    void tabChanged(const std::string name);
    std::function<void(const char key)> notify_main_window_key_pressed_;
    std::function<void(const char key)> notify_main_window_key_released_;
    std::function<std::vector<std::string>(void)> get_all_element_names_;
    std::function<void(const GuiElement* const)> notify_main_window_element_deleted_;

    std::function<void()> notify_main_window_new_window_;
    std::string name_;
    int current_unnamed_idx_;
    std::string name_of_selected_element_;
    WindowSettings* settings_;
    wxFrame* main_window_;

    wxMenu* popup_menu_window_;
    wxMenu* popup_menu_element_;
    wxMenu* popup_menu_tab_;

    int current_tab_num_;

    std::string last_clicked_item_;

    HelpPane help_pane_;

    RGBTripletf dialog_color_;

public:
    WindowView() = delete;
    WindowView(wxFrame* main_window,
               const WindowSettings& window_settings,
               const int callback_id,
               const std::function<void(const char key)>& notify_main_window_key_pressed,
               const std::function<void(const char key)>& notify_main_window_key_released,
               const std::function<std::vector<std::string>(void)>& get_all_element_names,
               const std::function<void(const GuiElement* const)>& notify_main_window_element_deleted);
    ~WindowView();
    int getCallbackId() const;
    void OnSize(wxSizeEvent& event);
    void keyPressedCallback(wxKeyEvent& evt);
    void keyReleasedCallback(wxKeyEvent& evt);

    void setName(const std::string& new_name);
    WindowSettings getWindowSettings() const;
    std::string getName() const;
    void childModified(wxCommandEvent& event);

    GuiElement* getGuiElement(const std::string& element_name) const;

    void mouseRightPressed(const wxPoint pos, const ClickSource source, const std::string& item_name);

    void mouseRightPressedCallback(wxMouseEvent& event);
    void notifyChildrenOnKeyPressed(const char key);
    void notifyChildrenOnKeyReleased(const char key);

    void editWindowName(wxCommandEvent& WXUNUSED(event));

    void newTab(wxCommandEvent& WXUNUSED(event));
    void newElement(wxCommandEvent& WXUNUSED(event));

    void editElementName(wxCommandEvent& WXUNUSED(event));
    void deleteElement(wxCommandEvent& WXUNUSED(event));

    void raiseElement(wxCommandEvent& WXUNUSED(event));
    void lowerElement(wxCommandEvent& WXUNUSED(event));

    void editTabName(wxCommandEvent& WXUNUSED(event));
    void deleteTab(wxCommandEvent& WXUNUSED(event));

    virtual void OnClose(wxCloseEvent& event);

    std::vector<std::string> getElementNames() const;
};

#endif  // MAIN_APPLICATION_WINDOW_VIEW_H_
