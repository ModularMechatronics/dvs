#ifndef GUI_PANE_H
#define GUI_PANE_H

#include <wx/glcanvas.h>
#include <wx/notebook.h>
#include <wx/wx.h>

#include <atomic>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <OpenGL/gl3.h>

#include "shader.h"
#include "gui_element.h"
#include "vertex_buffer.h"

using namespace duoplot;

class GuiPane : public wxGLCanvas
{
private:
    wxGLContext* getContext();
    wxGLContext* m_context;

    ShaderCollection shader_collection_;

    wxGLAttributes getGLAttributes() const;

    void initShaders();

    glm::mat4 orth_projection_mat_;
    glm::mat4 persp_projection_mat_;
    glm::mat4 projection_mat_;
    glm::mat4 view_mat_;
    glm::mat4 model_mat_;
    glm::mat4 scale_mat_;
    glm::mat4 window_scale_mat_;

    Vec3d scale_vector_;
    Vec3d orth_scale_vector_;
    Vec3d persp_scale_vector_;

    Matrix<double> rot_mat;

    wxTimer receive_timer_;

    void TimerFunc(wxTimerEvent&);
    GuiElement* gui_element_;

public:
    GuiPane(wxFrame* parent);
    ~GuiPane();

    void render(wxPaintEvent& evt);
    void refresh();

};

#endif  // MAIN_APPLICATION_PLOT_PANE_H_
