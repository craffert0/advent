/* -*- compile-command: "make -f ../../Makefile one.bin && ./one.bin < sample.txt" -*- */

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

static void dump(const vector<int>& buffer) {
  for (int x : buffer) {
    if (x < 0) {
      cout << '.';
    } else {
      cout << (x % 10);
    }
  }
  cout << endl;
}

int main(int ac, const char* const * av) {
  string line;
  getline(cin, line);
  // cout << '[' << line << "]\n";
  int total = 0;
  for (char c : line) {
    total += (c - '0');
  }
  cout << line.size() << ": " << total << endl;
  vector<int> buffer(total, -1);
  bool free = false;
  int id = 0;
  int i = 0;
  for (char c : line) {
    const int count = c - '0';
    if (!free) {
      fill_n(buffer.begin() + i, count, id);
      ++id;
    }
    i += count;
    free = !free;
  }
  dump(buffer);
  for (int left = 0, right = total - 1; left < right;) {
    if (buffer[left] != -1) {
      ++left;
    } else if (buffer[right] == -1) {
      --right;
    } else {
      buffer[left] = buffer[right];
      buffer[right] = -1;
      ++left;
      --right;
    }
  }
  dump(buffer);
  int64_t sum = 0;
  for (int64_t i = 0; i < total && buffer[i] != -1; ++i) {
    sum += i * buffer[i];
  }
  cout << sum << endl;
}
