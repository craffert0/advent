/* -*- compile-command: "make -f ../../Makefile four.bin && ./four.bin < sample.txt" -*- */

#include <cassert>
#include <charconv>
#include <iostream>
#include <numeric>
#include <string>
#include <string_view>

using namespace std;

static constexpr uint64_t kNoValue = numeric_limits<uint64_t>::max();
static constexpr uint64_t kOffset = 10000000000000;

using Coordinate = pair<uint64_t, uint64_t>;

Coordinate plus(const Coordinate& a, const Coordinate& b) {
  return { a.first + b.first, a.second + b.second };
}

class Button {
public:
  Button(const string_view& line, int cost) : cost_(cost) {
    // "Button A: X+94, Y+34"

    // "94, Y+34"
    const auto xplus = line.substr(1 + line.find('+'));
    // "94"
    const auto x = xplus.substr(0, xplus.find(','));

    // "34"
    const auto y = xplus.substr(1 + xplus.find('+'));

    from_chars(x.begin(), x.end(), x_);
    from_chars(y.begin(), y.end(), y_);
  }

  Coordinate move(uint64_t n) const { return { n * x_, n * y_ }; }

public:
  uint64_t x_;
  uint64_t y_;
  int cost_;
};

ostream& operator<<(ostream& ostr, const Button& b) {
  return ostr << '[' << b.x_ << ", " << b.y_ << ": " << b.cost_ << ']';
}

class Prize {
public:
  explicit Prize(const string_view& line) {
    // Prize: X=8400, Y=5400

    // "8400, Y=5400"
    const auto xeq = line.substr(1 + line.find('='));
    // "8400"
    const auto x = xeq.substr(0, xeq.find(','));

    // "5400"
    const auto y = xeq.substr(1 + xeq.find('='));

    from_chars(x.begin(), x.end(), x_);
    from_chars(y.begin(), y.end(), y_);
    x_ += kOffset;
    y_ += kOffset;
  }

  bool after(const Coordinate& c) const { return c.first < x_ && c.second < y_; }
  bool equals(const Coordinate& c) const { return c.first == x_ && c.second == y_; }
  bool overshot(const Coordinate& c) const { return c.first > x_ || c.second > y_; }

  Coordinate subtract(const Coordinate& c) const {
    return {x_ - c.first, y_ - c.second};
  }

public:
  uint64_t x_;
  uint64_t y_;
};

ostream& operator<<(ostream& ostr, const Prize& p) {
  return ostr << '[' << p.x_ << ", " << p.y_ << ']';
}

uint64_t compute(const Button a, const Button b, const Prize p) {
  uint64_t best = kNoValue;
  for (uint64_t x = 0, y = 0; x < p.x_ && y < p.y_; x += a.x_, y += a.y_) {
    const auto xb = p.x_ - x;
    // if (xb % b.x_ == 0) {
    //   const auto j = xb / b.x_;
    //   if (j * b.y_ + y == p.y_) {
    //     const auto i = (x / a.x_);
    //     cout << i << ", " << j << endl;
    //     const uint64_t cost = i * a.cost_ + j * b.cost_;
    //     if (cost < best) {
    //       best = cost;
    //     }
    //   }
    // }
  }
  return best;
}

int main() {
  uint64_t total = 0;
  string blank;
  do {
    string button_a;
    string button_b;
    string prize;
    assert(getline(cin, button_a) && getline(cin, button_b) && getline(cin, prize));
    cout << button_a << " + " << button_b << " -> " << prize << endl;
    const uint64_t cost = compute(Button(button_a, 3), Button(button_b, 1), Prize(prize));
    cout << cost << endl;
    if (cost != kNoValue) {
      total += cost;
    }
  } while (getline(cin, blank));
  cout << total << endl;
}
