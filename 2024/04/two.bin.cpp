/* -*- compile-command: "make -k two.bin && ./two.bin < input.txt" -*- */

#include <iostream>
#include <sstream>
#include <string_view>
#include <vector>

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
    for (int x = 0; x < width_ - 2; ++x) {
      for (int y = 0; y < height_ - 2; ++y) {
        sum += (m(x, y, "MMSS") ||
                m(x, y, "SMMS") ||
                m(x, y, "SSMM") ||
                m(x, y, "MSSM")) ? 1 : 0;
      }
    }
    return sum;
  }

private:
  bool m(int x, int y, const char* s) const {
    return (lines_[y + 1][x + 1] == 'A' &&
            lines_[y][x] == s[0] &&
            lines_[y][x + 2] == s[1] &&
            lines_[y + 2][x + 2] == s[2] &&
            lines_[y + 2][x] == s[3]);
  }

private:
  std::vector<std::string> lines_;
  int width_;
  int height_;
};

int main(int ac, const char* const * av) {
  std::cout << Matrix(std::cin).count() << std::endl;
}
