/* -*- compile-command: "make -f ../../Makefile one.bin one.bin && ./one.bin < sample.txt" -*- */

#include <iostream>
#include <map>
#include <set>
#include <vector>

using namespace std;

class Point {
public:
  Point() = default;
  Point(int x, int y) : x_(x), y_(y) {}

  bool operator==(const Point& p) const {
    return x_ == p.x_ && y_ == p.y_;
  }

  bool operator<(const Point& p) const {
    return (x_ < p.x_) || (x_ == p.x_ && y_ < p.y_);
  }

  bool valid(int cols, int rows) const {
    return (x_ >= 0 && x_ < cols &&
            y_ >= 0 && y_ < rows);
  }

  int x() const { return x_; }
  int y() const { return y_; }

  Point antinode(const Point& p) const {
    return Point(2 * x_ - p.x_, 2 * y_ - p.y_);
  }

private:
  int x_;
  int y_;
};

ostream& operator<<(ostream& ostr, const Point& p) {
  return ostr << '[' << p.x() << ',' << p.y() << ']';
}

class Antinodes {
public:
  Antinodes(int cols, int rows) : cols_(cols), rows_(rows) {}

  int size() const { return points_.size(); }

  bool add(const Point& p) {
    if (p.valid(cols_, rows_)) {
      points_.insert(p);
      return true;
    }
    return false;
  }

private:
  const int cols_, rows_;
  set<Point> points_;
};

int main(int ac, const char* const * av) {
  map<char, vector<Point>> antennae;
  int cols = 0;
  int rows = 0;
  string line;
  for (int y = 0; getline(cin, line); ++y) {
    ++rows;
    cols = line.size();
    for (int x = 0; x < line.size(); ++x) {
      if (line[x] != '.') {
        antennae[line[x]].emplace_back(x, y);
      }
    }
  }

  Antinodes antinodes(cols, rows);
  for (const auto& v : antennae) {
    for (const auto& p1 : v.second) {
      for (const auto& p2 : v.second) {
        if (!(p1 == p2)) {
          antinodes.add(p1.antinode(p2));
        }
      }
    }
  }

  cout << antinodes.size() << endl;
}
