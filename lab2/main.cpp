#include <iostream>
#include "BinaryNumber.hpp"
#include "Stack.hpp"

bool isOperation(const string &el) {
    if (el != "+" && el != "*" && el != "-") {
        return false;
    }
    return true;

}

template<typename T>
T calc(T num1, T num2, const string &oper) {
    if (oper == "+") {
        return num1 + num2;
    } else if (oper == "-") {
        return num1 - num2;
    } else if (oper == "*") {
        return num1 * num2;
    }
}

int main() {
//    while (true) {
    string expr_str;
    cout << "Enter your expression:";
    getline(cin, expr_str);
    vector<string> expr_vec = parseString(expr_str);

    Stack<BinaryNumber<T>> stack;
    for (auto &el : expr_vec) {
        if (!isOperation(el)) {
            stack.push(BinaryNumber<T>(atoi(el.data())));
        } else {
            auto num2 = stack.pop();
            auto num1 = stack.pop();
            stack.push(calc(num1, num2, el));
        }
    }
    auto res = stack.pop();
    cout << "Result in Binary: " << res << endl;
    cout << "Result in Decimal: " << int(res.convert_to_decimal_num()) << endl;
}

