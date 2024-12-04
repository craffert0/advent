/* -*- compile-command: "make -k one.bin && ./one.bin < input.txt" -*- */

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

  int horizontal_ltr(std::string_view match) const {
    const int match_size = match.size();
    int sum = 0;
    for (int i = 0; i < height_; ++i) {
      const std::string_view line = lines_[i];
      for (int j = 0; j < width_ - (match_size - 1); ++j) {
        if (line.substr(j, match_size) == match) {
          ++sum;
        }
      }
    }
    std::cout << sum << std::endl;
    return sum;
  }

  int horizontal_rtl(std::string_view match) const {
    const int match_size = match.size();
    int sum = 0;
    for (int i = 0; i < height_; ++i) {
      for (int j = match_size - 1; j < width_; ++j) {
        bool did_match = true;
        for (int k = 0; did_match && k < match_size; ++k) {
          did_match &= (lines_[i][j - k] == match[k]);
        }
        if (did_match) {
          ++sum;
        }
      }
    }
    std::cout << sum << std::endl;
    return sum;
  }

  int verticle_ttb(std::string_view match) const {
    const int match_size = match.size();
    int sum = 0;
    for (int i = 0; i < width_; ++i) {
      for (int j = 0; j < height_ - (match_size - 1); ++j) {
        bool did_match = true;
        for (int k = 0; did_match && k < match_size; ++k) {
          did_match &= (lines_[j + k][i] == match[k]);
        }
        if (did_match) {
          ++sum;
        }
      }
    }
    std::cout << sum << std::endl;
    return sum;
  }

  int verticle_btt(std::string_view match) const {
    const int match_size = match.size();
    int sum = 0;
    for (int i = 0; i < width_; ++i) {
      for (int j = match_size - 1; j < height_; ++j) {
        bool did_match = true;
        for (int k = 0; did_match && k < match_size; ++k) {
          did_match &= (lines_[j - k][i] == match[k]);
        }
        if (did_match) {
          ++sum;
        }
      }
    }
    std::cout << sum << std::endl;
    return sum;
  }

  int diagonal_ttb_ltr(std::string_view match) const {
    const int match_size = match.size();
    int sum = 0;
    for (int i = 0; i < width_ - (match_size - 1); ++i) {
      for (int j = 0; j < height_ - (match_size - 1); ++j) {
        bool did_match = true;
        for (int k = 0; did_match && k < match_size; ++k) {
          did_match &= (lines_[j + k][i + k] == match[k]);
        }
        if (did_match) {
          ++sum;
        }
      }
    }
    std::cout << sum << std::endl;
    return sum;
  }

  int diagonal_btt_rtl(std::string_view match) const {
    const int match_size = match.size();
    int sum = 0;
    for (int i = match_size - 1; i < width_; ++i) {
      for (int j = match_size - 1; j < height_; ++j) {
        bool did_match = true;
        for (int k = 0; did_match && k < match_size; ++k) {
          did_match &= (lines_[j - k][i - k] == match[k]);
        }
        if (did_match) {
          ++sum;
        }
      }
    }
    std::cout << sum << std::endl;
    return sum;
  }

  int diagonal_ttb_rtl(std::string_view match) const {
    const int match_size = match.size();
    int sum = 0;
    for (int i = match_size - 1; i < width_; ++i) {
      for (int j = 0; j < height_ - (match_size - 1); ++j) {
        bool did_match = true;
        for (int k = 0; did_match && k < match_size; ++k) {
          did_match &= (lines_[j + k][i - k] == match[k]);
        }
        if (did_match) {
          ++sum;
        }
      }
    }
    std::cout << sum << std::endl;
    return sum;
  }

  int diagonal_btt_ltr(std::string_view match) const {
    const int match_size = match.size();
    int sum = 0;
    for (int i = 0; i < width_ - (match_size - 1); ++i) {
      for (int j = match_size - 1; j < height_; ++j) {
        bool did_match = true;
        for (int k = 0; did_match && k < match_size; ++k) {
          did_match &= (lines_[j - k][i + k] == match[k]);
        }
        if (did_match) {
          ++sum;
        }
      }
    }
    std::cout << sum << std::endl;
    return sum;
  }

private:
  std::vector<std::string> lines_;
  int width_;
  int height_;
};


int main(int ac, const char* const * av) {
  const Matrix m(std::cin);
  int sum = 0;
  sum += m.horizontal_ltr("XMAS");
  sum += m.horizontal_rtl("XMAS");
  sum += m.verticle_ttb("XMAS");
  sum += m.verticle_btt("XMAS");
  sum += m.diagonal_ttb_ltr("XMAS");
  sum += m.diagonal_btt_rtl("XMAS");
  sum += m.diagonal_ttb_rtl("XMAS");
  sum += m.diagonal_btt_ltr("XMAS");
  std::cout << sum << std::endl;
}
