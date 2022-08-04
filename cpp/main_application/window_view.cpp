#include "window_view.h"

#include "events.h"
#include "globals.h"
#include "plot_window_gl_pane.h"

WindowView::WindowView(wxFrame* parent,
    const WindowSettings& window_settings,
    const int callback_id,
    const std::function<void(const char key)>& notify_main_window_key_pressed,
    const std::function<void(const char key)>& notify_main_window_key_released)
    : ViewBase<wxFrame>(parent,
        window_settings,
        notify_main_window_key_pressed,
        notify_main_window_key_released)
{
    callback_id_ = callback_id;

    const std::vector<ElementSettings> elements = settings_->getElementSettingsList();

    for (const auto& elem : elements)
    {
        GuiElement* const ge = new PlotWindowGLPane(dynamic_cast<wxFrame*>(this),
            elem,
            grid_size_,
            notify_main_window_key_pressed_,
            notify_main_window_key_released_);

        ge->updateSizeFromParent(this->GetSize());
        gui_elements_[elem.name] = ge;
    }

    this->SetLabel(window_settings.getName());

    if (gui_elements_.size() > 0)
    {
        gui_elements_.begin()->second->setIsSelected();
    }

    Bind(wxEVT_CLOSE_WINDOW, &WindowView::OnClose, this);
    Bind(wxEVT_SIZE, &WindowView::OnSize, this);

    show();
}

void WindowView::OnSize(wxSizeEvent& event)
{
    wxFrame::OnSize(event);
    const wxSize new_size = event.GetSize();

    for (auto it : gui_elements_)
    {
        it.second->updateSizeFromParent(new_size);
    }
}

int WindowView::getCallbackId() const
{
    return callback_id_;
}

void WindowView::setName(const std::string& new_name)
{
    this->SetLabel(new_name);
    name_ = new_name;
}

void WindowView::onActivate(wxActivateEvent& event)
{
    if (event.GetActive())
    {
        wxCommandEvent parent_event(CHILD_WINDOW_IN_FOCUS_EVENT);
        parent_event.SetId(callback_id_);
        wxPostEvent(GetParent(), parent_event);
    }
}

void WindowView::hide()
{
    Unbind(wxEVT_ACTIVATE, &WindowView::onActivate, this);
    this->Hide();
}

void WindowView::show()
{
    this->Show();
    Bind(wxEVT_ACTIVATE, &WindowView::onActivate, this);
}

void WindowView::OnClose(wxCloseEvent& WXUNUSED(event))
{
    wxCommandEvent parent_event(CHILD_WINDOW_CLOSED_EVENT);
    wxPostEvent(GetParent(), parent_event);
    hide();
}

void WindowView::newElement(const std::string& element_name)
{
    ElementSettings elem;
    elem.x = 0;
    elem.y = 0;
    elem.width = 0.3;
    elem.height = 0.3;
    elem.name = element_name;

    GuiElement* const ge = new PlotWindowGLPane(dynamic_cast<wxNotebookPage*>(this),
        elem,
        grid_size_,
        notify_main_window_key_pressed_,
        notify_main_window_key_released_);

    ge->updateSizeFromParent(this->GetSize());
    if (is_editing_)
    {
        ge->setIsEditing(true);
    }
    gui_elements_[elem.name] = ge;
}

void WindowView::newElement()
{
    ElementSettings elem;
    elem.x = 0;
    elem.y = 0;
    elem.width = 0.3;
    elem.height = 0.3;
    elem.name = "new-element-" + std::to_string(current_unused_element_idx++);

    GuiElement* const ge = new PlotWindowGLPane(dynamic_cast<wxNotebookPage*>(this),
        elem,
        grid_size_,
        notify_main_window_key_pressed_,
        notify_main_window_key_released_);

    ge->updateSizeFromParent(this->GetSize());
    if (is_editing_)
    {
        ge->setIsEditing(true);
    }
    gui_elements_[elem.name] = ge;
}
