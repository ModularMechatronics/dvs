#ifndef DVS_EVENTS_H_
#define DVS_EVENTS_H_

#include <wx/wx.h>

wxDECLARE_EVENT(MY_EVENT, wxCommandEvent);
wxDECLARE_EVENT(EDIT_EVENT, wxCommandEvent);
wxDECLARE_EVENT(GUI_ELEMENT_CHANGED_EVENT, wxCommandEvent);
wxDECLARE_EVENT(CHILD_WINDOW_CLOSED_EVENT, wxCommandEvent);
wxDECLARE_EVENT(CHILD_WINDOW_IN_FOCUS_EVENT, wxCommandEvent);

namespace dvs_ids
{
enum DvsIds : uint16_t
{
    EDITING_DONE = wxID_HIGHEST,
    DELETE_TAB,
    ADD_TAB,
    EDIT_LAYOUT,
    TOGGLE_WINDOW_VISIBILITY,
    WINDOW_TOGGLE = wxID_HIGHEST + 1000
};

}

#endif
