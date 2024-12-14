/* -*- compile-command: "make -f ../../Makefile two.bin && ./two.bin < input.txt" -*- */

#include <array>
#include <charconv>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

ostream& operator<<(ostream& o, const pair<int, int>& p) {
  return o << '[' << p.first << ',' << p.second << ']';
}

class Robot {
public:
  explicit Robot(const string_view line) {
    // "p=0,4 v=3,-3"

    const auto first_comma = line.find(',');
    from_chars(line.begin() + 2, line.end(), px_);
    from_chars(line.begin() + first_comma + 1, line.end(), py_);

    const auto v = line.find('v');
    const auto second_comma = line.find(',', v);
    from_chars(line.begin() + v + 2, line.end(), vx_);
    from_chars(line.begin() + second_comma + 1, line.end(), vy_);
  }

  pair<int, int> move(int n, int width, int height) const {
    return {((n + 1) * width + px_ + n * vx_) % width, ((n + 1) * height + py_ + n * vy_) % height};
  }

private:
  int px_;
  int py_;
  int vx_;
  int vy_;
};

static bool close(const array<array<bool, 101>, 103>& M, int closeness) {
  for (const auto& row : M) {
    int count = 0;
    for (const bool b : row) {
      if (!b) {
        count = 0;
      } else if (++count == closeness) {
        return true;
      }
    }
  }
  return false;
}

int main() {
  int width, height;

  string line;
  getline(cin, line);
  istringstream istr(line);
  istr >> width >> height;
  const int w = width;
  const int h = height;

  vector<Robot> robots;
  while (getline(cin, line)) {
    robots.emplace_back(line);
  }
  for (int x = 1; x <= height * width; ++x) {
    array<array<bool, 101>, 103> M = {};
    for (const auto& r : robots) {
      const auto p = r.move(x, width, height);
      M[p.second][p.first] = true;
    }
    if (close(M, 6)) {
      cout << "================================ " << x << " ================================" << endl;
      for (const auto& row : M) {
        for (const bool b : row) {
          cout << (b ? 'X' : '.');
        }
        cout << endl;
      }
    }
  }
}
