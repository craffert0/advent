#pragma once

#include <cassert>
#include <iostream>
#include <vector>

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
    return (x_ < p.x_) || (x_ == p.x_ && y_ < p.y_);
  }

  bool valid(int cols, int rows) const {
    return (x_ >= 0 && x_ < cols &&
            y_ >= 0 && y_ < rows);
  }

  int x() const { return x_; }
  int y() const { return y_; }

  Point forward(char direction) const {
    switch (direction) {
    case '^':
      return Point(x_, y_ - 1);
    case '>':
      return Point(x_ + 1, y_);
    case 'v':
      return Point(x_, y_ + 1);
    case '<':
      return Point(x_ - 1, y_);
    default:
      assert(false);
    }
  }

private:
  int x_;
  int y_;
};

class Map {
public:
  explicit Map(istream& istr) {
    string line;
    for (int y = 0; getline(cin, line); ++y) {
      map_.push_back(vector<bool>());
      auto& row = map_.back();
      for (int x = 0; x < line.size(); ++x) {
        switch (line[x]) {
        case '.':
          row.push_back(false);
          break;
        case '#':
          row.push_back(true);
          break;
        case '^':
        case '>':
        case 'v':
        case '<':
          row.push_back(false);
          guard_ = Point(x, y);
          d_ = line[x];
          break;
        }
      }
    }
  }

  Map with_obstruction(Point p) {
    Map copy = *this;
    copy.map_[p.y()][p.x()] = true;
    return copy;
  }

  int rows() const { return map_.size(); }
  int cols() const { return map_[0].size(); }

  Point location() const { return guard_; }
  char direction() const { return d_; }
  bool obstruction(const Point& p) const {
    return map_[p.y()][p.x()];
  }

  bool valid() const { return valid(guard_); }

  void dump() const {
    for (int y = 0; y < map_.size(); ++y) {
      const auto& row = map_[y];
      for (int x = 0; x < row.size(); ++x) {
        if (guard_ == Point(x, y)) {
          cout << d_;
        } else if (row[x]) {
          cout << '#';
        } else {
          cout << '.';
        }
      }
      cout << endl;
    }
  }

  void move() {
    assert(valid());
    if (blocked()) {
      turn_right();
    } else {
      guard_ = guard_.forward(d_);
    }
  }

private:
  bool valid(const Point& p) const {
    return p.valid(rows(), cols());
  }

  bool blocked() const {
    const auto next = guard_.forward(d_);
    return valid(next) && obstruction(next);;
  }

  void turn_right() {
    switch (d_) {
    case '^':
      d_ = '>';
      break;
    case '>':
      d_ = 'v';
      break;
    case 'v':
      d_ = '<';
      break;
    case '<':
      d_ = '^';
      break;
    }
  }

private:
  vector<vector<bool> > map_;
  Point guard_;
  char d_;
};
