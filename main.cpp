#include <iostream>
#include <sstream>
#include <string>
#include <stack>
#include <cmath>

using namespace std;

struct Fraction {
    int numerator;
    int denominator;
};

double calculate(double operand1, double operand2, char operation) {
    switch (operation) {
        case '+':
            return operand1 + operand2;
        case '-':
            return operand1 - operand2;
        case '*':
            return operand1 * operand2;
        case '/':
            if (operand2 != 0)
                return operand1 / operand2;
            else {
                cerr << "Error: Division by zero!" << endl;
                return NAN;
            }
        case '^':
            return pow(operand1, operand2);
        default:
            cerr << "Error: Invalid operator!" << endl;
            return NAN;
    }
}

bool isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}

bool isFraction(const string& str) {
    return str.find('/') != string::npos;
}

bool isSquareRoot(const string& str) {
    return str.substr(0, 4) == "sqrt";
}

bool isLog(const string& str) {
    return str.substr(0, 3) == "log" && str.size() > 3 && str[3] == '(' && str.back() == ')';
}

bool isPower(const string& str) {
    return str == "^";
}

Fraction parseFraction(const string& str) {
    stringstream ss(str);
    Fraction fraction;
    char delimiter;
    ss >> fraction.numerator >> delimiter >> fraction.denominator;
    return fraction;
}

double fractionToDouble(const Fraction& fraction) {
    return static_cast<double>(fraction.numerator) / fraction.denominator;
}

double evaluateExpression(const string& expression) {
    istringstream iss(expression);
    stack<double> operands;
    stack<char> operators;
    // Inside the evaluateExpression function, before token processing
    string processedExpression = expression;
    processedExpression.erase(remove(processedExpression.begin(), processedExpression.end(), ' '), processedExpression.end());
    string token;
    while (iss >> token) {
        if (isdigit(token[0]) || (token[0] == '-' && token.length() > 1)) {
            if (isFraction(token)) {
                Fraction frac = parseFraction(token);
                double value = fractionToDouble(frac);
                operands.push(value);
            } else {
                double number;
                stringstream(token) >> number;
                operands.push(number);
            }
        } else if (isOperator(token[0])) {
            while (!operators.empty() && operators.top() != '(' &&
                   ((token[0] == '*' || token[0] == '/' || token[0] == '^') ||
                    (token[0] == '+' || token[0] == '-')) &&
                   (operators.top() == '*' || operators.top() == '/' || operators.top() == '^')) {
                double operand2 = operands.top();
                operands.pop();
                double operand1 = operands.top();
                operands.pop();
                char op = operators.top();
                operators.pop();
                double result = calculate(operand1, operand2, op);
                operands.push(result);
            }
            operators.push(token[0]);
        } else if (isSquareRoot(token)) {
            string number = token.substr(5, token.size() - 6);
            double value;
            stringstream(number) >> value;
            operands.push(sqrt(value));
        } else if (isLog(token)) {
            string number = token.substr(4, token.size() - 5); // Extract the number within parentheses
            double value;
            stringstream(number) >> value;

            if (value <= 0) {
                cerr << "Error: Logarithm of a non-positive number or zero!" << endl;
                return NAN;
            }

            operands.push(log10(value)); // Calculate base-10 logarithm
        } else if (isPower(token)) {
            while (!operators.empty() && operators.top() == '^') {
                double operand2 = operands.top();
                operands.pop();
                double operand1 = operands.top();
                operands.pop();
                char op = operators.top();
                operators.pop();
                double result = pow(operand1, operand2);
                operands.push(result);
            }
            operators.push('^');
        } else if (token[0] == '(') {
            operators.push(token[0]);
        } else if (token[0] == ')') {
            while (!operators.empty() && operators.top() != '(') {
                double operand2 = operands.top();
                operands.pop();
                double operand1 = operands.top();
                operands.pop();
                char op = operators.top();
                operators.pop();
                double result = calculate(operand1, operand2, op);
                operands.push(result);
            }
            if (!operators.empty()) {
                operators.pop();
            } else {
                cerr << "Error: Unmatched parenthesis!" << endl;
                return NAN;
            }
        }
    }

    while (!operators.empty()) {
        double operand2 = operands.top();
        operands.pop();
        double operand1 = operands.top();
        operands.pop();
        char op = operators.top();
        operators.pop();
        double result = calculate(operand1, operand2, op);
        operands.push(result);
    }

    return operands.top();
}

int main() {
    string expression;

    while (true) {
        cout << "Enter an expression (or 'q' to quit): ";
        getline(cin, expression);

        if (expression == "q")
            break;

        double result = evaluateExpression(expression);
        if (!isnan(result)) {
            cout << "Result: " << result << endl;
        }
    }

    return 0;
}