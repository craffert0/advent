/* -*- compile-command: "make -f ../../Makefile one.bin && ./one.bin 7 12 < sample.txt" -*- */

#include <charconv>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <vector>

using namespace std;

using Node = tuple<int, int>;
int x(const Node& n) { return get<0>(n); }
int y(const Node& n) { return get<1>(n); }
Node left(const Node& n) { return {x(n) - 1, y(n)}; }
Node right(const Node& n) { return {x(n) + 1, y(n)}; }
Node up(const Node& n) { return {x(n), y(n) - 1}; }
Node down(const Node& n) { return {x(n), y(n) + 1}; }

ostream& operator<<(ostream& o, const Node n) {
  return o << "[" << x(n) << ", " << y(n) << "]";
}

struct PriorityNode {
  Node n_;
  uint64_t cost_;
};

ostream& operator<<(ostream& o, const PriorityNode pn) {
  return o << pn.n_ << ": " << pn.cost_;
}

using DistanceMap = map<Node, uint64_t>;

struct PriorityGreater {
  bool operator()(const PriorityNode& a, const PriorityNode& b) {
    return a.cost_ > b.cost_;
  }
};

using PQ = priority_queue<PriorityNode, vector<PriorityNode>, PriorityGreater>;

class Maze {
public:
  Maze(int size, int count, istream& istr)
      : start_(0, 0), end_(size - 1, size - 1) {
    maze_ = vector<vector<char>>(size, vector<char>(size, '.'));
    load(count, istr);
    dist_[start_] = 0;
    push(start_, 0);
  }

  bool complete() const {
    if (pq_.empty()) {
      return true;
    }
    if (dist_.find(end_) != dist_.end()) {
      return true;
    }
    return false;
  }

  void next() {
    const auto u = pq_.top();
    pq_.pop();
    // cout << "pop: " << u << endl;
    if (dist_.at(u.n_) == u.cost_) {
      check(left(u.n_), u.cost_);
      check(right(u.n_), u.cost_);
      check(up(u.n_), u.cost_);
      check(down(u.n_), u.cost_);
    }
  }

  void dump() const {
    for (const auto& row : maze_) {
      for (const char c : row) {
        cout << c;
      }
      cout << endl;
    }
    const auto& e = end_;
    cout << x(e) << ", " << y(e) << ": ";
    const auto it = dist_.find(e);
    if (it != dist_.end()) {
      cout << it->second << endl;
    } else {
      cout << "nope!" << endl;
    }
  }

private:
  void push(const Node& n, uint64_t cost) {
    const PriorityNode u = {n, cost};
    // cout << "push: " << u << endl;
    pq_.push(u);
  }

  void check(const Node& n, uint64_t cost) {
    if (valid(n) && maze_[y(n)][x(n)] != '#') {
      const auto new_cost = cost + 1;
      auto& entry = dist_[n];
      if (entry == 0 || new_cost < entry) {
        entry = new_cost;
        push(n, new_cost);
      }
    }
  }

  bool valid(const Node& n) const {
    return x(n) >= 0 && y(n) >= 0 && x(n) < maze_.size() && y(n) < maze_.size();
  }

  void load(int count, istream& istr) {
    for (int i = 0; i < count && istr; ++i) {
      int x, y;
      char c;
      istr >> x >> c >> y;
      maze_[y][x] = '#';
    }
  }

private:
  vector<vector<char>> maze_;
  Node start_;
  Node end_;
  PQ pq_;
  DistanceMap dist_;
};

int main(int ac, const char* av[]) {
  if (ac != 3) {
    cerr << "usage: maze size count" << endl;
    return 1;
  }
  const int size = atoi(av[1]);
  const int count = atoi(av[2]);
  Maze maze(size, count, cin);
  int i = 0;
  while (!maze.complete() && i++ < 50000) {
    maze.next();
  }
  maze.dump();
}
