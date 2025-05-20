#include "MainFrame.h"

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title)
{
	MainPanel = new wxPanel(this);

	Font = wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	Font.SetFaceName("Segoe UI");

	Input = new wxTextCtrl(MainPanel, wxID_ANY, "", wxDefaultPosition, wxSize(400, 100));
	Input->SetEditable(false);
	Input->SetFont(Font);

	standardcalcpanel = new StandardCalcPanel(MainPanel, Input, Font);

	TypepanelName = new wxStaticText(MainPanel, wxID_ANY, "Standard", wxDefaultPosition, wxSize(400, 50));
	TypepanelName->SetFont(Font);

	this->Bind(wxEVT_CHAR_HOOK, &MainFrame::OnKeysPressed, this);

	MainSizer = new wxBoxSizer(wxVERTICAL);
	SetMainSizer();

	this->SetFocus();
}

void MainFrame::SetMainSizer()
{
	MainSizer->Add(TypepanelName, wxSizerFlags().Expand().Proportion(0).Border(wxLEFT | wxTOP | wxRIGHT, 5));
	MainSizer->Add(Input, wxSizerFlags().Expand().Proportion(0).Border(wxALL, 5));
	MainSizer->Add(standardcalcpanel, wxSizerFlags().Expand().Proportion(1).Border(wxALL, 5));

	MainPanel->SetSizer(MainSizer);
	MainSizer->SetSizeHints(this);
}