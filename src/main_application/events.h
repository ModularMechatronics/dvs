#ifndef MAIN_APPLICATION_EVENTS_H_
#define MAIN_APPLICATION_EVENTS_H_

#include <wx/wx.h>

wxDECLARE_EVENT(EDIT_EVENT, wxCommandEvent);
wxDECLARE_EVENT(NO_ELEMENT_SELECTED, wxCommandEvent);
wxDECLARE_EVENT(CHILD_WINDOW_IN_FOCUS_EVENT, wxCommandEvent);
wxDECLARE_EVENT(NEW_EVENT, wxCommandEvent);

namespace dvs_ids
{
enum DvsIds : uint16_t
{
    EDITING_DONE = wxID_HIGHEST,
    ADD_TAB,
    EDIT_LAYOUT,
    PREFERENCES,
    SHOW_MAIN_WINDOW,
    TOGGLE_WINDOW_VISIBILITY,
    EDIT_WINDOW_NAME,
    DELETE_WINDOW,
    NEW_WINDOW,
    NEW_TAB,
    NEW_ELEMENT,
    EDIT_ELEMENT_NAME,
    DELETE_ELEMENT,
    TOGGLE_PROJECTION_TYPE,
    RAISE_ELEMENT,
    LOWER_ELEMENT,
    EDIT_TAB_NAME,
    DELETE_TAB,
    WINDOW_TOGGLE = wxID_HIGHEST + 1000,
    NEW_EVENT_ID
};

}

#endif  // MAIN_APPLICATION_EVENTS_H_
