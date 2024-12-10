/* -*- compile-command: "make -f ../../Makefile two.bin && ./two.bin < sample.txt" -*- */

#include "shared.h"
#include <map>

class Puzzle {
public:
  explicit Puzzle(istream& istr) {
    string line;
    for (int y = 0; getline(istr, line); ++y) {
      ++rows_;
      cols_ = line.size();
      for (int x = 0; x < line.size(); ++x) {
        if (line[x] != '.') {
          map_[Point(x, y)] = line[x] - '0';
        }
      }
    }
  }

  int solve() {
    int sum = 0;
    for (const auto& [p, h] : map_) {
      if (h == 0) {
        const int s = score(p, h);
        sum += s;
      }
    }
    return sum;
  }

  void dump() const {
    for (const auto& [p, s] : scores_) {
      cout << p << "(" << map_.at(p) << "): " << s << endl;
    }
  }

private:
  int score(const Point& p, int h) {
    if (h == 9) {
      return 1;
    }
    if (const auto it = scores_.find(p); it != scores_.end()) {
      return it->second;
    }
    int sum =
        subscore(Point(p.x() - 1, p.y()), h + 1) +
        subscore(Point(p.x() + 1, p.y()), h + 1) +
        subscore(Point(p.x(), p.y() - 1), h + 1) +
        subscore(Point(p.x(), p.y() + 1), h + 1);
    scores_[p] = sum;
    return sum;
  }

  int subscore(const Point& p, int h) {
    if (const auto it = map_.find(p); it != map_.end() && it->second == h) {
      return score(p, h);
    } else {
      return 0;
    }
  }

private:
  map<Point, int> map_;
  int cols_ = 0;
  int rows_ = 0;
  map<Point, int> scores_;
};

int main() {
  Puzzle puzzle(cin);
  cout << puzzle.solve() << endl;
}
