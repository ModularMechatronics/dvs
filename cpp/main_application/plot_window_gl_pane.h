#ifndef MAIN_APPLICATION_PLOT_WINDOW_GL_PANE_H_
#define MAIN_APPLICATION_PLOT_WINDOW_GL_PANE_H_

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
#include "plot_data.h"
#include "view_base.h"

struct Bound2Df
{
    float x_min;
    float x_max;
    float y_min;
    float y_max;

    Bound2Df() = default;
};

inline Bound2Df operator+(const Bound2Df& bnd, const Vec2Df& offset)
{
    Bound2Df new_bnd;
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

class PlotWindowGLPane : public wxGLCanvas, public GuiElement
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

    int* getArgsPtr();
    wxSize parent_size_;
    Vec2Df mouse_pos_at_press_;
    Vec2Df current_mouse_pos_;
    wxPoint pos_at_press_;
    wxSize size_at_press_;
    float grid_size_;
    float edit_size_margin_;

    PlotDataHandler* plot_data_handler_;
    ShaderCollection shader_collection_;

    SuperBase* view_parent_;

    // ViewBase<wxNotebookPage>* tab_view_parent_;
    // ViewBase<wxFrame>* window_view_parent_;

    void notifyParentAboutModification();
    bool is3DFunction(const Function fcn);
    bool isImageFunction(const Function fcn);
    void initShaders();
    MouseInteractionAxis current_mouse_interaction_axis_;
    float legend_scale_factor_ = 1.0f;

public:
    PlotWindowGLPane(wxNotebookPage* parent,
        const ElementSettings& element_settings,
        const float grid_size,
        const std::function<void(const char key)>& notify_main_window_key_pressed,
        const std::function<void(const char key)>& notify_main_window_key_released);
    ~PlotWindowGLPane();

    int getWidth();
    int getHeight();

    void render(wxPaintEvent& evt);

    void setPosition(const wxPoint& new_pos);
    void setSize(const wxSize& new_size);
    void updateSizeFromParent(const wxSize& parent_size) override;
    void addData(std::unique_ptr<const ReceivedData> received_data, const dvs::internal::FunctionHeader& hdr) override;
    void show() override;
    void hide() override;
    void resetSelection() override;
    void setSelection() override;
    void destroy() override;
    void refresh() override;
    void keyPressed(const char key) override;
    void keyReleased(const char key) override;
    void showLegend(const bool show_legend) override;
    void keyPressedCallback(wxKeyEvent& evt);
    void keyReleasedCallback(wxKeyEvent& evt);

    // Event callback function
    void mouseMoved(wxMouseEvent& event);
    void mouseLeftWindow(wxMouseEvent& event);
    void mouseLeftPressed(wxMouseEvent& event);
    void mouseLeftReleased(wxMouseEvent& event);

    void bindCallbacks();
};

#endif // MAIN_APPLICATION_PLOT_WINDOW_GL_PANE_H_
