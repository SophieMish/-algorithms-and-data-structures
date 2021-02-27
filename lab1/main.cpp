#include <iostream>
#include <array>
#include <limits>

using namespace std;
using T = char;
using BinaryStorage = array<bool, 8 * sizeof(T)>;


//----------------------------Binary Number-------------------------------//

template<typename T>
class BinaryNumber {
    BinaryStorage binary_num_;
public:
    BinaryNumber() :
            binary_num_({}) {}


    explicit BinaryNumber(T num) :
            binary_num_(convert_to_binary_num(num)) {
    }

    explicit BinaryNumber(BinaryStorage binary_num) :
            binary_num_(binary_num) {}


    size_t size() const {
        return binary_num_.size();
    }
//-------------------------Converting------------------------------------//

    BinaryStorage convert_to_binary_num(T num) {
        T mask;
        BinaryStorage binary_num;
        printf("Number %d in binary radix: ", num);

        switch (sizeof(T)) {
            case 1 : //mask for char(8)
                mask = 0100;
                break;

            case 2 : //mask for short int(16)
                mask = 010000;
                break;

            case 4 : //mask for int(32)
                mask = 010000000000;
                break;
        }

        binary_num[0] = (num < 0 ? 1 : 0);
        for (int idx = 1; idx < size(); ++idx) {
            binary_num[idx] = (abs(num) & mask ? 1 : 0);
            mask >>= 1;
        }
        for (int idx = 0; idx < size(); idx++) {
            cout << binary_num[idx];
        }

        cout << endl;
        return binary_num;
    }

    BinaryNumber complimentary() const {
        BinaryNumber res = *this;
        bool temp = res.binary_num_[0];
        for (int idx = 0; idx < size(); idx++) {
            res.binary_num_[idx] = !res.binary_num_[idx];
        }
        BinaryNumber comp;
        comp.binary_num_[size() - 1] = true;
        res = res + comp;
        res.binary_num_[0] = temp;
        return res;
    }

    T convert_to_decimal_num() {
        int pow2 = 1;
        int res = 0;

        for (int idx = size() - 1; idx >= 0; idx--) {
            if (binary_num_[idx] == 1)
                res += pow2;
            pow2 *= 2;
        }

        return res;
    }

    //--------------------Overloading----------------------//
    //sum
    BinaryNumber operator+(const BinaryNumber &other) const {

        BinaryStorage res;

        BinaryNumber A = *this;
        BinaryNumber B = other;

        if (A.binary_num_[0]) {
            A = A.complimentary();
        }

        if (B.binary_num_[0]) {
            B = B.complimentary();
        }

        bool carry = false;
        bool carryOut;
        for (int idx = size() - 1; idx >= 0; idx--) {
            auto a = A.binary_num_[idx];
            auto b = B.binary_num_[idx];
            res[idx] = ((a ^ b) ^ carry);
            carry = ((a & b) | (a & carry)) | (b & carry);
            if (idx == 1) {
                carryOut = carry;
            }
        }
//       bool overflow = (!binary_num_[0] && !other.binary_num_[0] && res[0]) || (binary_num_[0] && other.binary_num_[0] && !res[0]);
        if ((carryOut && !carry) || (!carryOut && carry)) {
            throw runtime_error("Overflow error");
        }

        return BinaryNumber(res);
    }

    //mul
    BinaryNumber shift_left() {
        BinaryNumber res = *this;
        auto tmp = binary_num_[0];
        for (int idx = 0; idx < size() - 1; idx++) {
            res.binary_num_[idx] = res.binary_num_[idx + 1];
        }
        res.binary_num_[size()-1] = tmp;
        return res;
    }

    BinaryNumber operator-(const BinaryNumber<T> &other) const {
        auto B = other;
        B.binary_num_[0] = !binary_num_[0];
        return *this + B;
    }

    BinaryNumber operator*(const BinaryNumber<T> &other) const {
        auto sign = binary_num_[0] ^other.binary_num_[0];
        BinaryNumber acc;
        BinaryNumber multiplicand = *this;
        BinaryNumber multiplier = other;
        multiplicand.binary_num_[0] = false;
        multiplier.binary_num_[0] = false;

        for (int idx = size() - 1; idx > 0; idx--) {
            if (multiplier.binary_num_[idx]) {
                if (multiplicand.binary_num_[0]) {
                    throw runtime_error("Overflow error");
                }
                acc = acc + multiplicand;
            }
            multiplicand = multiplicand.shift_left();

            if (acc.binary_num_[0]) {
                throw runtime_error("Overflow error");
            }
        }
        acc.binary_num_[0] = sign;
        if (sign) {
            return acc.complimentary();
        } else {
            return acc;
        }

    }

    friend ostream &operator<<(ostream &stream, const BinaryNumber &binary_num) {
        for (auto element: binary_num.binary_num_) {
            stream << element;
        }
        return stream;
    }
};


class Expression {
public:
    const T number1;
    const T number2;
    const char oper;

    Expression(T number1, T number2, char oper) :
            number1(number1), number2(number2), oper(oper) {}

    auto calculate() const {
        BinaryNumber<T> binary_num1(number1);

        if (oper == '+') {
            BinaryNumber<T> binary_num2(number2);
            return binary_num1 + binary_num2;
        } else if (oper == '-') {
            BinaryNumber<T> binary_num2(-number2);
            return binary_num1 + binary_num2;
        } else if (oper == '*') {
            BinaryNumber<T> binary_num2(number2);
            return binary_num1 * binary_num2;
        } else {
            throw runtime_error("This is not operator");
        }
    }


};


void check_range(int number) {
    if (number < numeric_limits<T>::min() || number > numeric_limits<T>::max()) {
        throw runtime_error("number is out of range");
    }
}

//---------------------------Enter String-------------------------//
Expression getNumberFromConsole() {
    int num1;
    int num2;
    char oper;
    cout << "Enter the expression" << endl;
    cout << "Number 1: ";
    cin >> num1;
    check_range(num1);
    cout << "Operator: ";
    cin >> oper;
    cout << "Number 2: ";
    cin >> num2;
    check_range(num2);
    return Expression(num1, num2, oper);
}

int main() {
    while (true) {
        auto expression = getNumberFromConsole();
        cout << "Check your expression: " << int(expression.number1) << expression.oper << int(expression.number2)
             << endl;
        auto res = expression.calculate();
        cout << "Result in binary is: " << res << endl;
        cout << "Result in decimal:" << int(res.convert_to_decimal_num()) << endl;
    }

}
