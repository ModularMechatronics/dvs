
template <class C>
// class MovableElement : public C, public api_internal::InternalGuiElement, public ApplicationGuiElement
class MovableElement : public C, public ApplicationGuiElement
{
public:
    // Slider
    /*MovableElement(wxFrame* parent,
                   const std::string& handle_string,
                   const GuiElementCallback& elem_callback,
                   const GuiElementType gui_element_type,
                   const wxWindowID id,
                   const int init_value,
                   const int min_value,
                   const int max_value,
                   const wxPoint& pos,
                   const wxSize& size,
                   const long style)
        : C(parent, id, init_value, min_value, max_value, pos, size, style),
          api_internal::InternalGuiElement(handle_string, elem_callback, gui_element_type)
    {
        control_pressed_at_mouse_down_ = false;

        this->Bind(wxEVT_MOTION, &MovableElement<C>::mouseMovedOverItem, this);
        this->Bind(wxEVT_LEFT_DOWN, &MovableElement<C>::mouseLeftPressed, this);
        this->Bind(wxEVT_LEFT_UP, &MovableElement<C>::mouseLeftReleased, this);
    }*/

    // Button, Checkbox, ComboBox
    MovableElement(wxFrame* parent,
                   const std::string& handle_string,
                   // const GuiElementCallback& elem_callback,
                   const GuiElementType gui_element_type,
                   const wxWindowID id,
                   const std::string& label,
                   const wxPoint& pos,
                   const wxSize& size)
        : C(parent,
            id,
            label,
            pos,
            size)  // ,
                   // api_internal::InternalGuiElement(handle_string, elem_callback, gui_element_type)
    {
        control_pressed_at_mouse_down_ = false;

        this->Bind(wxEVT_MOTION, &MovableElement<C>::mouseMovedOverItem, this);
        this->Bind(wxEVT_LEFT_DOWN, &MovableElement<C>::mouseLeftPressed, this);
        this->Bind(wxEVT_LEFT_UP, &MovableElement<C>::mouseLeftReleased, this);
    }

    // ListBox
    /*MovableElement(wxFrame* parent,
                   const std::string& handle_string,
                   const GuiElementCallback& elem_callback,
                   const GuiElementType gui_element_type,
                   const wxWindowID id,
                   const wxPoint& pos,
                   const wxSize& size)
        : C(parent, id, pos, size), api_internal::InternalGuiElement(handle_string, elem_callback, gui_element_type)
    {
        control_pressed_at_mouse_down_ = false;

        this->Bind(wxEVT_MOTION, &MovableElement<C>::mouseMovedOverItem, this);
        this->Bind(wxEVT_LEFT_DOWN, &MovableElement<C>::mouseLeftPressed, this);
        this->Bind(wxEVT_LEFT_UP, &MovableElement<C>::mouseLeftReleased, this);
    }

    // EditableText
    MovableElement(wxFrame* parent,
                   const std::string& handle_string,
                   const GuiElementCallback& elem_callback,
                   const GuiElementType gui_element_type,
                   const wxWindowID id,
                   const std::string& initial_text,
                   const wxPoint& pos,
                   const wxSize& size,
                   const long style)
        : C(parent, id, initial_text, pos, size, style),
          api_internal::InternalGuiElement(handle_string, elem_callback, gui_element_type)
    {
        control_pressed_at_mouse_down_ = false;

        this->Bind(wxEVT_MOTION, &MovableElement<C>::mouseMovedOverItem, this);
        this->Bind(wxEVT_LEFT_DOWN, &MovableElement<C>::mouseLeftPressed, this);
        this->Bind(wxEVT_LEFT_UP, &MovableElement<C>::mouseLeftReleased, this);
    }

    // RadioButtonGroup
    MovableElement(wxFrame* parent,
                   const std::string& handle_string,
                   const GuiElementCallback& elem_callback,
                   const GuiElementType gui_element_type,
                   const wxWindowID id,
                   const std::string& initial_text,
                   const wxPoint& pos,
                   const wxSize& size,
                   const wxArrayString& radio_buttons_to_add,
                   const int n,
                   const long style)
        : C(parent, id, initial_text, pos, size, radio_buttons_to_add, n, style),
          api_internal::InternalGuiElement(handle_string, elem_callback, gui_element_type)
    {
        control_pressed_at_mouse_down_ = false;

        this->Bind(wxEVT_MOTION, &MovableElement<C>::mouseMovedOverItem, this);
        this->Bind(wxEVT_LEFT_DOWN, &MovableElement<C>::mouseLeftPressed, this);
        this->Bind(wxEVT_LEFT_UP, &MovableElement<C>::mouseLeftReleased, this);
    }*/

    void updateSizeFromParent(const wxSize& parent_size) override {}

    void keyPressed(const char key) override {}

    void keyReleased(const char key) override {}

    void setMouseInteractionType(const MouseInteractionType mit) override {}

    void show() override {}

    void hide() override {}

    void destroy() override {}

    // void refresh() override {}

    // void update() override {}

    void mouseLeftPressed(wxMouseEvent& event)
    {
        if (wxGetKeyState(WXK_CONTROL))
        {
            control_pressed_at_mouse_down_ = true;

            const wxPoint pos_at_press = this->GetPosition();

            mouse_pos_at_press_ = event.GetPosition() + pos_at_press;
            previous_mouse_pos_ = mouse_pos_at_press_;
        }
        else
        {
            event.Skip();
        }
    }

    void mouseLeftReleased(wxMouseEvent& event)
    {
        if (control_pressed_at_mouse_down_)
        {
            control_pressed_at_mouse_down_ = false;
        }
        else
        {
            event.Skip();
        }
    }

    void mouseMovedOverItem(wxMouseEvent& event)
    {
        if (control_pressed_at_mouse_down_ && event.LeftIsDown())
        {
            const wxPoint current_mouse_position_local = event.GetPosition();
            const wxPoint current_mouse_position_global = current_mouse_position_local + this->GetPosition();
            const wxPoint delta = current_mouse_position_global - previous_mouse_pos_;
            this->SetPosition(this->GetPosition() + delta);

            previous_mouse_pos_ = current_mouse_position_global;
        }
        else
        {
            event.Skip();
        }
    }

    /*long getId() const override
    {
        return this->GetId();
    }

    // General
    void setEnabled() override
    {
        wxControl* const contr = dynamic_cast<wxControl* const>(this);
        contr->Enable(true);
    }

    void setDisabled() override
    {
        wxControl* const contr = dynamic_cast<wxControl* const>(this);
        contr->Enable(false);
    }

    void setPosition(const std::int16_t x, const std::int16_t y) override
    {
        this->SetPosition(wxPoint(x, y));
    }

    void setSize(const std::int16_t width, const std::int16_t height) override
    {
        this->SetSize(wxSize(width, height));
    }

    // Slider
    std::int32_t getValue() const override
    {
        const wxSlider* const slider = dynamic_cast<const wxSlider* const>(this);
        return slider->GetValue();
    }

    std::int32_t getMin() const override
    {
        const wxSlider* const slider = dynamic_cast<const wxSlider* const>(this);
        return slider->GetMin();
    }

    std::int32_t getMax() const override
    {
        const wxSlider* const slider = dynamic_cast<const wxSlider* const>(this);
        return slider->GetMax();
    }

    void setMax(const std::int32_t new_max) override
    {
        wxSlider* const slider = dynamic_cast<wxSlider* const>(this);
        slider->SetMax(new_max);
    }

    void setMin(const std::int32_t new_min) override
    {
        wxSlider* const slider = dynamic_cast<wxSlider* const>(this);
        slider->SetMin(new_min);
    }

    void setValue(const std::int32_t new_value) override
    {
        wxSlider* const slider = dynamic_cast<wxSlider* const>(this);
        slider->SetValue(new_value);
    }

    // CheckBox
    bool isChecked() const override
    {
        const wxCheckBox* const check_box = dynamic_cast<const wxCheckBox* const>(this);
        return check_box->IsChecked();
    }

    void setChecked() override
    {
        wxCheckBox* const check_box = dynamic_cast<wxCheckBox* const>(this);
        check_box->SetValue(true);
    }

    void setUnChecked() override
    {
        wxCheckBox* const check_box = dynamic_cast<wxCheckBox* const>(this);
        check_box->SetValue(false);
    }

    // Button, Checkbox, TextLabel
    void setLabel(const std::string& new_label) override
    {
        wxControl* const contr = dynamic_cast<wxControl* const>(this);
        contr->SetLabel(new_label);
    }

    std::string getLabel(void) override
    {
        const wxControl* const contr = dynamic_cast<const wxControl* const>(this);
        return contr->GetLabel().ToStdString();
    }

    // RadioButtonGroup
    std::int16_t getSelectionIndex() const override
    {
        const wxRadioBox* const radio_box = dynamic_cast<const wxRadioBox* const>(this);
        return radio_box->GetSelection();
    }

    std::string getSelectionString() const override
    {
        const wxRadioBox* const radio_box = dynamic_cast<const wxRadioBox* const>(this);
        return radio_box->GetStringSelection().ToStdString();
    }

    // EditableText
    void setText(const std::string& new_text) override
    {
        wxTextCtrl* const text_ctrl = dynamic_cast<wxTextCtrl* const>(this);
        text_ctrl->SetValue(new_text);
    }

    std::string getText() const override
    {
        const wxTextCtrl* const text_ctrl = dynamic_cast<const wxTextCtrl* const>(this);
        return text_ctrl->GetValue().ToStdString();
    }

    bool enterPressed() const override
    {
        const wxTextCtrl* const text_ctrl = dynamic_cast<const wxTextCtrl* const>(this);
        return text_ctrl->HasFocus() && (wxGetKeyState(WXK_RETURN));
    }

    // DropDownMenu, ListBox
    void addItem(const std::string& item_text) override
    {
        wxItemContainer* const elem = dynamic_cast<wxItemContainer* const>(this);
        elem->Append(item_text);
    }

    void removeItem(const std::string& item_text) override
    {
        wxItemContainer* const elem = dynamic_cast<wxItemContainer* const>(this);
        const int idx = elem->FindString(item_text, true);
        if (idx != wxNOT_FOUND)
        {
            elem->Delete(idx);
        }
    }

    void clearItems() override
    {
        wxItemContainer* const elem = dynamic_cast<wxItemContainer* const>(this);
        elem->Clear();
    }

    void selectItem(const std::string& item_text) override
    {
        wxItemContainer* const elem = dynamic_cast<wxItemContainer* const>(this);
        elem->SetStringSelection(item_text);
    }

    void selectItem(const std::int32_t item_idx) override
    {
        wxItemContainer* const elem = dynamic_cast<wxItemContainer* const>(this);
        elem->Select(item_idx);
    }

    std::string getSelectedItem() const override
    {
        const wxItemContainer* const elem = dynamic_cast<const wxItemContainer* const>(this);
        return elem->GetStringSelection().ToStdString();
    }

    std::int32_t getNumItems() const override
    {
        const wxItemContainer* const elem = dynamic_cast<const wxItemContainer* const>(this);
        return elem->GetCount();
    }

    std::int32_t getSelectedItemIndex() const override
    {
        const wxItemContainer* const elem = dynamic_cast<const wxItemContainer* const>(this);
        return elem->GetSelection();
    }*/

private:
    bool control_pressed_at_mouse_down_;
    wxPoint mouse_pos_at_press_;
    wxPoint previous_mouse_pos_;
};
