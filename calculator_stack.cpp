#include <math.h>

#include <fstream>
#include <iostream>
#include <stack>
#include <string>
#include <vector>
using namespace std;

class NodeObj {
   public:
    char op_or_pr = ' ';  // oprators of brackets
    float operand;
    bool is_a_digit() {
        if (op_or_pr == ' ') {
            return true;
        } else {
            return false;
        }
    }
};

float char_to_float(char x) { return float(int(x) - 48); }

float round_r(float value) {
    float temp = round(value * 100);
    return temp / 100;
}

int operator_precedence(char x) {
    // BODMAS
    switch (x) {
        case '^':
            return 3;
            break;
        case '/':
            return 2;
            break;
        case '*':
            return 2;
            break;
        case '+':
            return 1;
            break;
        case '-':
            return 1;
            break;
        default:
            return -1;
    }
}

bool is_a_digit(char x) {
    if (x == '(' || x == ')' || x == '*' || x == '/' || x == '+' || x == '-' ||
        x == '^') {
        return false;
    } else {
        return true;
    }
}

void infix_to_postfix(vector<char> &infix_expression,
                      vector<NodeObj> &postfix_expression) {
    stack<char> temp_stack;
    char prev = 'a';

    for (int i = 0; i < infix_expression.size(); i++) {
        char current_char = infix_expression[i];
        if (is_a_digit(current_char)) {
            if (is_a_digit(prev)) {
                NodeObj temp =
                    postfix_expression[postfix_expression.size() - 1];
                float temp_val =
                    temp.operand * 10 + char_to_float(current_char);
                postfix_expression.pop_back();
                NodeObj temp_2;
                temp_2.operand = temp_val;
                postfix_expression.push_back(temp_2);

            } else {
                NodeObj node;
                node.operand = char_to_float(current_char);
                postfix_expression.push_back(node);
            }

        } else if (current_char == '(') {
            temp_stack.push(current_char);
        } else if (current_char == ')') {
            while (temp_stack.top() != '(') {
                NodeObj node;
                node.op_or_pr = temp_stack.top();
                postfix_expression.push_back(node);
                temp_stack.pop();
            }
            temp_stack.pop();
        } else {
            while (!temp_stack.empty() &&
                   operator_precedence(current_char) <=
                       operator_precedence(temp_stack.top())) {
                NodeObj node;
                node.op_or_pr = temp_stack.top();
                postfix_expression.push_back(node);
                temp_stack.pop();
            }
            temp_stack.push(current_char);
        }

        prev = current_char;
    }
    while (!temp_stack.empty()) {
        NodeObj node;
        node.op_or_pr = temp_stack.top();
        postfix_expression.push_back(node);
        temp_stack.pop();
    }
}

float evalutate_value(char current_char, float temp_a, float temp_b) {
    switch (current_char) {
        case '*':
            return temp_a * temp_b;
            break;
        case '+':
            return temp_a + temp_b;
            break;
        case '-':
            return temp_a - temp_b;
            break;
        case '/':
            return temp_a / temp_b;
            break;
        case '^':
            return pow(temp_a, temp_b);
            break;
        default:
            return -1;
    }
}

float postfix_evaluation(vector<NodeObj> &postfix_expression) {
    stack<float> temp_stack;
    for (int i = 0; i < postfix_expression.size(); i++) {
        NodeObj current_char = postfix_expression[i];
        if (current_char.is_a_digit()) {
            temp_stack.push(current_char.operand);
        } else {
            float temp_b = temp_stack.top();
            temp_stack.pop();
            float temp_a = temp_stack.top();
            temp_stack.pop();
            temp_stack.push(
                evalutate_value(current_char.op_or_pr, temp_a, temp_b));
        }
    }
    return temp_stack.top();
}

vector<char> clear_white_spaces(vector<char> &infix_expression) {
    vector<char> temp_vector;
    for (int i = 0; i < infix_expression.size(); i++) {
        if (infix_expression[i] != ' ') {
            temp_vector.push_back(infix_expression[i]);
        }
    }
    return temp_vector;
}

bool is_a_valid_expression(vector<char> infix_expression) {
    bool is_valid = true;
    for (int i = 0; i < infix_expression.size(); i++) {
        char c = infix_expression[i];
        if (c >= '0' && c <= '9') {
            is_valid = true;
        } else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^' ||
                   c == '(' || c == ')') {
            is_valid = true;
        } else {
            return false;
        }
    }
    return is_valid;
}

void read_from_file() {
    ofstream out_file;
    out_file.open("result.txt");
    ifstream new_file("formulas.txt");
    string a;
    while (getline(new_file, a)) {
        if (a.empty()) {
            out_file << a << "  ->Empty Line" << endl;
            continue;
        }
        int i = 0;
        vector<char> exp;
        for (int i = 0; i < a.length(); i++) {
            exp.push_back(a[i]);
        }
        exp.pop_back();
        exp = clear_white_spaces(exp);
        exp.insert(exp.begin(), '(');   //adds a ( to begining
        exp.push_back(')');             //adds a ) to end
        vector<char> infix;
        vector<NodeObj> postfix;
        infix = exp;
        if (is_a_valid_expression(infix)) {
            infix_to_postfix(infix, postfix);
            float val = postfix_evaluation(postfix);
            val = round_r(val);
            out_file << a << val << endl;
        } else {
            out_file << a << "  ->The Expression is not valid" << endl;
        }
    }
    out_file.close();
    new_file.close();
}

int main() { read_from_file(); }
