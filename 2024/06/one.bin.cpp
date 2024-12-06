/* -*- compile-command: "c++ one.bin.cpp -o one.bin && ./one.bin < sample.txt" -*- */

#include "shared.h"

#include <set>

int main(int ac, const char* const * av) {
  Map m(cin);
  set<Point> locations;
  do {
    locations.insert(m.location());
    // m.dump();
    // cout << "----------------------------------------------------------------" << endl;
    m.move();
  } while (m.valid());
  cout << locations.size() << endl;
}
