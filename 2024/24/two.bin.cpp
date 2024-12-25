/* -*- compile-command: "make -f ../../Makefile two.bin && ./two.bin < sample.txt" -*- */

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

void out(const string& k) {
  cout << " " << k;
  const auto it = calcs.find(k);
  if (it != calcs.end()) {
    out(get<0>(it->second));
    out(get<2>(it->second));
  }
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
    cout << k << " ";
    out(get<0>(v));
    out(get<2>(v));
    cout << endl;
  }
}
