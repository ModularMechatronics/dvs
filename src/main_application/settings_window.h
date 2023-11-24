#ifndef SETTINGS_WINDOW_H
#define SETTINGS_WINDOW_H

#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/frame.h>
#include <wx/menu.h>
#include <wx/notebook.h>
#include <wx/textctrl.h>
#include <wx/timer.h>
#include <wx/wx.h>

#include <map>
#include <mutex>
#include <string>
#include <thread>
#include <tuple>
#include <utility>
#include <vector>

class SettingsWindow : public wxDialog
{
private:
    wxTextCtrl* editable_text_handle_string_;
    std::map<std::string, wxTextCtrl*> editable_text_generic_fields_map_;

public:
    using FieldsType = std::map<std::string, std::pair<std::string, std::string>>;

    SettingsWindow() = delete;
    SettingsWindow(wxFrame* parent, const std::string& some_string, const FieldsType& fields);

    std::string getHandleString() const;

    std::string getFieldString(const std::string& field_name) const;
};

#endif
