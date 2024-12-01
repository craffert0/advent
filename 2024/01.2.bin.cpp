/* -*- compile-command: "make -k 01.2.bin && ./01.2.bin < 01.input.1.txt" -*- */

#include <iostream>
#include <map>
#include <sstream>
#include <vector>

int main(int ac, const char* const * av) {
  std::vector<int> left;
  std::map<int, int> right;
  std::string line;
  while (std::getline(std::cin, line)) {
    std::istringstream istr(line);
    int l, r;
    istr >> l >> r;
    left.push_back(l);
    right[r] += 1;
  }
  std::sort(left.begin(), left.end());
  int sum = 0;
  for (int i = 0; i < left.size(); ++i) {
    sum += left[i] * right[left[i]];
  }
  std::cout << sum << std::endl;
}
