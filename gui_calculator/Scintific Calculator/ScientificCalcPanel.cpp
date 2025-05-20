#include "ScientificCalcPanel.h"

ScientificCalcPanel::ScientificCalcPanel(wxFrame& mainframe, wxTextCtrl& Input, wxFont& Font)
{
	this->MainFrame = &mainframe;
	this->input = &Input;
	this->font = &Font;

}