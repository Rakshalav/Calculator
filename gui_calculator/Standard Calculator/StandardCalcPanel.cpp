#include "StandardCalcPanel.h"
#include "StandardCalcLogic.h"

StandardCalcPanel::StandardCalcPanel(wxWindow* parent, wxTextCtrl* Input, wxFont& Font) : wxPanel(parent, wxID_ANY)
{
	this->input = Input;
	this->font = &Font;
	this->font->SetFaceName("Segoe UI");

	this->Bind(wxEVT_SIZE, &StandardCalcPanel::OnSize, this);
	this->Bind(wxEVT_IDLE, &StandardCalcPanel::OnIdle, this);

	std::vector<wxButton*> buttons = CreateButtons();
	SetUpSizers(buttons, *font);
}

void StandardCalcPanel::SetUpSizers(std::vector<wxButton*> Buttons, wxFont font)
{
	wxGridSizer* ButtonsSizer = new wxGridSizer(6, 4, wxSize(5, 5));
	wxSizerFlags flags = wxSizerFlags().Expand();

	for (wxButton* Button : Buttons) {
		Button->SetFont(font);
		ButtonsSizer->Add(Button, flags);
	}

	this->SetSizer(ButtonsSizer);
	ButtonsSizer->SetSizeHints(this);
}

std::vector<wxButton*> StandardCalcPanel::CreateButtons()
{
	std::array<wxString, 24> ArrayButtonLabels = {
		Label.BracketOpen, Label.BracketClose, Label.Delete,   Label.ClearAll,
		Label.Root,        Label.Power2,       Label.Exponent, Label.Divide,
		Label.Seven,       Label.Eight,        Label.Nine,     Label.Multiply,
		Label.Four,        Label.Five,         Label.Six,      Label.Add,
		Label.One,         Label.Two,          Label.Three,    Label.Substract,
		Label.Negative,    Label.Zero,         Label.Decimal,  Label.Answer
	};

	std::array<int, 24> ArrayButtonID = {
		ID_BTN_OPENBRACKET, ID_BTN_CLOSEBRACKET, ID_BTN_DELETE,   ID_BTN_CLEARALL,
		ID_BTN_ROOT,        ID_BTN_POWER2,       ID_BTN_EXPONENT, ID_BTN_DIVIDE,
		ID_BTN_SEVEN,       ID_BTN_EIGHT,        ID_BTN_NINE,     ID_BTN_MULTIPLY,
		ID_BTN_FOUR,        ID_BTN_FIVE,         ID_BTN_SIX,      ID_BTN_ADD,
		ID_BTN_ONE,         ID_BTN_TWO,          ID_BTN_THREE,    ID_BTN_SUBTRACT,
		ID_BTN_NEGATIVE,    ID_BTN_ZERO,         ID_BTN_DECIMAL,  ID_BTN_ANSWER
	};

	std::array<wxString, 24> ArrayButtonColors = {
		LightTheme.OperatorKeys, LightTheme.OperatorKeys, LightTheme.OperatorKeys, LightTheme.OperatorKeys,
		LightTheme.OperatorKeys, LightTheme.OperatorKeys, LightTheme.OperatorKeys, LightTheme.OperatorKeys,
		LightTheme.NumberKeys,   LightTheme.NumberKeys,   LightTheme.NumberKeys,   LightTheme.OperatorKeys,
		LightTheme.NumberKeys,   LightTheme.NumberKeys,   LightTheme.NumberKeys,   LightTheme.OperatorKeys,
		LightTheme.NumberKeys,   LightTheme.NumberKeys,   LightTheme.NumberKeys,   LightTheme.OperatorKeys,
		LightTheme.OperatorKeys, LightTheme.NumberKeys,   LightTheme.OperatorKeys, LightTheme.AnswerKey
	};

	std::array<wxButton**, 24> StructButtonFields = {
		&button.OpenBracket, &button.CloseBracket, &button.Delete,    &button.ClearAll,
		&button.Root,        &button.Power2,       &button.Exponent,  &button.Divide,
		&button.Seven,       &button.Eight,        &button.Nine,      &button.Multiply,
		&button.Four,        &button.Five,         &button.Six,       &button.Add,
		&button.One,         &button.Two,          &button.Three,     &button.Subtract,
		&button.Negative,    &button.Zero,         &button.Decimal,   &button.Answer
	};

	std::vector<wxButton*> ArrayButtons;

	for (size_t i = 0; i < ArrayButtonLabels.size(); ++i)
	{
		wxButton* btn = new wxButton(this, ArrayButtonID[i], ArrayButtonLabels[i], wxDefaultPosition, wxSize(Btn_Width_Height, Btn_Width_Height));
		btn->SetBackgroundColour(ArrayButtonColors[i]);
		btn->SetForegroundColour(LightTheme.TextColor);
		btn->Bind(wxEVT_BUTTON, &StandardCalcPanel::OnButtonPressed, this, ArrayButtonID[i]);

		*StructButtonFields[i] = btn;
		ArrayButtons.push_back(btn);
	}

	return ArrayButtons;
}

void StandardCalcPanel::OnButtonPressed(wxCommandEvent& evt)
{
	wxString currentInput = input->GetValue();
	if (CheckErrorMessage(currentInput))
		input->Clear();

	int id = evt.GetId();
	switch (id)
	{
	case StandardCalcPanel::ID_BTN_ZERO:         AppendSymbol(Label.Zero); break;
	case StandardCalcPanel::ID_BTN_ONE:          AppendSymbol(Label.One); break;
	case StandardCalcPanel::ID_BTN_TWO:          AppendSymbol(Label.Two); break;
	case StandardCalcPanel::ID_BTN_THREE:        AppendSymbol(Label.Three); break;
	case StandardCalcPanel::ID_BTN_FOUR:         AppendSymbol(Label.Four); break;
	case StandardCalcPanel::ID_BTN_FIVE:         AppendSymbol(Label.Five); break;
	case StandardCalcPanel::ID_BTN_SIX:          AppendSymbol(Label.Six); break;
	case StandardCalcPanel::ID_BTN_SEVEN:        AppendSymbol(Label.Seven); break;
	case StandardCalcPanel::ID_BTN_EIGHT:        AppendSymbol(Label.Eight); break;
	case StandardCalcPanel::ID_BTN_NINE:         AppendSymbol(Label.Nine); break;

	case StandardCalcPanel::ID_BTN_EXPONENT:     AppendSymbol(Label.Exponent); break;
	case StandardCalcPanel::ID_BTN_POWER2:       AppendSymbol(Text.Power2); break;
	case StandardCalcPanel::ID_BTN_ROOT:         SQRT_button(); break;

	case StandardCalcPanel::ID_BTN_DIVIDE:       AppendSymbol(Label.Divide); break;
	case StandardCalcPanel::ID_BTN_MULTIPLY:     AppendSymbol(Label.Multiply); break;
	case StandardCalcPanel::ID_BTN_ADD:          AppendSymbol(Label.Add); break;
	case StandardCalcPanel::ID_BTN_SUBTRACT:     AppendSymbol(Label.Substract); break;

	case StandardCalcPanel::ID_BTN_NEGATIVE:     AppendSymbol(Text.Negative); break;
	case StandardCalcPanel::ID_BTN_DELETE:       OnBtnDel_pressed(); break;
	case StandardCalcPanel::ID_BTN_CLEARALL:     input->Clear(); break;
	case StandardCalcPanel::ID_BTN_ANSWER:
		if (CheckErrorMessage(currentInput)) { input->Clear(); }
		else { GetAnswer(); }
		break;
	case StandardCalcPanel::ID_BTN_DECIMAL:      AppendSymbol(Label.Decimal); break;

	case StandardCalcPanel::ID_BTN_OPENBRACKET:  AppendSymbol(Label.BracketOpen); break;
	case StandardCalcPanel::ID_BTN_CLOSEBRACKET: AppendSymbol(Label.BracketClose); break;

	default:
		break;
	}
}

void StandardCalcPanel::OnBackSpace_pressed()
{
	long from, to;
	input->GetSelection(&from, &to);

	if (from != to)
	{
		input->Remove(from, to);
		input->SetInsertionPoint(from);
	}
	else if (from > 0)
	{
		input->Remove(from - 1, from);
		input->SetInsertionPoint(from - 1);
	}
}

//Fix delete button issues
void StandardCalcPanel::OnBtnDel_pressed() {
	long from, to;
	input->GetSelection(&from, &to);

	if (from != to) {
		input->Remove(from, to);
		input->SetInsertionPoint(from);
	}
	else if (from > 0) {
		input->Remove(from - 1, from);
		input->SetInsertionPoint(from - 1);
	}
	else {
		input->Remove(from + 1, from);
		input->SetInsertionPoint(from);
	}
}

void StandardCalcPanel::SQRT_button()
{
	long pos = input->GetInsertionPoint();
	input->WriteText(Text.Root);
	input->SetInsertionPoint(pos + 5);
}

void StandardCalcPanel::GetAnswer()
{
	wxString equation = input->GetValue();

	if (!equation.IsEmpty()) {
		StandardCalcLogic calc(std::string(equation.mb_str()));
		std::string answer = calc.Answer();
		wxString result = wxString::FromUTF8(answer);

		input->Clear();
		input->SetValue(result);
		input->SetInsertionPointEnd();
	}
}

// fix: delete key issues
void StandardCalcPanel::Delete()
{
	long from, to;
	input->GetSelection(&from, &to);

	if (from != to || from > 0) {
		input->Remove(from - 1, from);
	}
	input->SetInsertionPoint(from);
}

void StandardCalcPanel::AppendSymbol(wxString& Symbol)
{
	uint8_t chars = 40;
	if (input->GetValue().Length() >= chars)
		return;

	input->WriteText(Symbol);
	input->SetInsertionPoint(input->GetInsertionPoint());
	AdjustInputFontSize();
}

void StandardCalcPanel::OnKeyDown(wxKeyEvent& evt)
{
	if (CheckErrorMessage(input->GetValue()))
		input->Clear();

	int KeyCode = evt.GetKeyCode();
	bool shiftPressed = evt.ShiftDown();

	switch (KeyCode)
	{
	case '0':
		if (shiftPressed)
			AppendSymbol(Label.BracketClose);
		else
			AppendSymbol(Label.Zero);
		break;
	case '1': AppendSymbol(Label.One); break;
	case '2': AppendSymbol(Label.Two); break;
	case '3': AppendSymbol(Label.Three); break;
	case '4': AppendSymbol(Label.Four); break;
	case '5': AppendSymbol(Label.Five); break;
	case '6':
		if (shiftPressed)
			AppendSymbol(Label.Exponent);
		else
			AppendSymbol(Label.Six);
		break;
	case '7': AppendSymbol(Label.Seven); break;
	case '8':
		if (shiftPressed)
			AppendSymbol(Label.Multiply);
		else
			AppendSymbol(Label.Eight);
		break;
	case '9':
		if (shiftPressed)
			AppendSymbol(Label.BracketOpen);
		else
			AppendSymbol(Label.Nine);
		break;

	case '/': AppendSymbol(Label.Divide); break;
	case '=':
		if (shiftPressed)
			AppendSymbol(Label.Add);
		break;
	case '-': AppendSymbol(Label.Substract); break;
	case '.': AppendSymbol(Label.Decimal); break;

		// Numpad keys
	case WXK_NUMPAD0: AppendSymbol(Label.Zero); break;
	case WXK_NUMPAD1: AppendSymbol(Label.One); break;
	case WXK_NUMPAD2: AppendSymbol(Label.Two); break;
	case WXK_NUMPAD3: AppendSymbol(Label.Three); break;
	case WXK_NUMPAD4: AppendSymbol(Label.Four); break;
	case WXK_NUMPAD5: AppendSymbol(Label.Five); break;
	case WXK_NUMPAD6: AppendSymbol(Label.Six); break;
	case WXK_NUMPAD7: AppendSymbol(Label.Seven); break;
	case WXK_NUMPAD8: AppendSymbol(Label.Eight); break;
	case WXK_NUMPAD9: AppendSymbol(Label.Nine); break;

	case WXK_NUMPAD_DIVIDE:    AppendSymbol(Label.Divide); break;
	case WXK_NUMPAD_MULTIPLY:  AppendSymbol(Label.Multiply); break;
	case WXK_NUMPAD_ADD:       AppendSymbol(Label.Add); break;
	case WXK_NUMPAD_SUBTRACT:  AppendSymbol(Label.Substract); break;
	case WXK_NUMPAD_DECIMAL:   AppendSymbol(Label.Decimal); break;
	case WXK_NUMPAD_ENTER:     GetAnswer(); break;

	case WXK_BACK:   OnBackSpace_pressed(); break;
	case WXK_DELETE: Delete(); break;
	case WXK_RETURN: GetAnswer(); break;

	default: evt.Skip(); break;
	}
}

bool StandardCalcPanel::CheckErrorMessage(wxString ErrorMessages)
{
	if (ErrorMessages == "Syntax error!" || ErrorMessages == "Math Error: Square root of negative number!" ||
		ErrorMessages == "Math Error: Division by zero!" || ErrorMessages == "Unknown error!")
		return true;
	return false;
}

void StandardCalcPanel::AdjustInputFontSize()
{
	const int MAX_FONT_SIZE = 20;
	const int MIN_FONT_SIZE = 12;

	wxString text = input->GetValue();
	if (text.IsEmpty()) {
		input->SetFont(wxFont(MAX_FONT_SIZE, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
		input->Refresh();
		return;
	}

	wxClientDC dc(input);
	int clientwidth = input->GetClientSize().GetWidth();
	for (int fontSize = MAX_FONT_SIZE; fontSize >= MIN_FONT_SIZE; --fontSize) {
		wxFont Font(fontSize, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
		Font.SetFaceName("Segoe UI");
		dc.SetFont(Font);

		int textWidth, textHeight;
		dc.GetTextExtent(text, &textWidth, &textHeight);

		if (textWidth <= clientwidth) {
			input->SetFont(Font);
			input->Refresh();
			break;
		}
	}
}

void StandardCalcPanel::OnSize(wxSizeEvent& event) {

	AdjustInputFontSize();
	input->Refresh();
	this->Layout();

	event.Skip();
}

void StandardCalcPanel::OnIdle(wxIdleEvent& event)
{
	int currentwidth = input->GetClientSize().GetWidth();
	if (currentwidth != LastInputWidth) {
		AdjustInputFontSize();
		LastInputWidth = currentwidth;
	}
	event.Skip();
}