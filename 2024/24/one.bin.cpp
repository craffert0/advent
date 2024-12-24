/* -*- compile-command: "make -f ../../Makefile one.bin && ./one.bin < sample.txt" -*- */

#include <iostream>
#include <map>
#include <set>

using namespace std;

class Func {
public:
  virtual ~Func() = default;
  virtual bool operator()(bool a, bool b) const = 0;
};

class XOR : public Func {
public:
  bool operator()(bool a, bool b) const override { return a ^ b; }
};

class OR : public Func {
public:
  bool operator()(bool a, bool b) const override { return a || b; }
};

class AND : public Func {
public:
  bool operator()(bool a, bool b) const override { return a && b; }
};

static const XOR kXOR;
static const OR kOR;
static const AND kAND;

map<string, bool> values;
map<string, tuple<string, const Func*, string>> calcs;

bool value(const string& k);

bool calc(const string& k) {
  const auto& [left, f, right] = calcs.at(k);
  return values[k] = (*f)(value(left), value(right));
}

bool value(const string& k) {
  const auto it = values.find(k);
  return it == values.end() ? calc(k) : it->second;
}

int main() {
  string line;
  while (getline(cin, line) && !line.empty()) {
    values[line.substr(0, 3)] = line[5] == '1';
  }
  while (getline(cin, line)) {
    const auto left = line.substr(0, 3);
    const auto right = line.substr(line.size() - 10, 3);
    const auto result = line.substr(line.size() - 3, 3);
    const Func* f = [&]() -> const Func* {
      switch (line[4]) {
      case 'A': return &kAND;
      case 'O': return &kOR;
      case 'X': return &kXOR;
      default: return nullptr;
      }
    }();
    calcs[result] = make_tuple(left, f, right);
  }
  for (const auto& [k, v] : calcs) {
    cout << k << ": " << calc(k) << endl;
  }
  cout << endl;
  uint64_t value = 0;
  for (const auto& [k, v] : values) {
    cout << k << ": " << v << endl;
    if (k[0] == 'z' && v) {
      value += ((uint64_t)0x01) << (10 * (k[1] - '0') + (k[2] - '0'));
    }
  }
  cout << endl << value << endl;
}
