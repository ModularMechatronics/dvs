#ifndef SHADER_APP_GL_CANVAS_H_
#define SHADER_APP_GL_CANVAS_H_

#include <wx/glcanvas.h>
#include <wx/notebook.h>
#include <wx/wx.h>

#include "axes/axes.h"
#include "axes_renderer.h"
#include "communication/received_data.h"
#include "communication/udp_server.h"
#include "dvs/dvs.h"
#include "gui_element.h"
#include "io_devices/io_devices.h"
#include "opengl_low_level/opengl_header.h"
#include "plot_data.h"
#include "view_base.h"
#include "vbo_wrapper.h"
#include "shader.h"
#include "plot_box_walls.h"
#include "plot_box_silhouette.h"
// #include "plot_box_grid.h"

class GlCanvas : public wxGLCanvas
{
private:
    wxGLContext* m_context;

    int args[9];
    Shader shader_;
    unsigned int vertex_buffer_, vertex_buffer_array_;
    GLuint colorbuffer;
    bool use_perspective_proj_;
    VboWrapper3D cube_;

    int* getArgsPtr()
    {
        args[0] = WX_GL_RGBA;
        args[1] = WX_GL_DOUBLEBUFFER;
        args[2] = WX_GL_DEPTH_SIZE;
        args[3] = 16;
        args[4] = WX_GL_SAMPLES;
        args[5] = 4;
        args[6] = WX_GL_SAMPLE_BUFFERS;
        args[7] = 1;
        args[8] = 0;
    
        return args;
    }
    MouseActivity current_mouse_activity_;
    MouseInteractionAxis current_mouse_interaction_axis_;
    AxesInteractor* axes_interactor_;
    AxesRenderer* axes_renderer_;
    AxesSettings axes_settings_;
    MouseButtonState left_mouse_button_;
    KeyboardState keyboard_state_;

    void keyPressed(wxKeyEvent& event);
    void keyReleased(wxKeyEvent& event);
    void mouseMoved(wxMouseEvent& event);
    void mouseLeftWindow(wxMouseEvent& event);
    void mouseLeftPressed(wxMouseEvent& event);
    void mouseLeftReleased(wxMouseEvent& event);
    void reloadShader();

    int getWidth();
    int getHeight();

public:
    GlCanvas(wxNotebookPage* parent);
    ~GlCanvas();

    void render(wxPaintEvent& evt);
    void show();
    void setSize(const wxSize& new_size);

};

std::string getGLErrorString(const GLenum err);
InteractionType keyboardStateToInteractionTypeNew(const KeyboardState& keyboard_state);

#endif
