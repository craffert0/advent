/* -*- compile-command: "make -f ../../Makefile one.bin && ./one.bin < sample.txt" -*- */

#include <charconv>
#include <iostream>
#include <map>
#include <numeric>
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
  int cost_;
};

ostream& operator<<(ostream& o, const PriorityNode pn) {
  return o << pn.n_ << ": " << pn.cost_;
}

using DistanceMap = map<Node, int>;

struct PriorityGreater {
  bool operator()(const PriorityNode& a, const PriorityNode& b) {
    return a.cost_ > b.cost_;
  }
};

using PQ = priority_queue<PriorityNode, vector<PriorityNode>, PriorityGreater>;

class Validator {
public:
  virtual ~Validator() = default;
  virtual bool valid(const Node& n) const = 0;
};

class Solver {
public:
  Solver(const Validator& validator, const Node& start, const Node& end)
      : validator_(validator), end_(end) {
    dist_[start] = 0;
    push(start, 0);
  }

  bool done(int lowcost) const {
    return pq_.empty() || pq_.top().cost_ >= lowcost || dist_.find(end_) != dist_.end();
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

  int result() const {
    const auto it = dist_.find(end_);
    return it != dist_.end() ? it->second : 0;
  }

private:
  void check(const Node& n, int cost) {
    if (validator_.valid(n)) {
      const auto new_cost = cost + 1;
      auto& entry = dist_[n];
      if (entry == 0 || new_cost < entry) {
        entry = new_cost;
        push(n, new_cost);
      }
    }
  }

  void push(const Node& n, int cost) {
    const PriorityNode u = {n, cost};
    // cout << "push: " << u << endl;
    pq_.push(u);
  }

private:
  const Validator& validator_;
  const Node end_;
  PQ pq_;
  DistanceMap dist_;
};

class Maze : private Validator {
public:
  Maze(istream& istr) {
    load(istr);
  }

  void dump() const {
    for (const auto& row : maze_) {
      for (const char c : row) {
        cout << c;
      }
      cout << endl;
    }
  }

  void solve(int delta) {
    const int worst = solve1(numeric_limits<int>::max());
    const int lowcost =  worst - delta;
    int sum = 0;
    for (int y = 1; y < maze_.size() - 1; ++y) {
      auto& row = maze_[y];
      for (int x = 1; x < row.size() - 1; ++x) {
        if ((row[x] == '#') &&
            ((row[x-1] != '#') + (row[x+1] != '#') +
             (maze_[y-1][x] != '#') + (maze_[y+1][x] != '#')) > 1) {
          row[x] = '.';
          if (const int better = solve1(lowcost)) {
            ++sum;
            cout << Node(x, y) << ": " << worst - better << endl;
          }
          row[x] = '#';
        }
      }
    }
    cout << sum << endl;
  }

private:
  int solve1(int lowcost) {
    Solver s(*this, start_, end_);
    while (!s.done(lowcost)) {
      s.next();
    }
    return s.result();
  }

  bool valid(const Node& n) const override {
    return (x(n) >= 0 && y(n) >= 0 &&
            x(n) < maze_.size() && y(n) < maze_.size() &&
            maze_[y(n)][x(n)] != '#');
  }

  void load(istream& istr) {
    string line;
    for (int y = 0; getline(istr, line); ++y) {
      maze_.emplace_back();
      for (int x = 0; x < line.size(); ++x) {
        switch (line[x]) {
        case 'S':
          maze_[y].push_back('.');
          start_ = { x, y };
          break;
        case 'E':
          maze_[y].push_back('.');
          end_ = { x, y };
          break;
        default:
          maze_[y].push_back(line[x]);
          break;
        }
      }
    }
  }

private:
  vector<vector<char>> maze_;
  Node start_;
  Node end_;
};

int main(int ac, const char* av[]) {
  if (ac != 2) {
    cerr << "usage: maze delta" << endl;
    return 1;
  }
  const int delta = atoi(av[1]);
  Maze maze(cin);
  maze.solve(delta);
  // maze.dump();
}
