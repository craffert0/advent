/* -*- compile-command: "make -k one.bin && ./one.bin < input.txt" -*- */

#include <iostream>
#include <sstream>
#include <vector>

int main(int ac, const char* const * av) {
  std::vector<int> left, right;
  std::string line;
  while (std::getline(std::cin, line)) {
    std::istringstream istr(line);
    int l, r;
    istr >> l >> r;
    left.push_back(l);
    right.push_back(r);
  }
  std::sort(left.begin(), left.end());
  std::sort(right.begin(), right.end());
  int sum = 0;
  for (int i = 0; i < left.size(); ++i) {
    sum += std::abs(left[i] - right[i]);
  }
  std::cout << sum << std::endl;
}
