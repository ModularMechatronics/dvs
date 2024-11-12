#ifndef GUI_PANE_H
#define GUI_PANE_H

#include <OpenGL/gl3.h>
#include <wx/glcanvas.h>
#include <wx/notebook.h>
#include <wx/wx.h>

#include <atomic>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <memory>
#include <vector>

#include "button.h"
#include "checkbox.h"
#include "gui_element.h"
#include "shader.h"
#include "slider.h"
#include "vertex_buffer.h"

using namespace duoplot;

enum EventIds : uint16_t
{
    BRING_TO_FRONT = wxID_HIGHEST,
    SEND_TO_BACK,
    BRING_FORWARD,
    SEND_BACKWARD
};

enum class InteractionState : uint8_t
{
    EditingObject,
    Interacting,
    Hoovering,
    Dragging
};

class GuiPane : public wxGLCanvas
{
private:
    wxGLContext* getContext();
    wxGLContext* m_context_;

    wxPoint previous_mouse_position_;

    ShaderCollection shader_collection_;
    InteractionState interaction_state_;
    ChangeDirection change_direction_at_press_;

    bool control_pressed_;
    bool left_mouse_pressed_;
    bool right_mouse_pressed_;

    std::function<void(RGBTripletf)> set_shader_color_;

    wxTimer update_timer_;

    std::vector<std::shared_ptr<GuiElement>> gui_elements_;
    std::shared_ptr<GuiElement> current_hovered_element_;
    std::shared_ptr<GuiElement> current_pressed_element_;
    std::shared_ptr<GuiElement> right_clicked_element_;

    wxMenu* popup_menu_;

    wxGLAttributes getGLAttributes() const;

    void initShaders();

    void TimerFunc(wxTimerEvent&);

    void mouseLeftDoubleClicked(wxMouseEvent& event);
    void mouseLeftPressed(wxMouseEvent& event);
    void mouseLeftReleased(wxMouseEvent& event);
    void mouseMoved(wxMouseEvent& event);
    void mouseRightPressed(wxMouseEvent& event);
    void mouseRightReleased(wxMouseEvent& event);
    void mouseEntered(wxMouseEvent& event);
    void mouseExited(wxMouseEvent& event);

    void keyPressedCallback(wxKeyEvent& evt);
    void keyReleasedCallback(wxKeyEvent& evt);
    void setCursor(const ChangeDirection change_direction);
    void sendElementToBackCallback(wxCommandEvent& WXUNUSED(event));
    void bringElementToFrontCallback(wxCommandEvent& WXUNUSED(event));
    void bringElementForwardCallback(wxCommandEvent& WXUNUSED(event));
    void sendElementBackwardCallback(wxCommandEvent& WXUNUSED(event));

    void OnSize(wxSizeEvent& event);

public:
    GuiPane(wxFrame* parent);
    ~GuiPane();

    void UpdateSizeFromParent(const wxSize new_size);

    void render(wxPaintEvent& evt);
    void refresh();
};

#endif  // MAIN_APPLICATION_PLOT_PANE_H_
