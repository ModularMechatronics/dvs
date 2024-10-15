#ifndef TOPIC_TEXT_OUTPUT_WINDOW_H
#define TOPIC_TEXT_OUTPUT_WINDOW_H

#include <wx/button.h>
#include <wx/frame.h>
#include <wx/menu.h>
#include <wx/notebook.h>
#include <wx/textctrl.h>
#include <wx/timer.h>
#include <wx/window.h>
#include <wx/wx.h>

#include "color.h"

class TopicTextOutputWindow : public wxFrame
{
private:
    wxTextCtrl* text_ctrl_;

public:
    TopicTextOutputWindow();
    ~TopicTextOutputWindow();

    void OnSize(wxSizeEvent& event);
    void OnClose(wxCloseEvent& event);
    void clear();

    void pushNewText(const Color_t col, const std::string& text);
};

#endif  // TOPIC_TEXT_OUTPUT_WINDOW_H
