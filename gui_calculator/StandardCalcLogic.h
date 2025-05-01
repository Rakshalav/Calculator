#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <vector>
#include <string>
#include <unordered_map>
#include <sstream>
#include <stack>
#include <cmath>
#include <stdexcept>
#include <deque>
#include <cctype>
#include <array>
#include <tuple>
#include <iomanip>
#include <wx/wx.h>

class StandardCalcLogic
{
private:
    std::string Equation;

    struct Ooperator {
        enum class Precedence {
            SUBTRACT = 1, ADD, MULTIPLY, DIVIDE, EXPONENT, SQRT
        };
        Precedence precedence;
        uint8_t argument = 0;
    };

    enum class Errors { NONE, SYNTAX_ERROR, NEGATIVE_SQRT, ZERO_DIVISION };

    std::tuple<bool, std::string> ErrorHandler(Errors);

    bool CheckSyntax(const std::vector<std::string>& input);

    std::string replace_unicode_operators(std::string& equation);

    std::vector<std::string> input_vect(std::string Equation);

    std::unordered_map<std::string, Ooperator> tokens();

    bool is_number(const std::string& token);

    std::deque<std::string> postfix_equation(std::vector<std::string> input, std::unordered_map<std::string, Ooperator> ops);

    std::tuple<double, Errors> postfix_evaluation(std::deque<std::string> postfix_eq);


public:
    StandardCalcLogic(std::string input) : Equation(input){}
    std::string Answer();
    
};


