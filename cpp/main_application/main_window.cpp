#include "main_window.h"

#include <unistd.h>

#include <csignal>
#include <iostream>
#include <stdexcept>

#include "math/math.h"

using namespace dvs::internal;

std::string getProjectFilePath()
{
    return "../../project_files/exp1.dvs.json";
}


void MainWindow::setupGui()
{
    const int nx = 13;
    const int ny = 9;
    int dx = 1500 / nx;
    int dy = 700 / ny;
    int xpos = 0;
    int ypos = 0;

    // x: 1500, y: 700
    for(int x = 0; x < nx; x++)
    {
        ypos = 0;
        for(int y = 0; y < ny; y++)
        {
            const wxPoint pos(xpos, ypos);
            const wxSize size(dx, dy);
            
            ResizablePanel* panel = new ResizablePanel(this, pos, size);
            panel->setBackgroundColour(wxColor(((10 * y) % 255), ((10 * x) % 255), (((x + y) * 10) * x) % 255));
            panels_.push_back(panel);
            ypos += dy;
        }
        xpos += dx;
    }
    /*std::vector<int> x = {0, 350, 2 * 350, 3 * 350};
    int idx = 0;
    for(auto e : project_file_.getElements())
    {
        const std::string name = e->getName();
        gui_elements_[name] = new GuiElement(this, wxPoint(x[idx], 0), wxSize(350, 350), name);
        idx++;
    }*/
}

MainWindow::~MainWindow()
{
    std::map<std::string, GuiElement*>::iterator it;

    for (it = gui_elements_.begin(); it != gui_elements_.end(); it++)
    {
        delete it->second;
    }
}

void MainWindow::OnSize(wxSizeEvent& event)
{
    /*for(auto it : gui_elements_)
    {
        it.second->windowSizeChanged(event);
    }*/

    wxSize new_size = event.GetSize();
    const int nx = 13;
    const int ny = 9;
    float dx = static_cast<float>(new_size.GetWidth()) / static_cast<float>(nx);
    float dy = static_cast<float>(new_size.GetHeight() - 30) / static_cast<float>(ny);
    float xpos = 0;
    float ypos = 0;

    // x: 1500, y: 700
    for(int x = 0; x < nx; x++)
    {
        ypos = 0;
        for(int y = 0; y < ny; y++)
        {
            const int idx = x + nx * y;
            const wxPoint pos(std::round(xpos), std::round(ypos));
            const wxSize size(dx, dy);
            panels_.at(idx)->setSize(pos, size);
            
            // ResizablePanel* panel = new ResizablePanel(this, pos, size);
            // panel->setBackgroundColour(wxColor(100, 100, (((x + y) * 10) * x) % 255));
            // panels_.push_back(panel);
            ypos += dy;
        }
        xpos += dx;
    }
}

MainWindow::MainWindow(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title, wxPoint(30, 30), wxSize(1500, 700)), project_file_(getProjectFilePath())
{
    udp_server_ = new UdpServer(9752);
    udp_server_->start();
    current_gui_element_ = nullptr;
    current_gui_element_set_ = false;

    setupGui();

    Bind(wxEVT_SIZE, &MainWindow::OnSize, this);
    // SplashScreen?
    // wxFrame *frame = new wxFrame(NULL, wxID_ANY, "Something", wxPoint(300, 300), wxSize(200, 200), wxFRAME_TOOL_WINDOW | wxNO_BORDER);
    // frame->Show(true);

    timer_.Bind(wxEVT_TIMER, &MainWindow::OnTimer, this);
    timer_.Start(10);
}

void MainWindow::OnTimer(wxTimerEvent&)
{
    try
    {
        receiveData();
    }
    catch(const std::runtime_error& e)
    {
        std::cerr << "Got runtime_error when receiving: " << e.what() << std::endl;
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
