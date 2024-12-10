/* -*- compile-command: "make -f ../../Makefile one.bin && ./one.bin < sample.txt" -*- */

#include "shared.h"
#include <map>
#include <set>

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
        const auto& destinations = score(p, h);
        sum += destinations.size();
      }
    }
    return sum;
  }

  void dump() const {
    for (const auto& [p, s] : destinations_) {
      dump(p, s);
    }
  }

private:
  void dump(const Point& p, const set<Point>& s) const {
    cout << p << "(" << map_.at(p) << "): [";
    for (const auto& p2 : s) {
      cout << p2 << ", ";
    }
    cout << endl;
  }

  set<Point> score(const Point& p, int h) {
    auto& d = destinations_[p];
    if (d.empty()) {
      if (h == 9) {
        d.insert(p);
      } else {
        update(d, Point(p.x() - 1, p.y()), h + 1);
        update(d, Point(p.x() + 1, p.y()), h + 1);
        update(d, Point(p.x(), p.y() - 1), h + 1);
        update(d, Point(p.x(), p.y() + 1), h + 1);
      }
    }
    return d;
  }

  void update(set<Point>& s, const Point& p, int h) {
    if (const auto it = map_.find(p); it != map_.end() && it->second == h) {
      const auto more = score(p, h);
      s.insert(more.begin(), more.end());
    }
  }

private:
  map<Point, int> map_;
  int cols_ = 0;
  int rows_ = 0;
  map<Point, set<Point>> destinations_;
};

int main() {
  Puzzle puzzle(cin);
  cout << puzzle.solve() << endl;
}
