#pragma once

#include <wx/wx.h>

class ScientificCalcPanel : public wxPanel
{
private:
	wxFrame* MainFrame;
	wxTextCtrl* input;
	wxFont* font;

public:
	ScientificCalcPanel(wxFrame& mainframe, wxTextCtrl& input, wxFont& Font);
};

