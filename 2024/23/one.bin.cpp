/* -*- compile-command: "make -f ../../Makefile one.bin && ./one.bin < sample.txt" -*- */

#include <iostream>
#include <map>
#include <set>

using namespace std;

int main() {
  map<string, set<string>> connections;
  string line;
  while (getline(cin, line)) {
    const auto left = line.substr(0, 2);
    const auto right = line.substr(3, 5);
    if (left < right) {
      connections[left].insert(right);
    } else {
      connections[right].insert(left);
    }
  }
  int sum = 0;
  for (const auto& [a, bs] : connections) {
    for (const auto& b : bs) {
      const auto it = connections.find(b);
      if (it != connections.end()) {
        for (const auto& c : it->second) {
          if (bs.find(c) != bs.end()) {
            cout << a << "," << b << "," << c << endl;
            if (a[0] == 't' || b[0] == 't' || c[0] == 't') {
              ++sum;
            }
          }
        }
      }
    }
  }
  cout << sum << endl;
}
