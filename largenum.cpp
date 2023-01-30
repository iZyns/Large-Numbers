#include "largenum.h"
#include <algorithm>
#include <cassert>
#include <iostream>

using namespace std;

LargeNum::LargeNum(const string &str) {
  num = str;
  if (num.length() == 1) {
    return;
  }
  reverse(num.begin(), num.end());
  if (num.back() == '-') {
    isPositive = false;
    num.pop_back();
  }
  bool isZero = false;
  for (int i = 0; i < num.length(); i++) {
    if (num.at(i) == '0') {
      isZero = true;
    } else {
      return;
    }
  }
  if (isZero == true) {
    num.clear();
    num.insert(0, "0");
  }
}

LargeNum::LargeNum(int anInteger) {
  if (anInteger < 0) {
    isPositive = false;
    anInteger *= -1;
  }
  if (anInteger == 0) {
    anInteger = 0;
  }
  num = to_string(anInteger);
  if (num.length() == 1) {
    return;
  }
  reverse(num.begin(), num.end());
}

bool isSmaller(string n1, string n2) {
  int l1 = n1.length();
  int l2 = n2.length();
  if (l1 < l2) {
    return true;
  }
  if (l2 < l1) {
    return false;
  }
  for (int i = 0; i < l1; i++) {
    if (n1[i] < n2[i]) {
      return true;
    } else if (n1[i] > n2[i]) {
      return false;
    }
  }
  return false;
}

bool LargeNum::isZero() const {
  bool isZero = false;
  for (int i = 0; i < num.length(); i++) {
    if (num.at(i) == '0') {
      isZero = true;
    } else {
      return false;
    }
  }
  return isZero;
}

LargeNum &LargeNum::negate() {
  if (isZero() == true) {
    isPositive = true;
  }
  if (isPositive == true) {
    isPositive = false;
  } else {
    isPositive = true;
  }
  return *this;
}

LargeNum LargeNum::operator+(const LargeNum &rhs) const {
  LargeNum temp2(rhs);
  if (temp2.isPositive == false && isPositive == true) {
    temp2.negate();
    return operator-(temp2);
  } else if (temp2.isPositive == true && isPositive == false) {
    return operator-(rhs);
  }
  string num1 = num;
  string num2 = temp2.num;
  if (num1.length() > num2.length())
    swap(num1, num2);
  string str = "";
  int n1 = num1.length();
  int n2 = num2.length();
  int carry = 0;
  for (int i = 0; i < n1; i++) {
    int sum = ((num1[i] - '0') + (num2[i] - '0') + carry);
    str.push_back(sum % 10 + '0');
    carry = sum / 10;
  }
  for (int i = n1; i < n2; i++) {
    int sum = ((num2[i] - '0') + carry);
    str.push_back(sum % 10 + '0');
    carry = sum / 10;
  }
  if (carry) {
    str.push_back(carry + '0');
  }
  LargeNum res(str);
  if (isPositive == false && temp2.isPositive == false) {
    res.negate();
  }
  reverse(res.num.begin(), res.num.end());
  return res;
}

LargeNum LargeNum::operator-(const LargeNum &rhs) const {
  LargeNum temp2(rhs);
  if (temp2.isPositive == false && isPositive == true) {
    temp2.negate();
    return operator+(temp2);
  }
  string num2 = temp2.num;
  string num1 = num;
  if (isSmaller(num1, num2)) {
    swap(num1, num2);
  }
  string str = "";
  int n1 = num1.length();
  int n2 = num2.length();
  int carry = 0;
  for (int i = 0; i < n2; i++) {
    int sub = ((num1[i] - '0') - (num2[i] - '0') - carry);
    if (sub < 0) {
      sub = sub + 10;
      carry = 1;
    } else
      carry = 0;
    str.push_back(sub + '0');
  }
  for (int i = n2; i < n1; i++) {
    int sub = ((num1[i] - '0') - carry);
    if (sub < 0) {
      sub = sub + 10;
      carry = 1;
    } else
      carry = 0;
    str.push_back(sub + '0');
  }
  while (str.length() > 1 && str.at(str.length() - 1) == '0') {
    str.pop_back();
  }
  reverse(str.begin(), str.end());
  LargeNum res(str);
  if (isPositive == false && temp2.isPositive == false) {
    res.negate();
    return res;
  } else if (isSmaller(num1, num2)) {
    res.negate();
    return res;
  }
  return res;
}

LargeNum LargeNum::operator*(const LargeNum &rhs) const {
  LargeNum temp2(rhs);
  string s1 = num;
  string s2 = temp2.num;
  vector<int> m(s1.length() + s2.length());
  for (int i = 0; i < s1.length(); i++) {
    for (int j = 0; j < s2.length(); j++) {
      m[i + j] = m[i + j] + (s1[i] - '0') * (s2[j] - '0');
    }
  }
  string product = "";
  for (int i = 0; i < m.size(); i++) {
    int digit = m[i] % 10;
    int carry = m[i] / 10;
    if (i + 1 < m.size()) {
      m[i + 1] = m[i + 1] + carry;
    }
    product = to_string(digit) + product;
  }
  while (product.length() > 1 && product[0] == '0') {
    product = product.substr(1);
  }
  LargeNum res(product);
  if (isPositive == false || temp2.isPositive == false) {
    res.isPositive = false;
  }
  return res;
}

LargeNum LargeNum::operator/(const LargeNum &rhs) const {
  LargeNum temp1(num);
  LargeNum rhsCopy(rhs);
  LargeNum rhs2;
  rhs2.num = rhsCopy.num;
  temp1.num = num;
  int count = 0;
  if (temp1 < rhs) {
    LargeNum zero(0);
    return zero;
  }
  bool isNeg = false;
  if (temp1.isPositive == false && rhs.isPositive == false) {
    temp1.negate();
    rhsCopy.negate();
    isNeg = true;
  } else if ((temp1.isPositive == true && rhs.isPositive == false)) {
    rhsCopy.negate();
    isNeg = true;
  } else if ((temp1.isPositive == false && rhs.isPositive == true)) {
    temp1.negate();
    isNeg = true;
  }
  while (temp1 >= rhs2) {
    temp1 = temp1 - rhs2;
    rhs2.num = rhsCopy.num;
    count++;
  }
  LargeNum res(count);
  if (isNeg) {
    res.negate();
  }
  return res;
}

bool LargeNum::operator==(const LargeNum &rhs) const { return num == rhs.num; }

bool LargeNum::operator!=(const LargeNum &rhs) const { return !(*this == rhs); }

bool LargeNum::operator<(const LargeNum &rhs) const {
  LargeNum temp1(num);
  temp1.num = num;
  LargeNum rhsCopy(rhs);
  int n = temp1.num.length();
  int m = rhs.num.length();
  if (isPositive == false && rhsCopy.isPositive == true) {
    return true;
  } else if (isPositive == true && rhsCopy.isPositive == false) {
    return false;
  } else if (isPositive == false && rhsCopy.isPositive == false) {
    if (n != m) {
      return n > m;
    } else {
      temp1.negate();
      rhsCopy.negate();
      while (n--) {
        if (temp1.num.at(n) != rhsCopy.num.at(n)) {
          return temp1.num.at(n) > rhsCopy.num.at(n);
        }
      }
    }
  }
  if (n != m) {
    return n < m;
  }
  while (n--) {
    if (temp1.num.at(n) != rhs.num.at(n)) {
      return temp1.num.at(n) < rhs.num.at(n);
    }
  }
  return false;
}

bool LargeNum::operator>(const LargeNum &rhs) const {
  return (!(*this == rhs) || rhs < *this);
}

bool LargeNum::operator<=(const LargeNum &rhs) const {
  return (*this == rhs || *this < rhs);
}

bool LargeNum::operator>=(const LargeNum &rhs) const {
  return (!(*this < rhs) || *this == rhs);
}

LargeNum &LargeNum::operator++() {
  int i;
  int n = num.length();
  if (isPositive == false) {
    isPositive = true;
    LargeNum res;
    res = operator--();
    res.isPositive = false;
    *this = res;
    return *this;
  }
  for (i = 0; i < n && num.at(i) == '9'; i++)
    num.at(i) = '0';
  if (i == n)
    num.push_back('1');
  else
    num.at(i)++;
  return *this;
}

LargeNum LargeNum::operator++(int) {
  LargeNum temp1(*this);
  ++(*this);
  return temp1;
}

LargeNum &LargeNum::operator--() {
  int i;
  int n = num.length();
  if (num.length() == 1) {
    if (num.at(0) == '1') {
      LargeNum res(num);
      LargeNum temp(1);
      res.num = num;
      res = res - temp;
      *this = res;
      return *this;
    } else if (num.at(0) == '0') {
      LargeNum res(num);
      LargeNum temp(1);
      res.num = num;
      res = res + temp;
      res.negate();
      *this = res;
      return *this;
    }
  }
  if (isPositive == false) {
    isPositive = true;
    LargeNum res;
    res = operator++();
    res.isPositive = false;
    *this = res;
    return *this;
  }
  for (i = 0; num.at(i) == '0' && i < n; i++)
    num.at(i) = '9';
  num.at(i)--;
  if (n > 1 && num.at(n - 1) == '0')
    num.pop_back();
  return *this;
}

LargeNum LargeNum::operator--(int) {
  LargeNum temp1(*this);
  --(*this);
  return temp1;
}

ostream &operator<<(ostream &out, const LargeNum &num) {
  if (num.isZero() == true) {
    return out << "0";
  }
  if (num.isPositive == false) {
    out << '-';
  }
  if (num.num.length() == 1) {
    return out << num.num.at(0);
  } else if (num.num.length() == 2) {
    out << num.num.at(1);
    out << num.num.at(0);
    return out;
  } else if (num.num.length() == 3) {
    out << num.num.at(2);
    out << num.num.at(1);
    out << num.num.at(0);
    return out;
  }
  int index = num.num.length() % 3;
  if (index == 0) {
    index = 3;
  }
  for (int i = num.num.length() - 1; i >= num.num.length() - index; i--) {
    out << num.num.at(i);
  }
  out << ',';
  int count = 0;
  for (int i = num.num.length() - index - 1; i >= 0; i--) {
    if (count == 3) {
      out << ',';
      count = 0;
    }
    out << num.num.at(i);
    count++;
  }
  return out;
}
