/* -*- compile-command: "make -f ../../Makefile two.bin && ./two.bin < sample.txt" -*- */

#include <iostream>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <vector>

using namespace std;

class Trie {
public:
  void add(string_view towel) {
    Trie* t = this;
    for (string_view p = towel; !p.empty();
         t = &t->next_[p[0]], p = p.substr(1)) {
    }
    t->terminal_ = towel;
  }

  void dump() const {
    if (!terminal_.empty()) {
      cout << terminal_ << endl;
    }
    for (const auto& [c, p] : next_) {
      p.dump();
    }
  }

  uint64_t solve(string_view design) const {
    map<string_view, uint64_t> cache;
    return solve(cache, design, *this, 0);
  }

private:
  const Trie* get(char c) const {
    const auto it = next_.find(c);
    return it == next_.end() ? nullptr : &(it->second);
  }

  uint64_t solve(map<string_view, uint64_t>& cache,
                 string_view design, const Trie& base, int depth) const {
    // cout << "[" << design << "]: " << depth << endl;
    if (this == &base) {
      const auto it = cache.find(design);
      if (it != cache.end()) {
        return it->second;
      }
    }
    if (design.empty()) {
      return terminal_.empty() ? 0 : 1;
    }

    uint64_t count = 0;
    if (!terminal_.empty()) {
      const auto subcount = base.solve(cache, design, base, depth + 100);
      count += subcount;
      cache[design] = subcount;
    }
    if (const Trie* t = get(design[0])) {
      count += t->solve(cache, design.substr(1), base, depth + 1);
    }
    return count;
  }

private:
  string terminal_;
  map<char, Trie> next_;
};

static void load(Trie& t, string_view input) {
  string_view::size_type p;
  while ((p = input.find(',')) != string_view::npos) {
    t.add(input.substr(0, p));
    input = input.substr(p + 2);
  }
  t.add(input);
}

int main() {
  string line;
  getline(cin, line);
  Trie t;
  load(t, line);
  t.dump();
  getline(cin, line);
  uint64_t sum = 0;
  while (getline(cin, line)) {
    cout << "solve: " << line << endl;
    const auto subcount = t.solve(line);
    cout << subcount << endl;
    sum += subcount;
  }
  cout << sum << endl;
}
