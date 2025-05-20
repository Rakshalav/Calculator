#include "StandardCalcLogic.h"

std::string StandardCalcLogic::replace_unicode_operators(std::string& equation)
{
    // Create a mapping of the Unicode characters to their ASCII equivalents
    std::unordered_map<std::string, std::string> convert_ops = {
        {"\u00F7", "/"},  // ÷ -> /
        {"\u00D7", "*"},  // × -> *
        {"\u2013", "-"}   // – -> -
    };


    // Iterate over each Unicode operator and replace in the input string
    for (const auto& op : convert_ops) {
        size_t pos = 0;
        while ((pos = equation.find(op.first, pos)) != std::string::npos) {
            equation.replace(pos, op.first.length(), op.second);  // Replace the Unicode operator with ASCII
            pos += op.second.length();  // Move past the replaced portion
        }
    }
    return equation;
}

std::vector<std::string> StandardCalcLogic::input_vect(std::string Equation) {
    // Define the operators array with Unicode characters
    std::array<char, 7> ops = { '(', '^', '/', '*', '+', '-', ')' };

    std::string spaced_equation;
    bool lastWasOperator = true;

    // Iterate through the equation string and replace Unicode signs before adding spaces
    for (size_t i = 0; i < Equation.length(); i++) {
        char c = Equation[i];

        // Check if the current character is one of the operators (including Unicode ones)
        if (c == '-' && (i == 0 || lastWasOperator)) {
            spaced_equation += c;  // Negative sign handling
            lastWasOperator = false;
        }
        else if (std::find(std::begin(ops), std::end(ops), c) != std::end(ops)) {
            // Handle operators and add spaces around them
            spaced_equation += " ";
            spaced_equation += c;
            spaced_equation += " ";
            lastWasOperator = true;
        }
        else {
            spaced_equation += c;  // For numbers or non-operator characters
            lastWasOperator = false;
        }
    }

    std::stringstream ss(spaced_equation);  // Tokenize the spaced equation string
    std::string token;
    std::vector<std::string> input_equation;

    // Tokenize the string based on spaces
    while (ss >> token) {
        input_equation.push_back(token);
    }
    return input_equation;
}

std::unordered_map<std::string, StandardCalcLogic::Ooperator> StandardCalcLogic::tokens() {
    return { {"sqrt", {Ooperator::Precedence::SQRT, 1}},
             {"^", {Ooperator::Precedence::EXPONENT, 2}},
             {"/", {Ooperator::Precedence::DIVIDE, 2}},
             {"*", {Ooperator::Precedence::MULTIPLY, 2}},
             {"+", {Ooperator::Precedence::ADD, 2}},
             {"-", {Ooperator::Precedence::SUBTRACT, 2}} };
}

bool StandardCalcLogic::is_number(const std::string& token) {
    if (token.empty()) return false;
    char* end = nullptr;
    std::strtod(token.c_str(), &end);
    return end == token.c_str() + token.size();
}

std::deque<std::string> StandardCalcLogic::postfix_equation(std::vector<std::string> input, std::unordered_map<std::string, Ooperator> ops) {
    std::stack<std::string> operator_stack;
    std::deque<std::string> output_queue;

    for (const std::string& token : input) {
        if (is_number(token)) {
            output_queue.push_back(token);  // Push numbers directly to output
        }
        else if (token == "(") {
            operator_stack.push(token);  // Left parentheses are pushed to stack
        }
        else if (token == ")") {
            while (!operator_stack.empty() && operator_stack.top() != "(") {
                output_queue.push_back(operator_stack.top());  // Pop operators to output
                operator_stack.pop();
            }
            operator_stack.pop();  // Discard the '('
        }
        else {
            // Handle operators (including unary functions like sqrt)
            auto it = ops.find(token);
            if (it != ops.end()) {
                const Ooperator& op = it->second;

                while (!operator_stack.empty() && operator_stack.top() != "(" &&
                    (op.precedence <= ops[operator_stack.top()].precedence ||
                        op.precedence == ops[operator_stack.top()].precedence && op.argument <= ops[operator_stack.top()].argument)) {
                    output_queue.push_back(operator_stack.top());
                    operator_stack.pop();
                }

                operator_stack.push(token);  // Push the operator to stack
            }
        }
    }

    // Pop remaining operators from the stack
    while (!operator_stack.empty()) {
        output_queue.push_back(operator_stack.top());
        operator_stack.pop();
    }

    return output_queue;
}

std::tuple<double, StandardCalcLogic::Errors> StandardCalcLogic::postfix_evaluation(std::deque<std::string> postfix_eq)
{
    std::stack<double> result_stk;

    // Helper function to pop two values from the stack
    auto pop_two_values = [&result_stk]() -> std::pair<double, double> {
        double b = result_stk.top(); result_stk.pop();
        double a = result_stk.top(); result_stk.pop();
        return { a, b };
        };

    for (const auto& token : postfix_eq) {
        try {
            result_stk.push(std::stod(token));  // If it's a number, push it to the stack
        }
        catch (const std::invalid_argument&) {  // If it's an operator or function (like sqrt)
            if (result_stk.size() < 1 && token != "sqrt") {  // For unary operators like sqrt, 1 operand is needed
                return { NAN, Errors::SYNTAX_ERROR };  // Return error and NaN result
            }

            if (token == "sqrt") {  // Special handling for sqrt (unary operator)
                double a = result_stk.top(); result_stk.pop();
                if (a < 0) {  // If the number is negative, sqrt is invalid
                    return { NAN, Errors::NEGATIVE_SQRT };
                }
                result_stk.push(std::sqrt(a));  // Push the square root of the number
            }
            else {
                // Handle binary operators (+, -, *, /, ^)
                double a, b;
                std::tie(a, b) = pop_two_values();  // Pop two values

                if (token == "^") result_stk.push(pow(a, b));  // Exponentiation
                else if (token == "/") {
                    if (b == 0) {
                        return { NAN, Errors::ZERO_DIVISION };  // Division by zero
                    }
                    result_stk.push(a / b);  // Division
                }
                else if (token == "*") result_stk.push(a * b);  // Multiplication
                else if (token == "+") result_stk.push(a + b);  // Addition
                else if (token == "-") result_stk.push(a - b);  // Subtraction
            }
        }
    }

    if (result_stk.empty()) {
        return { NAN, Errors::SYNTAX_ERROR };  // If stack is empty, return error
    }
    else {
        return { result_stk.top(), Errors::NONE };  // Return the result and no error
    }
}


std::tuple<bool, std::string>StandardCalcLogic::ErrorHandler(Errors error_type)
{
    bool hasError = false;
    std::string errorMessage;
    switch (error_type)
    {
    case StandardCalcLogic::Errors::NONE:
        hasError = false;
        errorMessage = "No error";
        break;
    case StandardCalcLogic::Errors::SYNTAX_ERROR:
        hasError = true;
        errorMessage = "Syntax error!";
        break;
    case StandardCalcLogic::Errors::NEGATIVE_SQRT:
        hasError = true;
        errorMessage = "Math Error: Square root of negative number!";
        break;
    case StandardCalcLogic::Errors::ZERO_DIVISION:
        hasError = true;
        errorMessage = "Math Error: Division by zero!";
        break;
    default:
        hasError = false;
        errorMessage = "Unknown error!";
        break;
    }
    return std::make_tuple(hasError, errorMessage);
}

bool StandardCalcLogic::CheckSyntax(const std::vector<std::string>& input)
{
    int brackets_count = 0;
    bool last_was_op = true;

    for (size_t i = 0; i < input.size(); i++)
    {
        const std::string& token = input[i];

        if (token == "(") {
            brackets_count++;
            last_was_op = true;
        }
        else if (token == ")") {
            brackets_count--;
            if (brackets_count < 0)
                return false;
            last_was_op = false;
        }
        else if (token == "^" || token == "/" || token == "*" || token == "+" || token == "-") {
            if (last_was_op)
                return false;
            last_was_op = true;
        }
        else if (token == "sqrt") {
            if (!last_was_op)
                return false;
        }
        else if (is_number(token))
            last_was_op = false;
        else
            return false;
    }
    if (brackets_count != 0)
        return false;
    if (last_was_op)
        return false;
    return true;
}

std::string StandardCalcLogic::Answer()
{
    std::string convert_math_symbols = replace_unicode_operators(Equation);
    std::vector<std::string> equation_vect = input_vect(convert_math_symbols);

    if (CheckSyntax(equation_vect))
    {
        std::unordered_map<std::string, Ooperator> ops = tokens();
        std::deque<std::string> postfix_eq = postfix_equation(equation_vect, ops);

        auto result_and_error = postfix_evaluation(postfix_eq);

        double result = std::get<0>(result_and_error);
        Errors errorType = std::get<1>(result_and_error);

        if (std::isnan(result)) {
            bool hasError;
            std::string message;
            std::tie(hasError, message) = ErrorHandler(errorType);
            if (hasError) {
                return message;
            }
        }

        std::ostringstream oss;
        oss << std::fixed << std::setprecision(9) << result;
        std::string formatted = oss.str();

        formatted.erase(formatted.find_last_not_of('0') + 1);
        if (formatted.back() == '.')
            formatted.pop_back();

        return formatted;
    }
    else
    {
        std::tuple<bool, std::string> syntax_error = ErrorHandler(Errors::SYNTAX_ERROR);
        std::string get_error_message = std::get<1>(syntax_error);

        return get_error_message;
    }
}