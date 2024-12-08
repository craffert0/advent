/* -*- compile-command: "make -f ../../Makefile one.bin one.bin && ./one.bin < sample.txt" -*- */

#include "shared.h"

int main(int ac, const char* const * av) {
  Map antennae(cin);

  Antinodes antinodes(antennae.cols(), antennae.rows());
  for (const auto& v : antennae) {
    for (const auto& p1 : v.second) {
      for (const auto& p2 : v.second) {
        if (!(p1 == p2)) {
          antinodes.add(p1.antinode(p2, 1));
        }
      }
    }
  }

  cout << antinodes.size() << endl;
}
