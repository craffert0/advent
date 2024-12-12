/* -*- compile-command: "make -f ../../Makefile one.bin && ./one.bin < sample.txt" -*- */

#include <iostream>
#include <map>
#include <set>

using namespace std;

class Point {
public:
  Point() = default;
  Point(int x, int y) : x_(x), y_(y) {}

  Point up() const { return Point(x_, y_ - 1); }
  Point down() const { return Point(x_, y_ + 1); }
  Point left() const { return Point(x_ - 1, y_); }
  Point right() const { return Point(x_ + 1, y_); }

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

private:
  int x_;
  int y_;
};

ostream& operator<<(ostream& ostr, const Point& p) {
  return ostr << '[' << p.x() << ',' << p.y() << ']';
}

using BitMap = vector<vector<bool>>;
class Region : public set<Point> {
public:
  explicit Region(char c) : c_(c) {}

  char c() const { return c_; }

private:
  char c_;
};

class Garden {
public:
  explicit Garden(istream& istr) {
    string line;
    for (int y = 0; getline(istr, line); ++y) {
      plots_.emplace_back();
      for (char c : line) {
        plots_[y].push_back(c);
      }
    }

    BitMap handled(plots_.size(), vector<bool>(plots_[0].size(), false));
    for (int y = 0; y < plots_.size(); ++y) {
      for (int x = 0; x < plots_[y].size(); ++x) {
        if (!handled[y][x]) {
          const Point p(x, y);
          regions_.emplace_back(plots_[y][x]);
          Region& r = regions_.back();
          expand(r, p, handled);
        }
      }
    }
  }

  void dump() const {
    for (const auto& row : plots_) {
      for (const char c : row) {
        cout << c;
      }
      cout << endl;
    }
    int64_t sum = 0;
    for (const auto& r : regions_) {
      int walls = 0;
      for (const auto& p : r) {
        walls += same(r.c(), p.up()) ? 0 : 1;
        walls += same(r.c(), p.down()) ? 0 : 1;
        walls += same(r.c(), p.left()) ? 0 : 1;
        walls += same(r.c(), p.right()) ? 0 : 1;
      }
      sum += (r.size() * walls);
      cout << r.c() << '('
           << r.size() << " * " << walls << " = " << (r.size() * walls)
           << "):";
      for (const auto& p : r) {
        cout << ' ' << p;
      }
      cout << endl;
    }
    cout << sum << endl;
  }

private:
  bool valid(const Point& p) const {
    return p.valid(plots_.size(), plots_[0].size());
  }

  bool same(char c, const Point& p) const {
    return valid(p) && c == plots_[p.y()][p.x()];
  }

  void expand(Region& r, const Point& p, BitMap& handled) {
    handled[p.y()][p.x()] = true;
    r.insert(p);
    expand(r, p, handled, p.up());
    expand(r, p, handled, p.down());
    expand(r, p, handled, p.left());
    expand(r, p, handled, p.right());
  }

  void expand(Region& r, const Point& p, BitMap& handled, const Point& n) {
    if (same(plots_[p.y()][p.x()], n) &&
        !handled[n.y()][n.x()]) {
      expand(r, n, handled);
    }
  }

private:
  vector<vector<char>> plots_;
  vector<Region> regions_;
};

int main() {
  Garden garden(cin);
  garden.dump();
}
