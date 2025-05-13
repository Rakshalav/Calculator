#pragma once

#include <wx/wx.h>
#include <array>

class StandardCalcPanel : public wxPanel
{
private:
	wxFrame* MainFrame;
	wxTextCtrl* input;
	wxFont* font;
	uint8_t Btn_Width_Height = 95;

	struct Buttons {
		wxButton* Root;        wxButton* Power2;       wxButton* Delete;    wxButton* ClearAll;
		wxButton* OpenBracket; wxButton* CloseBracket; wxButton* Exponent;  wxButton* Divide;
		wxButton* Seven;       wxButton* Eight;        wxButton* Nine;      wxButton* Multiply;
		wxButton* Four;        wxButton* Five;         wxButton* Six;       wxButton* Add;
		wxButton* One;         wxButton* Two;          wxButton* Three;     wxButton* Subtract;
		wxButton* Negative;    wxButton* Zero;         wxButton* Decimal;   wxButton* Answer;
	};
	Buttons button;

	struct ButtonLabels {
		wxString Root = L"√x";      wxString Power2 = "x²";      wxString Delete = "DEL"; wxString ClearAll = "AC";
		wxString BracketOpen = "("; wxString BracketClose = ")"; wxString Exponent = "^"; wxString Divide = "÷";
		wxString Seven = "7";       wxString Eight = "8";        wxString Nine = "9";     wxString Multiply = "×";
		wxString Four = "4";        wxString Five = "5";         wxString Six = "6";      wxString Add = "+";
		wxString One = "1";         wxString Two = "2";          wxString Three = "3";    wxString Substract = "–";
		wxString Negative = "±";    wxString Zero = "0";         wxString Decimal = ".";  wxString Answer = "=";
	};
	ButtonLabels Label;

	struct TextLabels { wxString Power2 = "^2"; wxString Negative = "-"; wxString Root = "sqrt()"; };
	TextLabels Text;

	enum ButtonID {
		ID_BTN_ROOT = wxID_HIGHEST + 1, ID_BTN_POWER2, ID_BTN_DELETE, ID_BTN_CLEARALL,
		ID_BTN_OPENBRACKET, ID_BTN_CLOSEBRACKET, ID_BTN_EXPONENT, ID_BTN_DIVIDE,
		ID_BTN_SEVEN, ID_BTN_EIGHT, ID_BTN_NINE, ID_BTN_MULTIPLY,
		ID_BTN_FOUR, ID_BTN_FIVE, ID_BTN_SIX, ID_BTN_ADD,
		ID_BTN_ONE, ID_BTN_TWO, ID_BTN_THREE, ID_BTN_SUBTRACT,
		ID_BTN_NEGATIVE, ID_BTN_ZERO, ID_BTN_DECIMAL, ID_BTN_ANSWER
	};

	struct LightThemePalette {
		wxString MainBackground = "FFFDF6"; wxString InputBox = "#ffffff";
		wxString NumberKeys = "#d1d1e0"; wxString OperatorKeys = "#a2a2c2";
		wxString AnswerKey = "#0984e3"; wxString TextColor = "#222222";
	};
	LightThemePalette LightTheme;

	std::vector<wxButton*> CreateButtons();

	void SetUpSizers(std::vector<wxButton*> Buttons, wxFont font);

	// Button related functions
	void OnButtonPressed(wxCommandEvent& evt);
	void OnBackSpace_pressed();
	void OnBtnDel_pressed();
	void SQRT_button();
	void GetAnswer();
	void Delete();

	void AppendSymbol(wxString& Symbol);

	void OnKeyDown(wxKeyEvent& evt);

	bool CheckErrorMessage(wxString ErrorMessages);


	int LastInputWidth = -1;
	void AdjustInputFontSize();
	void OnSize(wxSizeEvent& event);
	void OnIdle(wxIdleEvent& event);

public:
	StandardCalcPanel(wxFrame* mainframe);
};

