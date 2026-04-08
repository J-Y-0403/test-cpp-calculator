#include <iostream>
#include <string>
#include <stack>
#include <cctype>

using namespace std;

// トークンの種類
enum TokenType {
    NUMBER,
    OPERATOR,
    LPAREN,
    RPAREN,
    END
};

// トークン構造体
struct Token {
    TokenType type;
    double value;
    char op;
};

// トークナイザー
class Tokenizer {
public:
    Tokenizer(const string& expr) : expression(expr), pos(0) {}

    Token getNextToken() {
        while (pos < expression.length() && isspace(expression[pos])) pos++;

        if (pos >= expression.length()) return {END, 0, '\0'};

        if (isdigit(expression[pos]) || expression[pos] == '.') {
            double num = 0;
            while (pos < expression.length() && (isdigit(expression[pos]) || expression[pos] == '.')) {
                num = num * 10 + (expression[pos] - '0');
                pos++;
            }
            return {NUMBER, num, '\0'};
        }

        char ch = expression[pos++];
        if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            return {OPERATOR, 0, ch};
        } else if (ch == '(') {
            return {LPAREN, 0, '\0'};
        } else if (ch == ')') {
            return {RPAREN, 0, '\0'};
        }

        return {END, 0, '\0'};
    }

private:
    string expression;
    size_t pos;
};

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// 式を評価する関数
double evaluate(const string& expr) {
    Tokenizer tokenizer(expr);
    stack<double> values;
    stack<char> ops;

    auto applyOp = [&]() {
        double b = values.top(); values.pop();
        double a = values.top(); values.pop();
        char op = ops.top(); ops.pop();
        if (op == '+') values.push(a + b);
        else if (op == '-') values.push(a - b);
        else if (op == '*') values.push(a * b);
        else if (op == '/') {
            if (b == 0) throw runtime_error("Division by zero");
            values.push(a / b);
        }
    };

    Token token = tokenizer.getNextToken();
    while (token.type != END) {
        if (token.type == NUMBER) {
            values.push(token.value);
        } else if (token.type == LPAREN) {
            ops.push('(');
        } else if (token.type == RPAREN) {
            while (!ops.empty() && ops.top() != '(') {
                applyOp();
            }
            if (!ops.empty()) ops.pop(); // '(' をポップ
        } else if (token.type == OPERATOR) {
            while (!ops.empty() && precedence(ops.top()) >= precedence(token.op)) {
                applyOp();
            }
            ops.push(token.op);
        }
        token = tokenizer.getNextToken();
    }

    while (!ops.empty()) {
        applyOp();
    }

    return values.top();
}

int main() {
    cout << "C++ Calculator" << endl;
    cout << "Enter expressions like: 3 + 4 * 2" << endl;
    cout << "Type 'exit' to quit." << endl;

    string input;
    while (true) {
        cout << "> ";
        getline(cin, input);
        if (input == "exit") break;

        try {
            double result = evaluate(input);
            cout << "= " << result << endl;
        } catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }

    return 0;
}