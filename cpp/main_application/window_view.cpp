#include "window_view.h"

#include "plot_window_gl_pane.h"

#include "events.h"

WindowView::WindowView(wxFrame* parent, const WindowSettings& window_settings, const int callback_id) : 
    ViewBase<wxFrame>(parent, window_settings)
{
    callback_id_ = callback_id;
    const int outer = 245;
    const int middle = 200;
    const wxColor outer_color(outer, outer, outer);
    const wxColor middle_color(middle, middle, middle);

    background_panel_ = new BackgroundPanel(this, this->GetSize(), outer_color, middle_color);

    const std::vector<project_file::ElementSettings> elements = settings_->getElementSettingsList();

    for(const auto elem : elements)
    {
        GuiElement* const ge = new PlotWindowGLPane(dynamic_cast<wxFrame*>(this), elem, grid_size_);

        ge->updateSizeFromParent(this->GetSize());
        gui_elements_[elem.name] = ge;
    }

    this->SetLabel(window_settings.getName());

    if(gui_elements_.size() > 0)
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

    for(auto it : gui_elements_)
    {
        it.second->updateSizeFromParent(new_size);
    }

    background_panel_->resize(new_size);
}

int WindowView::getCallbackId() const
{
    return callback_id_;
}

void WindowView::onActivate(wxActivateEvent& event)
{
    if(event.GetActive())
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

void WindowView::newElement()
{
    current_unnamed_idx_++;
    ElementSettings elem;
    elem.x = 0;
    elem.y = 0;
    elem.width = 0.3;
    elem.height = 0.3;
    elem.name = "no-name-" + std::to_string(current_unnamed_idx_);

    GuiElement* const ge = new PlotWindowGLPane(dynamic_cast<wxNotebookPage*>(this), elem, grid_size_);

    ge->updateSizeFromParent(this->GetSize());
    if(is_editing_)
    {
        ge->setIsEditing(true);
    }
    gui_elements_[elem.name] = ge;
}
