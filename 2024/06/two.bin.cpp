/* -*- compile-command: "c++ -std=c++17 two.bin.cpp -o two.bin && ./two.bin < sample.txt" -*- */

#include "shared.h"

#include <set>
#include <tuple>

using PD = tuple<Point, char>;

static bool has_loop(Map m) {
  set<PD> seen;
  do {
    const PD here(m.location(), m.direction());
    if (seen.count(here) > 0) {
      return true;
    }
    seen.insert(here);
    m.move();
  } while (m.valid());
  return false;
}

int main(int ac, const char* const * av) {
  Map m(cin);
  int count = 0;
  for (int x = 0; x < m.rows(); ++x) {
    for (int y = 0; y < m.cols(); ++y) {
      const Point p(x, y);
      if (p != m.location() && !m.obstruction(p)) {
        Map m2 = m.with_obstruction(p);
        if (has_loop(m2)) {
          // m2.dump();
          ++count;
        }
      }
    }
  }
  cout << count << endl;
}
