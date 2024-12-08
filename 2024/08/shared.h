#pragma once

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

  Point antinode(const Point& p, int harmonic) const {
    return Point(x_ + harmonic * (x_ - p.x_),
                 y_ + harmonic * (y_ - p.y_));
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

class Map {
public:
  using map_type = map<char, vector<Point>>;
  using const_iterator = map_type::const_iterator;

public:
  explicit Map(istream& istr) {
    string line;
    for (int y = 0; getline(istr, line); ++y) {
      ++rows_;
      cols_ = line.size();
      for (int x = 0; x < line.size(); ++x) {
        if (line[x] != '.') {
          antennae_[line[x]].emplace_back(x, y);
        }
      }
    }
  }

  const_iterator begin() const { return antennae_.begin(); }
  const_iterator end() const { return antennae_.end(); }

  int cols() const { return cols_; }
  int rows() const { return rows_; }

private:
  map_type antennae_;
  int cols_ = 0;
  int rows_ = 0;
};
