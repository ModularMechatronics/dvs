#include "main_window.h"

#include <unistd.h>

#include <csignal>
#include <iostream>

#include "math/math.h"

using namespace dvs::internal;

std::string getProjectFilePath()
{
    return "../../project_files/exp1.dvs.json";
}

void MainWindow::setupGui()
{
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    for(auto e : project_file_.getElements())
    {
        const std::string name = e->getName();
        wxPanel *panel = new wxPanel(this, wxID_ANY);
        gui_elements_[name] = new GuiElement(panel, wxSize(150, 150), name);
        sizer->Add(panel);
    }

    /*wxPanel *panel_00 = new wxPanel(this, wxID_ANY);
    GuiElement* gl_pane_00 = new GuiElement(panel_00, wxSize(150, 150), true, true);

    wxPanel *panel_10 = new wxPanel(this, wxID_ANY);
    GuiElement* gl_pane_10 = new GuiElement(panel_10, wxSize(150, 150), true, true);

    wxPanel *panel_01 = new wxPanel(this, wxID_ANY);
    GuiElement* gl_pane_01 = new GuiElement(panel_01, wxSize(150, 150), true, true);

    wxPanel *panel_11 = new wxPanel(this, wxID_ANY);
    GuiElement* gl_pane_11 = new GuiElement(panel_11, wxSize(150, 150), true, true);

    wxPanel *panel_2 = new wxPanel(this, wxID_ANY);
    GuiElement* gl_pane_2 = new GuiElement(panel_2, wxSize(300, 150), true, true);

    wxPanel *panel_3 = new wxPanel(this, wxID_ANY);
    GuiElement* gl_pane_3 = new GuiElement(panel_3, wxSize(150, 450), true, true);

    gui_elements_.push_back(gl_pane_00);
    gui_elements_.push_back(gl_pane_10);
    gui_elements_.push_back(gl_pane_01);
    gui_elements_.push_back(gl_pane_11);
    gui_elements_.push_back(gl_pane_2);
    gui_elements_.push_back(gl_pane_3);*/


    /*wxBoxSizer* sizer0 = new wxBoxSizer(wxHORIZONTAL);
    sizer0->Add(panel_00, 0, wxALL, 5);
    sizer0->Add(panel_10, 0, wxALL, 5);

    wxBoxSizer* sizer1 = new wxBoxSizer(wxHORIZONTAL);
    sizer1->Add(panel_01, 0, wxALL, 5);
    sizer1->Add(panel_11, 0, wxALL, 5);

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer2 = new wxBoxSizer(wxHORIZONTAL);

    sizer->Add(sizer0, 0, wxALL, 5);
    sizer->Add(sizer1, 0, wxALL, 5);
    sizer->Add(panel_2, 0, wxALL, 5);

    sizer2->Add(sizer, 0, wxALL, 5);
    sizer2->Add(panel_3);*/

    SetSizer(sizer);
}

MainWindow::~MainWindow()
{
    std::map<std::string, GuiElement*>::iterator it;

    for (it = gui_elements_.begin(); it != gui_elements_.end(); it++)
    {
        delete it->second;
    }
}

MainWindow::MainWindow(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title, wxPoint(30, 30), wxSize(700, 700)), project_file_(getProjectFilePath())
{
    udp_server_ = new UdpServer(9752);
    udp_server_->start();
    current_gui_element_ = nullptr;
    current_gui_element_set_ = false;

    setupGui();

    timer_.Bind(wxEVT_TIMER, &MainWindow::OnTimer, this);
    timer_.Start(10);
}

void MainWindow::setCurrentElement(const FunctionHeader& hdr)
{
    const FunctionHeaderObject elem_obj = hdr.getObjectFromType(FunctionHeaderObjectType::ELEMENT_NAME);
    const properties::Name elem_name = elem_obj.getAs<properties::Name>();
    const std::string element_name_str = elem_name.data;

    if(gui_elements_.count(element_name_str) > 0)
    {
        std::cout << "Current element name: " << element_name_str << std::endl;
        current_gui_element_ = gui_elements_[element_name_str];
    }
    else
    {
        std::cout << "I don't have name: " << element_name_str << std::endl;
    }
}

void MainWindow::OnTimer(wxTimerEvent&)
{
    try
    {
        std::unique_ptr<const ReceivedData> received_data = udp_server_->getReceivedData();
        if(received_data)
        {
            const FunctionHeader hdr = received_data->getFunctionHeader();

            const FunctionHeaderObject fcn_obj = hdr.getObjectAtIdx(0);
            if(fcn_obj.type != FunctionHeaderObjectType::FUNCTION)
            {
                std::cout << "Function was not at element 0!" << std::endl;
                return;
            }

            const Function fcn = fcn_obj.getAs<Function>();

            switch(fcn)
            {
                case Function::SET_CURRENT_ELEMENT:
                    setCurrentElement(hdr);

                    break;
                default:
                    std::cout << "Got default" << std::endl;
            }
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << "Got exception when receiving: " << e.what() << std::endl;
    }
    
    
}

void MainWindow::OnClose(wxCloseEvent& event)
{
    (void)event;
    if (wxMessageBox("Are you sure you want to exit?",
                     "Please confirm",
                     wxICON_QUESTION | wxYES_NO) != wxYES)
    {
        return;
    }

    std::cout << "Window close" << std::endl;
    Destroy();
}
