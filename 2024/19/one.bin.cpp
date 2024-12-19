/* -*- compile-command: "make -f ../../Makefile one.bin && ./one.bin < sample.txt" -*- */

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

  bool solve(string_view design) const {
    set<string_view> failures;
    return solve(failures, design, *this, 0);
  }

private:
  const Trie* get(char c) const {
    const auto it = next_.find(c);
    return it == next_.end() ? nullptr : &(it->second);
  }

  bool solve(set<string_view>& failures, string_view design, const Trie& base, int depth) const {
    // cout << "[" << design << "]: " << depth << endl;
    if (this == &base && failures.count(design) != 0) {
      return false;
    }
    if (design.empty()) {
      return !terminal_.empty();
    }
    if (!terminal_.empty() && failures.count(design) == 0) {
      if (base.solve(failures, design, base, depth + 100)) {
        return true;
      } else {
        failures.insert(design);
      }
    }
    if (const Trie* t = get(design[0])) {
      return t->solve(failures, design.substr(1), base, depth + 1);
    }
    return false;
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
  int sum = 0;
  while (getline(cin, line)) {
    cout << "solve: " << line << endl;
    if (t.solve(line)) {
      cout << "yes!" << endl;
      ++sum;
    }
  }
  cout << sum << endl;
}
