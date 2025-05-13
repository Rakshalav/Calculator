#include "App.h"
#include "MainFrame.h"
#include <wx/wx.h>
#include <cstdint>
#include <wx/icon.h>

wxIMPLEMENT_APP(App);

const uint16_t width = 405, height = 700;

bool App::OnInit()
{
    // Load the icon
    wxIcon appIcon;
    if (!appIcon.LoadFile("C:\\Users\\ragha\\OneDrive\\Desktop\\C++\\gui_calculator\\gui_calculator\\calculator_icon.ico", wxBITMAP_TYPE_ICO)) {
        return false;
    }

    // Create the main frame
    MainFrame* mainFrame = new MainFrame("Calculator");
    mainFrame->SetClientSize(width, height);
    mainFrame->Center();
    mainFrame->Show();

    // Set the icon for the window
    mainFrame->SetIcon(appIcon);

    return true;
}