/* -*- compile-command: "make -f ../../Makefile one.bin && ./one.bin < sample.txt" -*- */

#include <array>
#include <iostream>
#include <map>
#include <set>

using namespace std;

using Blob = array<int, 5>;
static set<Blob> locks;
static set<Blob> keys;

ostream& operator<<(ostream& o, const Blob& b) {
  for (int x : b) {
    o << x << " ";
  }
  return o;
}

static const int kMult[] = {
    10000, 1000, 100, 10, 1
};

static void read_one() {
  array<string, 7> input;
  for (auto& x : input) {
    getline(cin, x);
  }
  if (input[0] == ".....") {
    Blob key;
    for (int i = 0; i < 5; ++i) {
      for (int j = 0; j < 7; ++j) {
        if (input[6 - j][i] == '.') {
          key[i] = j;
          break;
        }
      }
    }
    // cout << "key: " << key << endl;
    keys.insert(key);
  } else {
    Blob lock;
    for (int i = 0; i < 5; ++i) {
      for (int j = 0; j < 7; ++j) {
        if (input[j][i] == '.') {
          lock[i] = j;
          break;
        }
      }
    }
    // cout << "lock: " << lock << endl;
    locks.insert(lock);
  }
}

static bool read_blank() {
  string line;
  return (bool)getline(cin, line);
}

static bool match(const Blob& a, const Blob& b) {
  for (int i = 0; i < 5; ++i) {
    if (a[i] + b[i] > 7) {
      return false;
    }
  }
  return true;
}

int main() {
  do {
    read_one();
  } while (read_blank());
  int sum = 0;
  for (const auto& k : keys) {
    for (const auto& l : locks) {
      if (match(k, l)) {
        cout << k << "+ " << l << endl;
        ++sum;
      }
    }
  }
  cout << sum << endl;
}
