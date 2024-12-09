/* -*- compile-command: "make -f ../../Makefile two.bin && ./two.bin < sample.txt" -*- */

#include <iostream>
#include <map>
#include <tuple>

using namespace std;

using File = tuple<int, int>; // id/size

int find_first(const map<int, int>& frees, int desired_size) {
  for (const auto [location, size] : frees) {
    if (desired_size <= size) {
      return location;
    }
  }
  return -1;
}

void dump(const map<int, File, greater<int>>& files,
          const map<int, int>& frees,
          int total) {
  return;
  for (int start = 0; start < total;) {
    const auto free_it = frees.find(start);
    const auto files_it = files.find(start);
    if (free_it != frees.end()) {
      const int freed = free_it->second;
      for (int j = 0; j < freed; ++j) {
        cout << '.';
      }
      start += freed;
    } else if (files_it != files.end()) {
      const auto& file = files_it->second;
      for (int j = 0; j < get<1>(file); ++j) {
        cout << (get<0>(file) % 10);
      }
      start += get<1>(file);
    } else {
      cout << " wtaf: " << start << endl;
      return;
    }
  }
  cout << endl;
}

int main(int ac, const char* const * av) {
  map<int, File, greater<int>> files; // start/File
  map<int, int> frees; // start, size
  string line;
  getline(cin, line);
  int total = 0;
  for (int i = 0, free = 0, id = 0; i < line.size(); ++i, free = 1 - free) {
    int size = line[i] - '0';
    if (!free) {
      files[total] = File(id++, size);
    } else if (size > 0) {
      frees[total] = size;
    }
    total += size;
  }

  auto files2 = files;
  dump(files2, frees, total);

  for (const auto& [old_loc, f] : files) {
    const int size = get<1>(f);
    const int new_loc = find_first(frees, size);
    if (new_loc >= 0 && new_loc < old_loc) {
      files2.erase(old_loc);
      files2[new_loc] = f;
      frees[old_loc] = size;

      const int old_free = frees[new_loc];
      frees.erase(new_loc);
      const int new_free_size = old_free - size;
      if (new_free_size > 0) {
        frees[new_loc + size] = new_free_size;
      }
      dump(files2, frees, total);
    }
  }

  int64_t sum = 0;
  for (const auto& [loc, f] : files2) {
    const int id = get<0>(f);
    const int size = get<1>(f);
    for (int64_t i = 0; i < size; ++i) {
      sum += (loc + i) * id;
    }
  }
  cout << sum << endl;
}
