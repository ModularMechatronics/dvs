#ifndef ATTRIBUTES_H_
#define ATTRIBUTES_H_

#include <stdint.h>
#include <stdlib.h>

template <typename T> T getAsPlainOldData()
{
    static_assert(std::is_same<T, float>::value || std::is_same<T, double>::value || std::is_same<T, int>::value ||
                      std::is_same<T, uint64_t>::value || std::is_same<T, uint32_t>::value ||
                      std::is_same<T, uint16_t>::value || std::is_same<T, uint8_t>::value ||
                      std::is_same<T, int64_t>::value || std::is_same<T, int32_t>::value ||
                      std::is_same<T, int16_t>::value || std::is_same<T, int8_t>::value ||
                      std::is_same<T, char>::value || std::is_same<T, unsigned char>::value,
                  "Must be a POD type!");

    T out_var;
    uint8_t* ptr = reinterpret_cast<uint8_t*>(&out_var);

    for (size_t k = 0; k < sizeof(T); k++)
    {
        ptr[k] = data[k];
    }

    return out_var;
}

template <typename T> T getAsProperty() const
{
    // static_assert(std::is_base_of<PropertyBase, T>::value, "Must be a child class of AttributeBase!");

    /*
    if(num_bytes != (sizeof(T) + sizeof(AttributeType)))
    {
        throw std::exception("Mismatch in expected size!");
    }
    */

    T out_var;
    // out_var.setPropertyType(object_type);
    uint8_t* ptr = reinterpret_cast<uint8_t*>(&out_var);

    for (size_t k = 0; k < sizeof(T); k++)
    {
        ptr[k] = data[k];
    }

    return out_var;
}

/*void MainWindow::onRightClickMenu(wxCommandEvent& event)
{
    std::cout << "Menu!" << std::endl;
    wxTextEntryDialog* wx_te_dialog = new wxTextEntryDialog(this, "Change name", "Enter a new tab name", "<old-name>",
wxOK | wxCANCEL | wxCENTRE);

    if ( wx_te_dialog->ShowModal() == wxID_OK )
    {
        const wxString value = wx_te_dialog->GetValue();
        std::cout << std::string(value.mb_str()) << std::endl;
    }
}

void MainWindow::onShowContextMenu(wxContextMenuEvent& event)
{
    std::cout << event.GetString() << std::endl;
    wxMenu menu;

    menu.Append(MENU_ID_CONTEXT_1, "Context Menu command 1");
    menu.Append(MENU_ID_CONTEXT_2, "Context Menu command 2");
    menu.Append(MENU_ID_CONTEXT_3, "Context Menu command 3");

    PopupMenu(&menu);
}*/

// void onRightClickMenu(wxCommandEvent& event);
// void onShowContextMenu(wxContextMenuEvent& event);
// enum MenuIDs { MENU_ID_CONTEXT_1 = wxID_HIGHEST + 4, MENU_ID_CONTEXT_2, MENU_ID_CONTEXT_3 };

// Bind(wxEVT_CONTEXT_MENU, &MainWindow::onShowContextMenu, this);
// Bind(wxEVT_COMMAND_MENU_SELECTED, &MainWindow::onRightClickMenu, this, MENU_ID_CONTEXT_1, MENU_ID_CONTEXT_3);

tb_edit = wxBitmap(wxT("../icons/edit.png"), wxBITMAP_TYPE_PNG);
tb_delete = wxBitmap(wxT("../icons/delete.png"), wxBITMAP_TYPE_PNG);
tb_done = wxBitmap(wxT("../icons/done2.png"), wxBITMAP_TYPE_PNG);
tb_add = wxBitmap(wxT("../icons/add.png"), wxBITMAP_TYPE_PNG);

namespace dvs
{
/*namespace internal
{
    constexpr int num_bytes_in_attr_data = 24;
    constexpr int max_num_data_bytes = num_bytes_in_attr_data + 2;
}

enum class AttributeType : uint8_t
{
    NO_TYPE,
    COLOR,
    LINE_WIDTH,
    LINE_COLOR,
    FACE_COLOR
};

struct Color
{
    float r, g, b;
    Color() = default;
    Color(const float r_, const float g_, const float b_) : r(r_), g(g_), b(b_) {}
};

class Attribute
{
private:
    uint8_t data_[internal::num_bytes_in_attr_data];
    uint8_t num_bytes_used_;
    AttributeType type;

public:

    void fillBufferWithInternalData(uint8_t data_to_fill[internal::max_num_data_bytes])
    {
        data_to_fill[0] = static_cast<uint8_t>(type);
        data_to_fill[1] = num_bytes_used_;
        for(size_t k = 0; k < num_bytes_used_; k++)
        {
            data_to_fill[k + 2] = data_[k];
        }
    }

    Attribute()
    {

        std::memset(data_, 0, internal::num_bytes_in_attr_data);
        num_bytes_used_ = 0;
        type = AttributeType::NO_TYPE;
    };

    Attribute(const Color& color);
    Attribute& operator=(const Color& color);
};

inline Attribute::Attribute(const Color& color)
{
    const uint8_t* r_data = reinterpret_cast<const uint8_t*>(&color.r);
    const uint8_t* g_data = reinterpret_cast<const uint8_t*>(&color.g);
    const uint8_t* b_data = reinterpret_cast<const uint8_t*>(&color.b);

    std::memcpy(data_, r_data, sizeof(color.r));
    std::memcpy(&(data_[sizeof(color.r)]), g_data, sizeof(color.g));
    std::memcpy(&(data_[sizeof(color.r) + sizeof(color.g)]), b_data, sizeof(color.b));

    num_bytes_used_ = sizeof(color.r) + sizeof(color.g) + sizeof(color.b);

    type = AttributeType::COLOR;
}

inline Attribute& Attribute::operator=(const Color& color)
{
    const uint8_t* r_data = reinterpret_cast<const uint8_t*>(&color.r);
    const uint8_t* g_data = reinterpret_cast<const uint8_t*>(&color.g);
    const uint8_t* b_data = reinterpret_cast<const uint8_t*>(&color.b);

    std::memcpy(data_, r_data, sizeof(color.r));
    std::memcpy(&(data_[sizeof(color.r)]), g_data, sizeof(color.g));
    std::memcpy(&(data_[sizeof(color.r) + sizeof(color.g)]), b_data, sizeof(color.b));

    num_bytes_used_ = sizeof(color.r) + sizeof(color.g) + sizeof(color.b);

    type = AttributeType::COLOR;

    return *this;
}*/

}

#endif
