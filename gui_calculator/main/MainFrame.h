#pragma once  

#include <wx/wx.h>  
#include "../Standard Calculator/StandardCalcPanel.h"  

class MainFrame : public wxFrame  
{  
private:  
   StandardCalcPanel* standardcalcpanel;

   //input
   wxTextCtrl* Input;
   
   wxFont Font;

   wxBoxSizer* MainSizer;

   wxPanel* MainPanel;

   wxStaticText* TypepanelName;

   void SetMainSizer();
   void OnKeysPressed(wxKeyEvent& event) { standardcalcpanel->HandlerProxyKeyDown(event); }

public:  
   MainFrame(const wxString& title);  
};
