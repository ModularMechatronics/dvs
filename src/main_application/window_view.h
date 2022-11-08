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
#include "plot_pane.h"
#include "project_state/project_settings.h"
#include "tab_buttons.h"

class WindowTab
{
private:
    std::string name_;
    std::vector<GuiElement*> gui_elements_;
    wxFrame* parent_window_;
    std::function<void(const char key)> notify_main_window_key_pressed_;
    std::function<void(const char key)> notify_main_window_key_released_;
    std::function<void(const wxPoint pos, const std::string& elem_name)> notify_parent_window_right_mouse_pressed_;
    int current_element_idx_;

public:
    WindowTab(wxFrame* parent_window,
              const TabSettings& tab_settings,
              const std::function<void(const char key)>& notify_main_window_key_pressed,
              const std::function<void(const char key)>& notify_main_window_key_released,
              const std::function<void(const wxPoint pos, const std::string& elem_name)>&
                  notify_parent_window_right_mouse_pressed)
        : name_{tab_settings.name},
          notify_main_window_key_pressed_{notify_main_window_key_pressed},
          notify_main_window_key_released_{notify_main_window_key_released},
          notify_parent_window_right_mouse_pressed_{notify_parent_window_right_mouse_pressed}
    {
        parent_window_ = parent_window;
        const float grid_size_ = 1.0f;
        current_element_idx_ = 0;

        for (const auto& elem : tab_settings.elements)
        {
            GuiElement* const ge = new PlotPane(parent_window_,
                                                elem,
                                                grid_size_,
                                                notify_main_window_key_pressed,
                                                notify_main_window_key_released,
                                                notify_parent_window_right_mouse_pressed);

            ge->updateSizeFromParent(parent_window_->GetSize());
            gui_elements_.push_back(ge);

            current_element_idx_++;
        }
    }

    ~WindowTab()
    {
        for (auto const& ge : gui_elements_)
        {
            delete ge;
        }
    }

    void newElement()
    {
        const float grid_size_ = 1.0f;

        ElementSettings elem_settings;
        elem_settings.x = 0.1;
        elem_settings.y = 0;
        elem_settings.width = 0.4;
        elem_settings.height = 0.4;
        elem_settings.name = "element-" + std::to_string(current_element_idx_);

        GuiElement* const ge = new PlotPane(parent_window_,
                                            elem_settings,
                                            grid_size_,
                                            notify_main_window_key_pressed_,
                                            notify_main_window_key_released_,
                                            notify_parent_window_right_mouse_pressed_);

        ge->updateSizeFromParent(parent_window_->GetSize());
        gui_elements_.push_back(ge);
        current_element_idx_++;
    }

    void newElement(const std::string& element_name)
    {
        const float grid_size_ = 1.0f;

        ElementSettings elem_settings;
        elem_settings.x = 0.1;
        elem_settings.y = 0;
        elem_settings.width = 0.4;
        elem_settings.height = 0.4;
        elem_settings.name = element_name;

        GuiElement* const ge = new PlotPane(parent_window_,
                                            elem_settings,
                                            grid_size_,
                                            notify_main_window_key_pressed_,
                                            notify_main_window_key_released_,
                                            notify_parent_window_right_mouse_pressed_);

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

    std::string getName() const
    {
        return name_;
    }

    TabSettings getTabSettings() const
    {
        TabSettings ts;

        ts.name = name_;

        for (const auto& ge : gui_elements_)
        {
            ts.elements.push_back(ge->getElementSettings());
        }

        return ts;
    }

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

    std::vector<std::string> getElementNames() const
    {
        std::vector<std::string> element_names;

        for (const auto& ge : gui_elements_)
        {
            element_names.push_back(ge->getName());
        }

        return element_names;
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
    std::map<std::string, WindowTab*> tabs_;
    int callback_id_;
    AxesSettings axes_settings_{};
    void tabChanged(const std::string name);
    std::function<void(const char key)> notify_main_window_key_pressed_;
    std::function<void(const char key)> notify_main_window_key_released_;
    std::function<std::vector<std::string>(void)> get_all_element_names_;

    std::function<void()> notify_main_window_new_window_;
    float grid_size_;
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

public:
    WindowView() = delete;
    WindowView(wxFrame* main_window,
               const WindowSettings& window_settings,
               const int callback_id,
               const std::function<void(const char key)>& notify_main_window_key_pressed,
               const std::function<void(const char key)>& notify_main_window_key_released,
               const std::function<std::vector<std::string>(void)>& get_all_element_names);
    ~WindowView();
    // void newElement(const std::string& element_name);
    // void newElement();
    int getCallbackId() const;
    void OnSize(wxSizeEvent& event);

    void onActivate(wxActivateEvent& event);
    void hide();
    void show();
    void setName(const std::string& new_name);
    WindowSettings getWindowSettings() const;
    std::string getName() const;
    void childModified(wxCommandEvent& event);

    void setSelectedElementName(const std::string& new_name);
    void mouseLeftPressed(wxMouseEvent& WXUNUSED(event));
    void setFirstElementSelected();

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

    void editTabName(wxCommandEvent& WXUNUSED(event));
    void deleteTab(wxCommandEvent& WXUNUSED(event));

    virtual void OnClose(wxCloseEvent& event);

    std::vector<std::string> getElementNames() const;
};

#endif  // MAIN_APPLICATION_WINDOW_VIEW_H_
