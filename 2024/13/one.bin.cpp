/* -*- compile-command: "make -f ../../Makefile one.bin && ./one.bin < sample.txt" -*- */

#include <cassert>
#include <charconv>
#include <iostream>
#include <string>
#include <string_view>

using namespace std;

static constexpr int kNoValue = 100 * 4 + 1;;

using Coordinate = pair<int, int>;

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

  Coordinate move(int n) const { return { n * x_, n * y_ }; }

public:
  int x_;
  int y_;
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
  }

  bool after(const Coordinate& c) const { return c.first < x_ && c.second < y_; }
  bool equals(const Coordinate& c) const { return c.first == x_ && c.second == y_; }
  bool overshot(const Coordinate& c) const { return c.first > x_ || c.second > y_; }

public:
  int x_;
  int y_;
};

ostream& operator<<(ostream& ostr, const Prize& p) {
  return ostr << '[' << p.x_ << ", " << p.y_ << ']';
}

int compute(const Button& a, const Button& b, const Prize& p) {
  int best = kNoValue;
  for (int i = 0; i < 100; ++i) {
    const auto a_move = a.move(i);
    if (p.overshot(a_move)) {
      break;
    }
    for (int j = 0; j < 100; ++j) {
      const auto total = ::plus(a_move, b.move(j));
      if (p.overshot(total)) {
        break;
      }
      if (p.equals(total)) {
        const int cost = i * a.cost_ + j * b.cost_;
        if (cost < best) {
          best = cost;
        }
        break;
      }
    }
  }
  return best;
}

int main() {
  int total = 0;
  string blank;
  do {
    string button_a;
    string button_b;
    string prize;
    assert(getline(cin, button_a) && getline(cin, button_b) && getline(cin, prize));
    cout << button_a << " + " << button_a << " -> " << prize << endl;
    const int cost = compute(Button(button_a, 3), Button(button_b, 1), Prize(prize));
    cout << cost << endl;
    if (cost != kNoValue) {
      total += cost;
    }
  } while (getline(cin, blank));
  cout << total << endl;
}
