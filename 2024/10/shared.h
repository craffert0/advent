#pragma once

#include <iostream>

using namespace std;

class Point {
public:
  Point() = default;
  Point(int x, int y) : x_(x), y_(y) {}

  bool operator==(const Point& p) const {
    return x_ == p.x_ && y_ == p.y_;
  }

  bool operator!=(const Point& p) const {
    return !(*this == p);
  }

  bool operator<(const Point& p) const {
    return (y_ < p.y_) || (y_ == p.y_ && x_ < p.x_);
  }

  bool valid(int cols, int rows) const {
    return (x_ >= 0 && x_ < cols &&
            y_ >= 0 && y_ < rows);
  }

  int x() const { return x_; }
  int y() const { return y_; }

private:
  int x_;
  int y_;
};

ostream& operator<<(ostream& ostr, const Point& p) {
  return ostr << '[' << p.x() << ',' << p.y() << ']';
}
