/* -*- compile-command: "make -k better-one.bin && ./better-one.bin < input.txt" -*- */

#include <iostream>
#include <sstream>

struct Point {
  int x;
  int y;
};

static Point matches[4][4] = {
    // horizontal
    {{0, 0}, {0, 1}, {0, 2}, {0, 3}},
    // vertical
    {{0, 0}, {1, 0}, {2, 0}, {3, 0}},
    // rtl diagonal
    {{0, 0}, {1, 1}, {2, 2}, {3, 3}},
    // ltr diagonal
    {{0, 3}, {1, 2}, {2, 1}, {3, 0}},
};

class Matrix {
public:
  explicit Matrix(std::istream& istr) {
    std::string line;
    while (std::getline(istr, line)) {
      lines_.push_back(line);
    }
    width_ = lines_.size();
    height_ = lines_[0].size();
  }

  int count() const {
    int sum = 0;
    for (int x = 0; x < width_; ++x) {
      for (int y = 0; y < height_; ++y) {
        for (const auto& m : matches) {
          if (valid(x, y, m)) {
            // forwards
            if (c(x, y, m[0]) == 'X' &&
                c(x, y, m[1]) == 'M' &&
                c(x, y, m[2]) == 'A' &&
                c(x, y, m[3]) == 'S') {
              sum += 1;
            }
            // backwards
            if (c(x, y, m[3]) == 'X' &&
                c(x, y, m[2]) == 'M' &&
                c(x, y, m[1]) == 'A' &&
                c(x, y, m[0]) == 'S') {
              sum += 1;
            }
          }
        }
      }
    }
    return sum;
  }

private:
  char c(int x, int y, const Point p) const {
    return lines_[y + p.y][x + p.x];
  }

  bool valid(int x, int y, const Point* m) const {
    for (int i = 0; i < 4; ++i) {
      if (y + m[i].y >= height_ || x + m[i].x >= width_) {
        return false;
      }
    }
    return true;
  }

private:
  std::vector<std::string> lines_;
  int width_;
  int height_;
};

int main(int ac, const char* const * av) {
  std::cout << Matrix(std::cin).count() << std::endl;
}
