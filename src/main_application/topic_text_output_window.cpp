#include "topic_text_output_window.h"

#include <ostream>

#include "color.h"
#include "project_state/project_settings.h"
#include "tab_button.h"

namespace
{
wxColour ColorToWxColour(const Color_t col)
{
    switch (col)
    {
        case Color_t::RED:
            return wxColour(255, 0, 0);
        case Color_t::GREEN:
            return wxColour(0, 255, 0);
        case Color_t::BLUE:
            return wxColour(0, 0, 255);
        case Color_t::YELLOW:
            return wxColour(255, 255, 0);
        case Color_t::ORANGE:
            return wxColour(255, 128, 0);
        case Color_t::PURPLE:
            return wxColour(128, 0, 255);
        case Color_t::PINK:
            return wxColour(255, 0, 255);
        case Color_t::CYAN:
            return wxColour(0, 255, 255);
        case Color_t::WHITE:
            return wxColour(255, 255, 255);
        case Color_t::BLACK:
            return wxColour(0, 0, 0);
        case Color_t::GRAY:
            return wxColour(128, 128, 128);
        case Color_t::BROWN:
            return wxColour(128, 64, 0);
        case Color_t::LIGHT_GREEN:
            return wxColour(128, 255, 128);
        case Color_t::LIGHT_ORANGE:
            return wxColour(255, 192, 128);
        default:
            throw std::runtime_error("ColorToWxColour: Unknown input color!");
            return wxColour(0, 0, 0);
    }
}

}  // namespace

TopicTextOutputWindow::TopicTextOutputWindow()
    : wxFrame(nullptr, wxID_ANY, "Output", wxDefaultPosition, wxSize(800, 600))
{
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    text_ctrl_ = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
    sizer->Add(text_ctrl_, 1, wxEXPAND);
    this->SetSizer(sizer);
    this->Layout();
    this->Centre(wxBOTH);

    const wxFont fnt{11, wxFontFamily::wxFONTFAMILY_TELETYPE, wxNORMAL, wxNORMAL};
    text_ctrl_->SetFont(fnt);

    // text_ctrl_->SetBackgroundColour(RGBTripletfToWxColour(kMainWindowBackgroundColor));

    SetBackgroundColour(RGBTripletfToWxColour(kMainWindowBackgroundColor));

    Bind(wxEVT_SIZE, &TopicTextOutputWindow::OnSize, this);
    Bind(wxEVT_CLOSE_WINDOW, &TopicTextOutputWindow::OnClose, this);
}

void TopicTextOutputWindow::pushNewText(const Color_t col, const std::string& text)
{
    text_ctrl_->SetDefaultStyle(ColorToWxColour(col));
    std::ostream stream(text_ctrl_);
    stream << text;
    stream.flush();
}

void TopicTextOutputWindow::OnClose(wxCloseEvent& event)
{
    Hide();
}

void TopicTextOutputWindow::clear()
{
    text_ctrl_->Clear();
}

void TopicTextOutputWindow::OnSize(wxSizeEvent& event)
{
    wxFrame::OnSize(event);
    // const wxSize new_size = event.GetSize();
}

TopicTextOutputWindow::~TopicTextOutputWindow() {}
