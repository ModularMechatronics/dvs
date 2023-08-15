#ifndef MAIN_APPLICATION_PLOT_PANE_H_
#define MAIN_APPLICATION_PLOT_PANE_H_

#include <wx/glcanvas.h>
#include <wx/notebook.h>
#include <wx/wx.h>

#include <atomic>

#include "axes/axes.h"
#include "communication/data_receiver.h"
#include "communication/received_data.h"
#include "dvs/dvs.h"
#include "gui_element.h"
#include "input_data.h"
#include "mouse_state.h"
#include "opengl_low_level/opengl_header.h"
#include "plot_data_handler.h"
#include "point_selection.h"

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
    MouseState mouse_state_;
    CursorSquareState cursor_state_at_press_;

    bool shift_pressed_at_mouse_press_;
    bool control_pressed_at_mouse_press_;
    bool left_mouse_pressed_;

    bool axes_from_min_max_disabled_;
    bool axes_set_;
    bool view_set_;

    int args[9];

    bool perspective_projection_;
    bool wait_for_flush_;

    wxSize parent_size_;
    Vec2f mouse_pos_at_press_;
    Vec2f current_mouse_pos_;
    Vec2f previous_mouse_pos_;
    wxPoint pos_at_press_;
    wxSize size_at_press_;
    float edit_size_margin_;

    PlotDataHandler* plot_data_handler_;
    PointSelection point_selection_;
    ShaderCollection shader_collection_;

    bool is3DFunction(const Function fcn);
    bool isImageFunction(const Function fcn);
    void initShaders();
    void clearPane();
    MouseInteractionAxis current_mouse_interaction_axis_;
    float legend_scale_factor_ = 1.0f;
    std::atomic<bool> pending_clear_;
    std::queue<std::unique_ptr<InputData>> queued_data_;
    std::queue<std::unique_ptr<InputData>> flush_queue_;

    dvs::Vec3<double> closest_point_;
    bool should_render_point_selection_;

    void adjustPaneSizeOnMouseMoved();
    void setCursorDependingOnMousePos(const wxPoint& current_mouse_position);

    void processActionQueue();
    void addPlotData(ReceivedData& received_data,
                     const PlotObjectAttributes& plot_object_attributes,
                     const PropertiesData& properties_data,
                     const std::shared_ptr<const ConvertedDataBase>& converted_data);

    wxGLAttributes getGLAttributes() const;

public:
    PlotPane(wxNotebookPage* parent,  // TODO: wxNotebookPage is obsolete, should be wxWindow/wxFrame?
             const ElementSettings& element_settings,
             const RGBTripletf& tab_background_color,
             const std::function<void(const char key)>& notify_main_window_key_pressed,
             const std::function<void(const char key)>& notify_main_window_key_released,
             const std::function<void(const wxPoint pos, const std::string& elem_name)>&
                 notify_parent_window_right_mouse_pressed,
             const std::function<void()>& notify_main_window_about_modification);
    ~PlotPane() override;

    int getWidth();
    int getHeight();

    void setHandleString(const std::string& new_name) override;
    void pushQueue(std::queue<std::unique_ptr<InputData>>& new_queue) override;

    void render(wxPaintEvent& evt);

    void setElementPositionAndSize();
    void setMinXPos(const int min_x_pos) override;
    void setSize(const wxSize& new_size);
    void updateSizeFromParent(const wxSize& parent_size) override;
    void raise() override;
    void lower() override;
    void addSettingsData(const ReceivedData& received_data,
                         const PlotObjectAttributes& plot_object_attributes,
                         const PropertiesData& properties_data);
    void show() override;
    void hide() override;
    void destroy() override;
    void refresh() override;
    void setMouseInteractionType(const MouseInteractionType mit) override;
    void keyPressed(const char key) override;
    void keyReleased(const char key) override;
    // void showLegend(const bool show_legend) override;
    void waitForFlush() override;
    void toggleProjectionMode() override;
    void update() override;
    void keyPressedCallback(wxKeyEvent& evt);
    void keyReleasedCallback(wxKeyEvent& evt);
    void mouseRightPressed(wxMouseEvent& event);
    void mouseRightReleased(wxMouseEvent& event);
    void mouseMiddlePressed(wxMouseEvent& event);
    void mouseMiddleReleased(wxMouseEvent& event);
    void setMinimumXPos(const int new_min_x_pos);

    // Event callback function
    void mouseMoved(wxMouseEvent& event);
    void mouseEntered(wxMouseEvent& event);
    void mouseLeftWindow(wxMouseEvent& event);
    void mouseLeftPressed(wxMouseEvent& event);
    void mouseLeftReleased(wxMouseEvent& event);

    void bindCallbacks();
};

#endif  // MAIN_APPLICATION_PLOT_PANE_H_
