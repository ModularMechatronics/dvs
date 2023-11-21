#ifndef MAIN_APPLICATION_PLOT_PANE_H_
#define MAIN_APPLICATION_PLOT_PANE_H_

#include <wx/glcanvas.h>
#include <wx/notebook.h>
#include <wx/wx.h>

#include <atomic>

#include "axes/axes.h"
#include "communication/data_receiver.h"
#include "communication/received_data.h"
#include "dvs/internal.h"
#include "dvs/math/math.h"
#include "gui_element.h"
#include "input_data.h"
#include "opengl_low_level/opengl_header.h"
#include "plot_data_handler.h"
#include "point_selection.h"

class PlotPane : public wxGLCanvas, public ApplicationGuiElement
{
private:
    wxGLContext* getContext();
    wxGLContext* m_context;

    AxesSettings axes_settings_;
    AxesInteractor axes_interactor_;
    AxesRenderer* axes_renderer_;

    bool shift_pressed_at_mouse_press_;

    bool axes_from_min_max_disabled_;
    bool axes_set_;
    bool view_set_;

    int args[9];

    bool perspective_projection_;
    bool wait_for_flush_;

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

    void processActionQueue();
    void addPlotData(ReceivedData& received_data,
                     const PlotObjectAttributes& plot_object_attributes,
                     const PropertiesData& properties_data,
                     const std::shared_ptr<const ConvertedDataBase>& converted_data);

    wxGLAttributes getGLAttributes() const;
    std::shared_ptr<PlotPaneSettings> plot_pane_settings_;

    std::uint64_t getGuiPayloadSize() const override
    {
        return 0U;
    }

    void fillGuiPayload(FillableUInt8Array& output_array) const override {}

public:
    PlotPane(wxNotebookPage* parent,  // TODO: wxNotebookPage is obsolete, should be wxWindow/wxFrame?
             const std::shared_ptr<ElementSettings>& element_settings,
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
    void pushQueue(std::queue<std::unique_ptr<InputData>>& new_queue);

    void render(wxPaintEvent& evt);

    void setElementPositionAndSize();
    void setMinXPos(const int min_x_pos) override;
    void setSize(const wxSize& new_size);
    void updateSizeFromParent(const wxSize& parent_size) override;
    void addSettingsData(const ReceivedData& received_data,
                         const PlotObjectAttributes& plot_object_attributes,
                         const PropertiesData& properties_data);
    // void show() override;
    // void hide() override;
    // void destroy() override;
    void refresh();
    void setMouseInteractionType(const MouseInteractionType mit);
    void keyPressed(const char key) override;
    void keyReleased(const char key) override;
    void waitForFlush();
    void toggleProjectionMode();
    void update();
    void keyPressedCallback(wxKeyEvent& evt);
    void keyReleasedCallback(wxKeyEvent& evt);
    void mouseRightPressed(wxMouseEvent& event);
    void mouseRightReleased(wxMouseEvent& event);
    void mouseMiddlePressed(wxMouseEvent& event);
    void mouseMiddleReleased(wxMouseEvent& event);
    void setMinimumXPos(const int new_min_x_pos);

    // Event callback function
    void mouseMovedGuiElementSpecific(wxMouseEvent& event);
    void mouseLeftPressedGuiElementSpecific(wxMouseEvent& event);
    void mouseLeftReleasedGuiElementSpecific(wxMouseEvent& event);

    void bindCallbacks();

    void setCursor(const wxCursor& cursor) override
    {
        wxSetCursor(cursor);
    }

    wxSize getSize() const override
    {
        return this->GetSize();
    }

    wxPoint getPosition() const override
    {
        return this->GetPosition();
    }

    /*virtual void showLegend(const bool show_legend)
    {
        static_cast<void>(show_legend);
        std::cout << "Not implemented!" << std::endl;
    }*/
};

#endif  // MAIN_APPLICATION_PLOT_PANE_H_
