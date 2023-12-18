#include "gui_element.h"

ApplicationGuiElement::ApplicationGuiElement(
    const std::shared_ptr<ElementSettings>& element_settings,
    const std::function<void(const char key)>& notify_main_window_key_pressed,
    const std::function<void(const char key)>& notify_main_window_key_released,
    const std::function<void(const wxPoint pos, const std::string& elem_name)>&
        notify_parent_window_right_mouse_pressed,
    const std::function<void()>& notify_main_window_about_modification,
    const std::function<void(const wxPoint& pos, const wxSize& size, const bool is_editing)>& notify_tab_about_editing,
    const std::function<void(const Color_t, const std::string&)>& push_text_to_cmdl_output_window)
    : element_settings_{element_settings},
      notify_main_window_key_pressed_{notify_main_window_key_pressed},
      notify_main_window_key_released_{notify_main_window_key_released},
      notify_parent_window_right_mouse_pressed_{notify_parent_window_right_mouse_pressed},
      notify_main_window_about_modification_{notify_main_window_about_modification},
      notify_tab_about_editing_{notify_tab_about_editing},
      push_text_to_cmdl_output_window_{push_text_to_cmdl_output_window}
{
    control_pressed_at_mouse_press_ = false;
    shift_pressed_at_mouse_press_ = false;
    mouse_is_inside_ = false;

    edit_size_margin_ = 5.0f;
    minimum_x_pos_ = 70;
    minimum_y_pos_ = 30;
}

void ApplicationGuiElement::keyPressed(const char key)
{
    if (wxGetKeyState(WXK_COMMAND))
    {
        wxWindow* parent_handle = this->getParent();

        const float x0 = static_cast<float>(this->getPosition().x);
        const float y0 = static_cast<float>(this->getPosition().y);

        const float x1 = x0 + static_cast<float>(this->getSize().GetWidth());
        const float y1 = y0 + static_cast<float>(this->getSize().GetHeight());

        const wxPoint pt = wxGetMousePosition() - parent_handle->GetPosition();

        if ((pt.x > x0) && (pt.x < x1) && (pt.y > y0) && (pt.y < y1))
        {
            setCursorDependingOnMousePos(pt - this->getPosition());
        }
        if (mouse_is_inside_)
        {
            notify_tab_about_editing_(this->getPosition(), this->getSize(), true);
        }
    }

    keyPressedElementSpecific(key);
}

void ApplicationGuiElement::keyReleased(const char key)
{
    if (!wxGetKeyState(WXK_COMMAND))
    {
        const wxMouseState mouse_state = wxGetMouseState();
        if (!mouse_state.LeftIsDown())
        {
            this->setCursor(wxCursor(wxCURSOR_ARROW));
            notify_tab_about_editing_(wxPoint{0, 0}, wxSize{0, 0}, false);
        }
    }

    keyReleasedElementSpecific(key);
}

void ApplicationGuiElement::mouseEnteredElement(wxMouseEvent& event)
{
    mouse_is_inside_ = true;
    const wxPoint current_mouse_local_position = event.GetPosition();
    const wxPoint current_element_position = this->getPosition();
    previous_mouse_pos_ = current_element_position + current_mouse_local_position;

    if (wxGetKeyState(WXK_COMMAND))
    {
        notify_tab_about_editing_(this->getPosition(), this->getSize(), true);
        setCursorDependingOnMousePos(event.GetPosition());
    }
}

void ApplicationGuiElement::mouseLeftElement(wxMouseEvent& event)
{
    mouse_is_inside_ = false;
    notify_tab_about_editing_(wxPoint{0, 0}, wxSize{0, 0}, false);
    this->setCursor(wxCursor(wxCURSOR_ARROW));
}

void ApplicationGuiElement::mouseLeftPressed(wxMouseEvent& event)
{
    const wxPoint current_mouse_position = event.GetPosition();

    element_pos_at_press_ = this->getPosition();
    element_size_at_press_ = this->getSize();
    mouse_pos_at_press_ = event.GetPosition() + this->getPosition();
    previous_mouse_pos_ = mouse_pos_at_press_;

    const auto [bnd, bnd_with_margin] = getBounds();

    cursor_state_at_press_ = getCursorSquareState(bnd, bnd_with_margin, current_mouse_position);

    if (wxGetKeyState(WXK_CONTROL))
    {
        control_pressed_at_mouse_press_ = true;
    }
    else
    {
        mouseLeftPressedGuiElementSpecific(event);
        event.Skip();
    }
}

void ApplicationGuiElement::mouseLeftReleased(wxMouseEvent& event)
{
    if (control_pressed_at_mouse_press_)
    {
        control_pressed_at_mouse_press_ = false;
        if (!wxGetKeyState(WXK_CONTROL))
        {
            this->setCursor(wxCursor(wxCURSOR_ARROW));
            notify_tab_about_editing_(wxPoint{0, 0}, wxSize{0, 0}, false);
        }
    }
    else
    {
        mouseLeftReleasedGuiElementSpecific(event);
        event.Skip();
    }
}

void ApplicationGuiElement::mouseMovedOverItem(wxMouseEvent& event)
{
    const wxPoint current_mouse_position = event.GetPosition();
    const wxPoint current_pane_position = this->getPosition();

    current_mouse_pos_ = current_pane_position + current_mouse_position;

    if (control_pressed_at_mouse_press_ && event.LeftIsDown())
    {
        adjustPaneSizeOnMouseMoved();
        notify_tab_about_editing_(this->getPosition(), this->getSize(), true);

        notify_main_window_about_modification_();
    }
    else
    {
        event.Skip();
        mouseMovedGuiElementSpecific(event);
    }

    if (wxGetKeyState(WXK_COMMAND) && !event.LeftIsDown())
    {
        setCursorDependingOnMousePos(event.GetPosition());
    }

    previous_mouse_pos_ = current_mouse_pos_;
}

void ApplicationGuiElement::mouseRightPressed(wxMouseEvent& event)
{
    const wxPoint current_mouse_position_local = event.GetPosition();
    const wxPoint current_pane_position = this->getPosition();

    previous_mouse_pos_ = current_pane_position + current_mouse_position_local;

    if (wxGetKeyState(WXK_SHIFT))
    {
        shift_pressed_at_mouse_press_ = true;
    }
    else
    {
        notify_parent_window_right_mouse_pressed_(this->getPosition() + event.GetPosition(),
                                                  element_settings_->handle_string);
    }
}

void ApplicationGuiElement::mouseRightReleased(wxMouseEvent& event)
{
    mouseRightPressedGuiElementSpecific(event);

    shift_pressed_at_mouse_press_ = false;
}

void ApplicationGuiElement::adjustPaneSizeOnMouseMoved()
{
    const wxPoint delta = current_mouse_pos_ - previous_mouse_pos_;
    const wxPoint delta_x = wxPoint(delta.x, 0);
    const wxPoint delta_y = wxPoint(0, delta.y);

    const wxSize delta_size_x = wxSize(delta.x, 0);
    const wxSize delta_size_y = wxSize(0, delta.y);
    const wxSize delta_size = wxSize(delta.x, delta.y);

    const wxPoint current_element_pos = this->getPosition();

    const wxSize current_size = this->getSize();

    wxPoint new_position = this->getPosition();
    wxSize new_size = this->getSize();

    switch (cursor_state_at_press_)
    {
        case CursorSquareState::LEFT:
            new_position = current_element_pos + delta_x;
            new_size = current_size - delta_size_x;
            break;
        case CursorSquareState::RIGHT:
            new_size = current_size + delta_size_x;
            break;
        case CursorSquareState::TOP:
            new_position = current_element_pos + delta_y;
            new_size = current_size - delta_size_y;
            break;
        case CursorSquareState::BOTTOM:
            new_size = current_size + delta_size_y;
            break;
        case CursorSquareState::INSIDE:
            new_position = current_element_pos + delta;
            new_size = element_size_at_press_;
            break;
        case CursorSquareState::BOTTOM_RIGHT:
            new_size = current_size + delta_size;
            break;
        case CursorSquareState::BOTTOM_LEFT:
            new_position = current_element_pos + delta_x;
            new_size = current_size - delta_size_x + delta_size_y;
            break;
        case CursorSquareState::TOP_RIGHT:
            new_position = current_element_pos + delta_y;
            new_size = current_size + delta_size_x - delta_size_y;
            break;
        case CursorSquareState::TOP_LEFT:
            new_position = current_element_pos + delta;
            new_size = current_size - delta_size_x - delta_size_y;
            break;
        case CursorSquareState::OUTSIDE:
            // Do nothing
            break;
        default:
            std::cout << "Invalid cursor state!" << std::endl;
    }
    if (new_size.GetWidth() < 10)
    {
        new_size.SetWidth(10);
        new_position.x = current_element_pos.x;
    }
    if (new_size.GetHeight() < 10)
    {
        new_size.SetHeight(10);
        new_position.y = current_element_pos.y;
    }

    const wxSize parent_size = this->getParent()->GetSize();
    const float px = parent_size.GetWidth();
    const float py = parent_size.GetHeight();

    if (new_position.x < minimum_x_pos_)
    {
        if (cursor_state_at_press_ == CursorSquareState::INSIDE)
        {
            new_position.x = current_element_pos.x;
        }
        else
        {
            new_size.SetWidth(current_size.GetWidth());
            new_position.x = current_element_pos.x;
        }
    }
    else if ((new_position.x + new_size.GetWidth()) > px)
    {
        if (cursor_state_at_press_ == CursorSquareState::INSIDE)
        {
            new_position.x = current_element_pos.x;
        }
        else
        {
            new_size.SetWidth(current_size.GetWidth());
        }
    }

    if (new_position.y < minimum_y_pos_)
    {
        if (cursor_state_at_press_ == CursorSquareState::INSIDE)
        {
            new_position.y = current_element_pos.y;
        }
        else
        {
            new_size.SetHeight(current_size.GetHeight());
            new_position.y = current_element_pos.y;
        }
    }
    else if ((new_position.y + new_size.GetHeight()) > py)
    {
        if (cursor_state_at_press_ == CursorSquareState::INSIDE)
        {
            new_position.y = current_element_pos.y;
        }
        else
        {
            new_size.SetHeight(current_size.GetHeight());
        }
    }

    if ((this->getPosition().x != new_position.x) || (this->getPosition().y != new_position.y) ||
        (new_size.GetWidth() != this->getSize().GetWidth()) || (new_size.GetHeight() != this->getSize().GetHeight()))
    {
        const float ratio_x = 1.0f - static_cast<float>(minimum_x_pos_) / px;
        const float ratio_y = 1.0f - static_cast<float>(minimum_y_pos_) / py;

        element_settings_->width = static_cast<float>(new_size.GetWidth()) / (px * ratio_x);
        element_settings_->height = static_cast<float>(new_size.GetHeight()) / (py * ratio_y);
        element_settings_->x = static_cast<float>(new_position.x - minimum_x_pos_) / (px * ratio_x);
        element_settings_->y = static_cast<float>(new_position.y - minimum_y_pos_) / (py * ratio_y);

        // Unbind(wxEVT_MOTION, &PlotPane::mouseMoved, this);  // TODO: Needed?
        setElementPositionAndSize();
        notify_main_window_about_modification_();
        // Bind(wxEVT_MOTION, &PlotPane::mouseMoved, this);
    }
}

CursorSquareState ApplicationGuiElement::getCursorSquareState(const Bound2D bound,
                                                              const Bound2D bound_margin,
                                                              const wxPoint mouse_pos)
{
    if (bound.pointIsWithin(mouse_pos))
    {
        if (mouse_pos.x <= bound_margin.x_min)
        {
            if (mouse_pos.y <= bound_margin.y_min)
            {
                return CursorSquareState::TOP_LEFT;
            }
            else if (bound_margin.y_max <= mouse_pos.y)
            {
                return CursorSquareState::BOTTOM_LEFT;
            }
            else
            {
                return CursorSquareState::LEFT;
            }
        }
        else if (bound_margin.x_max <= mouse_pos.x)
        {
            if (mouse_pos.y <= bound_margin.y_min)
            {
                return CursorSquareState::TOP_RIGHT;
            }
            else if (bound_margin.y_max <= mouse_pos.y)
            {
                return CursorSquareState::BOTTOM_RIGHT;
            }
            else
            {
                return CursorSquareState::RIGHT;
            }
        }
        else if (mouse_pos.y <= bound_margin.y_min)
        {
            return CursorSquareState::TOP;
        }
        else if (bound_margin.y_max <= mouse_pos.y)
        {
            return CursorSquareState::BOTTOM;
        }
        else
        {
            return CursorSquareState::INSIDE;
        }
    }
    else
    {
        return CursorSquareState::OUTSIDE;
    }
}

void ApplicationGuiElement::setCursorDependingOnMousePos(const wxPoint& current_mouse_position)
{
    const auto [bnd, bnd_with_margin] = getBounds();

    const CursorSquareState cms = getCursorSquareState(bnd, bnd_with_margin, current_mouse_position);

    switch (cms)
    {
        case CursorSquareState::LEFT:
            this->setCursor(wxCursor(wxCURSOR_SIZEWE));
            break;
        case CursorSquareState::RIGHT:
            this->setCursor(wxCursor(wxCURSOR_SIZEWE));
            break;
        case CursorSquareState::TOP:
            this->setCursor(wxCursor(wxCURSOR_SIZENS));
            break;
        case CursorSquareState::BOTTOM:
            this->setCursor(wxCursor(wxCURSOR_SIZENS));
            break;
        case CursorSquareState::BOTTOM_RIGHT:
            this->setCursor(wxCursor(wxCURSOR_SIZENWSE));
            break;
        case CursorSquareState::BOTTOM_LEFT:
            this->setCursor(wxCursor(wxCURSOR_SIZENESW));
            break;
        case CursorSquareState::TOP_RIGHT:
            this->setCursor(wxCursor(wxCURSOR_SIZENESW));
            break;
        case CursorSquareState::TOP_LEFT:
            this->setCursor(wxCursor(wxCURSOR_SIZENWSE));
            break;
        case CursorSquareState::INSIDE:
            this->setCursor(wxCursor(wxCURSOR_HAND));
            break;
        case CursorSquareState::OUTSIDE:
            this->setCursor(wxCursor(wxCURSOR_ARROW));
            break;
        default:
            this->setCursor(wxCursor(wxCURSOR_ARROW));
    }
}

void ApplicationGuiElement::setElementPositionAndSize()
{
    const wxSize parent_size = this->getParent()->GetSize();

    const float px = parent_size.GetWidth();
    const float py = parent_size.GetHeight();

    const float ratio_x = 1.0f - static_cast<float>(minimum_x_pos_) / px;
    const float ratio_y = 1.0f - static_cast<float>(minimum_y_pos_) / py;

    const wxSize new_size(element_settings_->width * px * ratio_x, element_settings_->height * py * ratio_y);

    const wxPoint new_pos{minimum_x_pos_ + static_cast<int>(element_settings_->x * px * ratio_x),
                          minimum_y_pos_ + static_cast<int>(element_settings_->y * py * ratio_y)};

    this->setPosition(new_pos);
    this->setSize(new_size);
}

void ApplicationGuiElement::sendGuiData()
{
    if (element_settings_->handle_string.length() >= 256U)
    {
        throw std::runtime_error("Handle string too long! Maximum length is 255 characters!");
    }

    const std::uint64_t num_bytes_to_send{basicDataSize() + getGuiPayloadSize()};

    FillableUInt8Array output_array{num_bytes_to_send};

    fillWithBasicData(output_array);

    output_array.fillWithStaticType(static_cast<std::uint32_t>(getGuiPayloadSize()));

    fillGuiPayload(output_array);
    sendThroughTcpInterface(output_array.view(), kGuiTcpPortNum);
}

void ApplicationGuiElement::fillWithBasicData(FillableUInt8Array& output_array) const
{
    const std::uint8_t handle_string_length = element_settings_->handle_string.length();

    output_array.fillWithStaticType(static_cast<std::uint8_t>(element_settings_->type));
    output_array.fillWithStaticType(handle_string_length);
    output_array.fillWithDataFromPointer(element_settings_->handle_string.data(),
                                         element_settings_->handle_string.length());
}

std::uint64_t ApplicationGuiElement::basicDataSize() const
{
    const std::uint8_t handle_string_length = element_settings_->handle_string.length();

    const std::uint64_t basic_data_size = handle_string_length +  // the handle_string itself
                                          sizeof(std::uint8_t) +  // length of handle_string
                                          sizeof(std::uint8_t) +  // type
                                          sizeof(std::uint32_t);  // payload size
    return basic_data_size;
}

void ApplicationGuiElement::setCursor(const wxCursor& cursor)
{
    wxSetCursor(cursor);
}

void ApplicationGuiElement::keyPressedCallback(wxKeyEvent& evt)
{
    const int key = evt.GetUnicodeKey();
    notify_main_window_key_pressed_(key);
}

void ApplicationGuiElement::keyReleasedCallback(wxKeyEvent& evt)
{
    const int key = evt.GetUnicodeKey();
    notify_main_window_key_released_(key);
}
