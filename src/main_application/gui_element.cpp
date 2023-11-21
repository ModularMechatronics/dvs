#include "gui_element.h"

void ApplicationGuiElement::mouseEnteredElement(wxMouseEvent& event)
{
    const wxPoint current_mouse_local_position = event.GetPosition();
    const wxPoint current_element_position = this->getPosition();
    previous_mouse_pos_ = current_element_position + current_mouse_local_position;

    if (wxGetKeyState(WXK_COMMAND))
    {
        setCursorDependingOnMousePos(event.GetPosition());
    }
}

void ApplicationGuiElement::mouseLeftElement(wxMouseEvent& event)
{
    this->setCursor(wxCursor(wxCURSOR_ARROW));
}

void ApplicationGuiElement::mouseLeftPressed(wxMouseEvent& event)
{
    const wxPoint current_mouse_position = event.GetPosition();

    element_pos_at_press_ = this->getPosition();
    element_size_at_press_ = this->getSize();

    const auto [bnd, bnd_with_margin] = getBounds();

    cursor_state_at_press_ = getCursorSquareState(bnd, bnd_with_margin, current_mouse_position);

    if (wxGetKeyState(WXK_CONTROL))
    {
        control_pressed_at_mouse_press_ = true;

        const wxPoint element_pos_at_press = this->getPosition();

        mouse_pos_at_press_ = event.GetPosition() + element_pos_at_press;
        previous_mouse_pos_ = mouse_pos_at_press_;
    }
    else
    {
        mouseReleasedGuiElementSpecific(event);
        event.Skip();
    }
}

void ApplicationGuiElement::mouseLeftReleased(wxMouseEvent& event)
{
    if (control_pressed_at_mouse_press_)
    {
        control_pressed_at_mouse_press_ = false;
    }
    else
    {
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

        notify_main_window_about_modification_();
    }
    else
    {
        event.Skip();
        mousePressedGuiElementSpecific(event);
    }

    if (wxGetKeyState(WXK_COMMAND))
    {
        setCursorDependingOnMousePos(event.GetPosition());
    }

    previous_mouse_pos_ = current_mouse_pos_;

    // TODO: Where to call?
    // notify_parent_window_right_mouse_pressed_(this->getPosition() + event.GetPosition(),
    //                                               element_settings_->handle_string);
}

void ApplicationGuiElement::adjustPaneSizeOnMouseMoved()
{
    const Vec2f delta(current_mouse_pos_.x - previous_mouse_pos_.x, current_mouse_pos_.y - previous_mouse_pos_.y);

    const Vec2f current_pos(this->getPosition().x, this->getPosition().y);

    const wxSize current_size = this->getSize();

    wxPoint new_position = this->getPosition();
    wxSize new_size = this->getSize();

    switch (cursor_state_at_press_)
    {
        case CursorSquareState::LEFT:
            new_position = wxPoint(current_pos.x + delta.x, current_pos.y);
            new_size = wxSize(current_size.GetWidth() - delta.x, current_size.GetHeight());
            break;
        case CursorSquareState::RIGHT:
            new_size = wxSize(current_size.GetWidth() + delta.x, current_size.GetHeight());
            break;
        case CursorSquareState::TOP:
            new_position = wxPoint(current_pos.x, current_pos.y + delta.y);
            new_size = wxSize(current_size.GetWidth(), current_size.GetHeight() - delta.y);
            break;
        case CursorSquareState::BOTTOM:
            new_size = wxSize(current_size.GetWidth(), current_size.GetHeight() + delta.y);
            break;
        case CursorSquareState::INSIDE:
            new_position = wxPoint(current_pos.x + delta.x, current_pos.y + delta.y);
            new_size = element_size_at_press_;
            break;
        case CursorSquareState::BOTTOM_RIGHT:
            new_size = wxSize(current_size.GetWidth() + delta.x, current_size.GetHeight() + delta.y);
            break;
        case CursorSquareState::BOTTOM_LEFT:
            new_position = wxPoint(current_pos.x + delta.x, current_pos.y);
            new_size = wxSize(current_size.GetWidth() - delta.x, current_size.GetHeight() + delta.y);
            break;
        case CursorSquareState::TOP_RIGHT:
            new_position = wxPoint(current_pos.x, current_pos.y + delta.y);
            new_size = wxSize(current_size.GetWidth() + delta.x, current_size.GetHeight() - delta.y);
            break;
        case CursorSquareState::TOP_LEFT:
            new_position = wxPoint(current_pos.x + delta.x, current_pos.y + delta.y);
            new_size = wxSize(current_size.GetWidth() - delta.x, current_size.GetHeight() - delta.y);
            break;
        default:
            std::cout << "Invalid cursor state!" << std::endl;
    }
    if (new_size.GetWidth() < 50)
    {
        new_size.SetWidth(50);
        new_position.x = current_pos.x;
    }
    if (new_size.GetHeight() < 50)
    {
        new_size.SetHeight(50);
        new_position.y = current_pos.y;
    }

    const wxSize parent_size = this->getParent()->GetSize();
    const float px = parent_size.GetWidth();
    const float py = parent_size.GetHeight();

    if (new_position.x < minimum_x_pos_)
    {
        if (cursor_state_at_press_ == CursorSquareState::INSIDE)
        {
            new_position.x = current_pos.x;
        }
        else
        {
            new_size.SetWidth(current_size.GetWidth());
            new_position.x = current_pos.x;
        }
    }
    else if ((new_position.x + new_size.GetWidth()) > px)
    {
        if (cursor_state_at_press_ == CursorSquareState::INSIDE)
        {
            new_position.x = current_pos.x;
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
            new_position.y = current_pos.y;
        }
        else
        {
            new_size.SetHeight(current_size.GetHeight());
            new_position.y = current_pos.y;
        }
    }
    else if ((new_position.y + new_size.GetHeight()) > py)
    {
        if (cursor_state_at_press_ == CursorSquareState::INSIDE)
        {
            new_position.y = current_pos.y;
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
    Bound2D bnd;
    bnd.x_min = 0.0f;
    bnd.x_max = this->getSize().GetWidth();
    bnd.y_min = 0.0f;
    bnd.y_max = this->getSize().GetHeight();

    Bound2D bnd_margin;
    bnd_margin.x_min = bnd.x_min + edit_size_margin_;
    bnd_margin.x_max = bnd.x_max - edit_size_margin_;
    bnd_margin.y_min = bnd.y_min + edit_size_margin_;
    bnd_margin.y_max = bnd.y_max - edit_size_margin_;

    const CursorSquareState cms = getCursorSquareState(bnd, bnd_margin, current_mouse_position);

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
        default:
            this->setCursor(wxCursor(wxCURSOR_HAND));
    }
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
