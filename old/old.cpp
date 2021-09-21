#ifndef ATTRIBUTES_H_
#define ATTRIBUTES_H_

#include <stdint.h>
#include <stdlib.h>

inline void sleepMS(const int ms)
{
    #include <stdlib.h>
    #include <unistd.h>
    usleep(ms * 1000);
}

void drawCircle3D(const Point3Dd p)
{
    const int num_points = 15;
    const float dt = 2.0f * M_PI / static_cast<float>(num_points);
    float t = 0.0f;
    const float r = 0.01f;

    glLineWidth(1);
    glColor3f(1.0f, 0.0f, 0.0f);

    glBegin(GL_LINE_LOOP);
    for (size_t k = 0; k < num_points; k++)
    {
        const float x = r * std::cos(t) + p.x;
        const float y = r * std::sin(t) + p.y;
        t += dt;
        glVertex3f(x, y, p.z);
    }
    glEnd();
}

void drawCircle(const Point2Dd p)
{
    const int num_points = 15;
    const float dt = 2.0f * M_PI / static_cast<float>(num_points);
    float t = 0.0f;
    const float r = 0.01f;

    glLineWidth(1);
    glColor3f(1.0f, 0.0f, 0.0f);

    glBegin(GL_LINE_LOOP);
    for (size_t k = 0; k < num_points; k++)
    {
        const float x = r * std::cos(t) + p.x;
        const float y = r * std::sin(t) + p.y;
        t += dt;
        glVertex2f(x, y);
    }
    glEnd();
}

#if 0
    const std::string splash_img_path = "../splash_trans_small.png";
    wxInitAllImageHandlers();
    wxImage splash_img(splash_img_path, wxBITMAP_TYPE_PNG);
    wxSplashScreen *scrn = new wxSplashScreen(splash_img,
                                              wxSPLASH_CENTRE_ON_SCREEN | wxSPLASH_TIMEOUT,
                                              2500,
                                              NULL,
                                              wxID_ANY,
                                              wxDefaultPosition,
                                              wxSize(500, 500),
                                              wxFRAME_NO_TASKBAR | wxSTAY_ON_TOP | wxTRANSPARENT_WINDOW);
    // SplashScreen?
    // wxFrame *frame = new wxFrame(NULL, wxID_ANY, "Something", wxPoint(300, 300), wxSize(200, 200), wxFRAME_TOOL_WINDOW | wxNO_BORDER);
    // frame->Show(true);
#endif

void modifyStuff(const AxesLimits& axes_limits,
                 const ViewAngles& view_angles,
                 const CoordinateConverter& coord_converter,
                 AxesSettings& axes_settings,
                 const float width,
                 const float height)
{
    dvs::Point3Dd p0(0.5, 0.5, 0.5);
    const auto R = view_angles.getSnappedRotationMatrix();
    const float azimuth = view_angles.getSnappedAzimuth();
    const float elevation = view_angles.getSnappedElevation();

    const auto pr = R * p0;

    const double tx = 0.9;

    const float sq = tx / pr.x;
    const float sx = sq * cos(azimuth);
    const float sy = sq * sin(azimuth);
    // const float sz = cos(elevation);

    Vec3Dd axes_scale = axes_settings.getAxesScale();
    // axes_scale.x = axes_scale.x * sx;
    // axes_scale.y = axes_scale.y * sy;
    // axes_scale.z = axes_scale.z * sz;

    axes_settings.setAxesScale(axes_scale);
}

void drawDebugPoints(const AxesLimits& axes_limits,
                     const ViewAngles& view_angles,
                     const CoordinateConverter& coord_converter,
                     const float width,
                     const float height)
{
    // std::cout << "Width: " << width << ", height: " << height << std::endl;

    dvs::Point3Dd p0(0.5, 0.5, 0.5);
    const auto R = view_angles.getSnappedRotationMatrix();
    const auto pr = R * p0;
    dvs::Point2Dd p1(pr.x, pr.y);

    const double q = 0.5;
    // clang-format off
    const Vector<Point3Dd> points = {{q, q, q},
                                     {-q, q, q},
                                     {q, -q, q},
                                     {-q, -q, q},
                                     {q, q, -q},
                                     {-q, q, -q},
                                     {q, -q, -q},
                                     {-q, -q, -q}};
    Point3Dd pr0 = R * points(0);
    // clang-format on
    std::pair<Point3Dd, Point3Dd> pmiw = {points(0), pr0}, pmaw = {points(0), pr0}, pmih = {points(0), pr0},
                                  pmah = {points(0), pr0};
    for (const Point3Dd p : points)
    {
        const auto pqr = R * p;
        // drawCircle({pqr.x, pqr.y});
        if (pqr.x < pmiw.second.x)
        {
            pmiw.second = pqr;
            pmiw.first = p;
        }

        if (pqr.x > pmaw.second.x)
        {
            pmaw.second = pqr;
            pmaw.first = p;
        }

        if (pqr.y < pmih.second.y)
        {
            pmih.second = pqr;
            pmih.first = p;
        }

        if (pqr.y > pmah.second.y)
        {
            pmah.second = pqr;
            pmah.first = p;
        }
    }

    const double sx2 = -0.9 / pmiw.second.x;

    drawCircle({pmiw.second.x, pmiw.second.y});
    drawCircle({pmaw.second.x, pmaw.second.y});
    drawCircle({pmih.second.x, pmih.second.y});
    drawCircle({pmah.second.x, pmah.second.y});

    const Vec3Dd new_scale = findScale(R, view_angles.getSnappedAzimuth(), view_angles.getSnappedElevation());
    for (const Point3Dd p : points)
    {
        // Point3Dd pq = p.elementWiseMultiply(new_scale);
        Point3Dd p_new = R * p;
        drawCircle({new_scale.x * p_new.x, new_scale.y * p_new.y});
    }
}

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
