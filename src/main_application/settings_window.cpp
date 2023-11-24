#include "settings_window.h"

SettingsWindow::SettingsWindow(wxFrame* parent,
                               const std::string& some_string,
                               const std::map<std::string, std::string>& fields)
    : wxDialog{
          parent, wxID_ANY, some_string, wxPoint(100, 100), wxSize(200, 200), wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER}
{
    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    wxSizer* empty_header_subsizer = new wxBoxSizer(wxHORIZONTAL);
    empty_header_subsizer->Add(new wxStaticText(this, wxID_ANY, ""), 0, wxALIGN_CENTER_VERTICAL);

    sizer->Add(empty_header_subsizer, 0, wxALL | wxEXPAND, 10);

    for (const auto& p : fields)
    {
        wxSizer* sub_sizer = new wxBoxSizer(wxHORIZONTAL);
        wxTextCtrl* editable_text_field = new wxTextCtrl(this, wxID_ANY, p.second);
        sub_sizer->Add(new wxStaticText(this, wxID_ANY, p.first), 0, wxALIGN_CENTER_VERTICAL);
        sub_sizer->Add(editable_text_field, 0, wxALIGN_CENTER_VERTICAL);
        sizer->Add(sub_sizer, 0, wxALL | wxEXPAND, 10);
        editable_text_generic_fields_map_[p.first] = editable_text_field;
    }

    wxButton* ok = new wxButton(this, wxID_OK, "Ok");
    wxButton* cancel = new wxButton(this, wxID_CANCEL, "Cancel");
    ok->SetDefault();

    wxSizer* ok_cancel_buttons_sizer = new wxBoxSizer(wxHORIZONTAL);
    ok_cancel_buttons_sizer->Add(ok, 0, wxALIGN_CENTER_VERTICAL);
    ok_cancel_buttons_sizer->Add(cancel, 0, wxALIGN_CENTER_VERTICAL);

    sizer->Add(ok_cancel_buttons_sizer, 0, wxALIGN_RIGHT | wxALL, 10);

    SetSizerAndFit(sizer);
}

std::string SettingsWindow::getFieldString(const std::string& field_name) const
{
    return editable_text_generic_fields_map_.at(field_name)->GetValue().ToStdString();
}
