#ifndef MAIN_APPLICATION_PLOT_PANE_H_
#define MAIN_APPLICATION_PLOT_PANE_H_

#include <wx/glcanvas.h>
#include <wx/notebook.h>
#include <wx/wx.h>

#include "axes/axes.h"
#include "communication/received_data.h"
#include "communication/udp_server.h"
#include "dvs/dvs.h"
#include "gui_element.h"
#include "io_devices/io_devices.h"
#include "opengl_low_level/opengl_header.h"
#include "plot_data_handler.h"

struct Bound2D
{
    float x_min;
    float x_max;
    float y_min;
    float y_max;

    Bound2D() = default;
};

inline Bound2D operator+(const Bound2D& bnd, const Vec2f& offset)
{
    Bound2D new_bnd;
    new_bnd.x_min = bnd.x_min + offset.x;
    new_bnd.x_max = bnd.x_max + offset.x;
    new_bnd.y_min = bnd.y_min + offset.y;
    new_bnd.y_max = bnd.y_max + offset.y;

    return new_bnd;
}

enum class CursorSquareState
{
    INSIDE,
    OUTSIDE,
    LEFT,
    RIGHT,
    TOP,
    BOTTOM,
    TOP_LEFT,
    TOP_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_RIGHT
};

class PlotPane : public wxGLCanvas, public GuiElement
{
private:
    wxGLContext* getContext();
    wxGLContext* m_context;

    AxesSettings axes_settings_;
    AxesInteractor axes_interactor_;
    AxesRenderer* axes_renderer_;
    MouseButtonState left_mouse_button_;
    KeyboardState keyboard_state_;
    CursorSquareState cursor_state_at_press_;

    bool hold_on_;
    bool axes_set_;
    bool view_set_;

    int args[9];

    bool perspective_projection_;
    bool wait_for_flush_;

    int* getArgsPtr();
    wxSize parent_size_;
    Vec2f mouse_pos_at_press_;
    Vec2f current_mouse_pos_;
    wxPoint pos_at_press_;
    wxSize size_at_press_;
    float grid_size_;
    float edit_size_margin_;

    PlotDataHandler* plot_data_handler_;
    ShaderCollection shader_collection_;

    void notifyParentAboutModification();
    bool is3DFunction(const Function fcn);
    bool isImageFunction(const Function fcn);
    void initShaders();
    MouseInteractionAxis current_mouse_interaction_axis_;
    float legend_scale_factor_ = 1.0f;

public:
    PlotPane(wxNotebookPage* parent,
             const ElementSettings& element_settings,
             const float grid_size,
             const std::function<void(const char key)>& notify_main_window_key_pressed,
             const std::function<void(const char key)>& notify_main_window_key_released,
             const std::function<void(const wxPoint pos)>& notify_parent_window_right_mouse_pressed);
    ~PlotPane();

    int getWidth();
    int getHeight();

    void render(wxPaintEvent& evt);

    void setPosition(const wxPoint& new_pos);
    void setSize(const wxSize& new_size);
    void updateSizeFromParent(const wxSize& parent_size) override;
    void addData(std::unique_ptr<const ReceivedData> received_data,
                 const dvs::internal::CommunicationHeader& hdr) override;
    void show() override;
    void hide() override;
    void destroy() override;
    void refresh() override;
    void keyPressed(const char key) override;
    void keyReleased(const char key) override;
    void showLegend(const bool show_legend) override;
    void waitForFlush() override;
    void keyPressedCallback(wxKeyEvent& evt);
    void keyReleasedCallback(wxKeyEvent& evt);
    void mouseRightPressed(wxMouseEvent& event);

    // Event callback function
    void mouseMoved(wxMouseEvent& event);
    void mouseLeftWindow(wxMouseEvent& event);
    void mouseLeftPressed(wxMouseEvent& event);
    void mouseLeftReleased(wxMouseEvent& event);

    void bindCallbacks();
};

#endif  // MAIN_APPLICATION_PLOT_PANE_H_
