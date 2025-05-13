#pragma once

#include <wx/wx.h>
#include "StandardCalcPanel.h"

class MainFrame : public wxFrame
{
private:
	StandardCalcPanel* standardcalcpanel;

public:
	MainFrame(const wxString& title);
};
