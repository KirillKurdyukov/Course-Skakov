#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>

// immutable class
class BigInteger {
private:
    bool isNaN = false;
    bool isNegative{};
    std::vector<int> num;
public:
    BigInteger(std::string number) {
        if (number[0] == '-') {
            isNegative = true;
            for (size_t i = number.size() - 1; i > 0; i--)
                num.push_back(number[i] - 48);
        } else {
            isNegative = false;
            for (int i = number.size() - 1; i >= 0; i--)
                num.push_back(number[i] - 48);
        }
    }

    BigInteger(bool _isNegative, std::vector<int> _num) : isNegative(_isNegative), num(std::move(_num)) {}

    std::vector<int> getNumber() const {
        return num;
    }

    bool isNegate() const {
        return isNegative;
    }

    bool NaN() const {
        return isNaN;
    }

    void clearZero() {
        int temp = this->num.size() - 1;
        while (this->num[temp] == 0 && temp > 0) {
            this->num.pop_back();
            temp--;
        }
    }

    BigInteger abs() const {
        return {false, this->num};
    }

    void toString() const {
        if (this->isNaN) {
            std::cout << "NaN";
            return;
        }
        if (this->isNegative)
            std::cout << "-";
        std::vector<int> temp = this->num;
        reverse(temp.begin(), temp.end());
        bool checkZero = false;
        for (auto now : temp) {
            if (now || checkZero) {
                checkZero = true;
                std::cout << now;
            }
        }
        if (!checkZero)
            std::cout << temp.back();
    }

    void toString(std::ofstream &out) const {
        if (this->isNaN) {
            out << "NaN";
            return;
        }
        if (this->isNegative)
            out << "-";
        std::vector<int> temp = this->num;
        reverse(temp.begin(), temp.end());
        bool checkZero = false;
        for (auto now : temp) {
            if (now || checkZero) {
                checkZero = true;
                out << now;
            }
        }
        if (!checkZero)
            out << temp.back();
    }

    // a.comparator(b) a > b return 1, a == b return 0, a < b return -1;
    int comparator(BigInteger &number) {
        number.clearZero();
        this->clearZero();
        if (!this->isNegative && !number.isNegate()) {
            if (this->num.size() > number.getNumber().size())
                return 1;
            if (this->num.size() < number.getNumber().size())
                return -1;
            if (this->num.size() == number.getNumber().size())
                for (int i = number.getNumber().size() - 1; i >= 0; i--) {
                    if (this->num[i] > number.getNumber()[i])
                        return 1;
                    if (this->num[i] < number.getNumber()[i])
                        return -1;
                }
            return 0;
        }
        if (number.isNegate() ^ this->isNegative) {
            if (number.isNegate())
                return 1;
            else return -1;
        }
        BigInteger cnt(!number.isNegate(), number.getNumber());
        return -BigInteger(!this->isNegative, this->num).comparator(cnt);
    }

    BigInteger negate() const {
        return BigInteger(!this->isNegative, this->num);
    }

    BigInteger add(BigInteger &number) {
        std::vector<int> newObject;
        int prev = 0;
        if (!(number.isNegate() ^ this->isNegative)) {
            for (size_t i = 0; i < std::min(number.getNumber().size(), this->num.size()); i++) {
                newObject.push_back((number.getNumber()[i] + this->num[i] + prev) % 10);
                prev = (number.getNumber()[i] + this->num[i] + prev) / 10;
            }
            if (number.getNumber().size() > this->num.size())
                for (size_t i = this->num.size(); i < number.getNumber().size(); i++) {
                    newObject.push_back((number.getNumber()[i] + prev) % 10);
                    prev = (number.getNumber()[i] + prev) / 10;
                }
            else
                for (size_t i = number.getNumber().size(); i < this->num.size(); i++) {
                    newObject.push_back((this->num[i] + prev) % 10);
                    prev = (this->num[i] + prev) / 10;
                }
            if (prev)
                newObject.push_back(prev);
            return BigInteger(number.isNegate(), newObject);
        } else {
            if (!this->isNegative) {
                BigInteger temp = number.negate();
                if (this->comparator(temp) == 0)
                    return BigInteger(true, {0});
                if (this->comparator(temp) == 1) {
                    for (size_t i = 0; i < std::min(number.getNumber().size(), this->num.size()); i++) {
                        if (this->num[i] - number.getNumber()[i] - prev >= 0) {
                            newObject.push_back(this->num[i] - number.getNumber()[i] - prev);
                            prev = 0;
                        } else {
                            newObject.push_back(10 - number.getNumber()[i] + this->num[i] - prev);
                            prev = 1;
                        }
                    }
                    for (size_t i = number.getNumber().size(); i < this->num.size(); i++) {
                        if (this->num[i] - prev >= 0) {
                            newObject.push_back(this->num[i] - prev);
                            prev = 0;
                        } else {
                            newObject.push_back(10 + this->num[i] - prev);
                            prev = 1;
                        }
                    }
                    return BigInteger(false, newObject);
                } else {
                    for (size_t i = 0; i < std::min(number.getNumber().size(), this->num.size()); i++) {
                        if (number.getNumber()[i] - this->num[i] - prev >= 0) {
                            newObject.push_back(number.getNumber()[i] - this->num[i] - prev);
                            prev = 0;
                        } else {
                            newObject.push_back(10 + number.getNumber()[i] - this->num[i] - prev);
                            prev = 1;
                        }
                    }
                    for (size_t i = this->num.size(); i < number.getNumber().size(); i++) {
                        if (number.getNumber()[i] - prev >= 0) {
                            newObject.push_back(number.getNumber()[i] - prev);
                            prev = 0;
                        } else {
                            newObject.push_back(10 + number.getNumber()[i] - prev);
                            prev = 1;
                        }
                    }
                    return BigInteger(true, newObject);
                }
            } else {
                return number.add(*this);
            }
        }
    }

    BigInteger subtract(BigInteger &number) {
        if (this->comparator(number) == 0)
            return BigInteger(false, {0});
        BigInteger temp(!number.isNegate(), number.getNumber());
        return this->add(temp);
    }

    BigInteger multiply(const BigInteger &number) {
        BigInteger ans = {false, {0}};
        std::vector<int> temp = this->num;
        for (int i = 0; i < number.getNumber().size(); i++) {
            std::vector<int> newObject;
            int prev = 0;
            for (int j : temp) {
                newObject.push_back((prev + j * number.getNumber()[i]) % 10);
                prev = (prev + j * number.getNumber()[i]) / 10;
            }
            if (prev)
                newObject.push_back(prev);
            BigInteger cnt(false, newObject);
            ans = ans.add(cnt);
            reverse(temp.begin(), temp.end());
            temp.push_back(0);
            reverse(temp.begin(), temp.end());
        }
        return BigInteger(number.isNegate() ^ this->isNegative, ans.getNumber());
    }

    BigInteger divide(BigInteger &number) {
        BigInteger temp = this->abs();
        if (temp.comparator(number) == -1)
            return {false, {0}};
        BigInteger cnt = number.abs();
        if (temp.comparator(cnt) == 0)
            return BigInteger(number.isNegate() ^ this->isNegative, {1});
        cnt = {false, {0}};
        if (number.comparator(cnt) == 0) {
            return true;
        }
        BigInteger ans = {false, {0}};
        while (temp.comparator(number) >= 0) {
            bool iterationDone = false;
            for (int i = temp.getNumber().size() - 1; i >= 0; i--) {
                std::vector<int> ranks;
                for (int j = i; j < temp.getNumber().size(); j++) {
                    ranks.push_back(temp.getNumber()[j]);
                }
                for (int k = 9; k > 0; k--) {
                    cnt = BigInteger(false, {k}).multiply(number);
                    if (BigInteger(false, ranks).comparator(cnt) >= 0) {
                        iterationDone = true;
                        ranks = BigInteger(false, {k}).multiply(number).getNumber();
                        reverse(ranks.begin(), ranks.end());
                        std::vector<int> newObject;
                        for (int j = 0; j < i; j++) {
                            ranks.push_back(0);
                            newObject.push_back(0);
                        }
                        newObject.push_back(k);
                        BigInteger t = {false, newObject};
                        ans = ans.add(t);
                        reverse(ranks.begin(), ranks.end());
                        BigInteger cur = {false, ranks};
                        temp = temp.subtract(cur);
                    }
                    if (iterationDone)
                        break;
                }
                if (iterationDone)
                    break;
            }
        }
        return BigInteger(number.isNegate() ^ this->isNegative, ans.getNumber());
    }

    BigInteger mod(BigInteger &number) {
        BigInteger temp(number.multiply(this->divide(number)));
        return this->subtract(temp);
    }

    BigInteger sqrt() {
        BigInteger a = {false, {0}};
        BigInteger b = {false, {1}};
        if (this->comparator(a) == -1) {
            return true;
        }
        if (this->comparator(a) == 0 || this->comparator(b) == 0)
            return {this->isNegative, this->num};
        BigInteger l = BigInteger(false, {0}); // множество чисел меньше корня
        BigInteger r = BigInteger({false, this->num}); // множество чисел больше корня
        while (r.subtract(l).comparator(b) > 0) {
            BigInteger cnt = {false, {2}};
            BigInteger temp = r.subtract(l).divide(cnt).add(l);
            if (temp.multiply(temp).comparator(*this) <= 0)
                l = temp;
            if (temp.multiply(temp).comparator(*this) > 0)
                r = temp;
        }
        return l;
    }

    BigInteger (int number) {
        isNegative = number < 0;
        if (number == 0)
            num.push_back(0);
        while (std::abs(number) > 0) {
            num.push_back(number % 10);
            number /= 10;
        }
    }

    BigInteger(bool NaN) : isNaN(NaN) {}

    BigInteger() = default;
};

BigInteger operator+(BigInteger &a, BigInteger &b) {
    return a.add(b);
}

BigInteger operator-(BigInteger &a, BigInteger &b) {
    return a.subtract(b);
}

BigInteger operator*(BigInteger &a, BigInteger &b) {
    return a.multiply(b);
}

BigInteger operator/(BigInteger &a, BigInteger &b) {
    return a.divide(b);
}

BigInteger operator%(BigInteger &a, BigInteger &b) {
    return a.mod(b);
}

std::ostream &operator<<(std::ofstream &out, const BigInteger &number) {
    number.toString(out);
    return out;
}

int operator>(BigInteger &a, BigInteger &b) {
    if (a.comparator(b) == 1)
        return 1;
    else return 0;
}

int operator<(BigInteger &a, BigInteger &b) {
    if (a.comparator(b) == -1)
        return 1;
    else return 0;
}

int operator==(BigInteger &a, BigInteger &b) {
    if (a.comparator(b) == 0)
        return 1;
    else return 0;
}

int operator!=(BigInteger &a, BigInteger &b) {
    if (a.comparator(b) != 0)
        return 1;
    else return 0;
}

int operator>=(BigInteger &a, BigInteger &b) {
    if (a.comparator(b) >= 0)
        return 1;
    else return 0;
}

int operator<=(BigInteger &a, BigInteger &b) {
    if (a.comparator(b) <= 0)
        return 1;
    else return 0;
}

BigInteger sqrt(BigInteger &a) {
    return a.sqrt();
}

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("There was no unexpected error, please try again");
        return 1;
    }
    std::ifstream in(argv[1]);
    if (!in.is_open()) {
        printf("Not found input file exception");
        in.close();
        return 1;
    }
    std::string s1, operation, s2;
    in >> s1 >> operation;
    BigInteger a(s1);
    std::ofstream out(argv[2]);
    if (!out.is_open()) {
        printf("Not found output file exception");
        out.close();
        return 1;
    }
    if (operation != "#") {
        in >> s2;
        BigInteger b(s2);
        switch (operation[0]) {
            case '+' :
                out << a + b;
                break;
            case '-' :
                out << a - b;
                break;
            case '*' :
                out << a * b;
                break;
            case '%' :
                out << a % b;
                break;
            case '/' :
                out << a / b;
                break;
        }
        if (operation == ">") {
            out << BigInteger(a > b);
        }
        if (operation == "<") {
            out << BigInteger(a < b);
        }
        if (operation == "<=") {
            out << BigInteger(a <= b);
        }
        if (operation == ">=") {
            out << BigInteger(a >= b);
        }
        if (operation == "==") {
            out << BigInteger(a == b);
        }
        if (operation == "!=") {
            out << BigInteger(a != b);
        }
    } else {
        out << sqrt(a);
    }
    in.close();
    out.close();
    return 0;
}
