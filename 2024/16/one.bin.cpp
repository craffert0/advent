/* -*- compile-command: "make -f ../../Makefile one.bin one.bin && ./one.bin < sample.txt" -*- */

// https://en.wikipedia.org/wiki/Dijkstra's_algorithm#Algorithm

#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <vector>

using namespace std;

enum class Direction {
    North, East, South, West,
};

ostream& operator<<(ostream& o, Direction d) {
  switch (d) {
  case Direction::North:
    return o << "North";
  case Direction::East:
    return o << "East";
  case Direction::South:
    return o << "South";
  case Direction::West:
    return o << "West";
  }
}

Direction left(Direction d) {
  switch (d) {
  case Direction::North:
    return Direction::West;
  case Direction::East:
    return Direction::North;
  case Direction::South:
    return Direction::East;
  case Direction::West:
    return Direction::South;
  }
}

Direction right(Direction d) {
  switch (d) {
  case Direction::North:
    return Direction::East;
  case Direction::East:
    return Direction::South;
  case Direction::South:
    return Direction::West;
  case Direction::West:
    return Direction::North;
  }
}

using Node = tuple<int, int, Direction>;
int x(const Node& n) { return get<0>(n); }
int y(const Node& n) { return get<1>(n); }
Direction d(const Node& n) { return get<2>(n); }

ostream& operator<<(ostream& o, const Node n) {
  return o << "[" << x(n) << ", " << y(n) << ", " << d(n) << "]";
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
  explicit Maze(istream& istr) {
    load(istr);
    dist_[start_] = 0;
    push(start_, 0);
  }

  bool complete() const {
    if (pq_.empty()) {
      return true;
    }
    for (const auto& e : ends_) {
      if (dist_.find(e) != dist_.end()) {
        return true;
      }
    }
    return false;
  }

  void next() {
    const auto u = pq_.top();
    pq_.pop();
    // cout << "pop: " << u << endl;
    if (dist_.at(u.n_) == u.cost_) {
      check_left(u);
      check_right(u);
      check_forward(u);
    }
  }

  void dump() const {
    for (const auto& e : ends_) {
      cout << x(e) << ", " << y(e) << ", " << d(e) << ": ";
      const auto it = dist_.find(e);
      if (it != dist_.end()) {
        cout << it->second << endl;
      } else {
        cout << "nope!" << endl;
      }
    }
  }

private:
  void push(const Node& n, uint64_t cost) {
    const PriorityNode u = {n, cost};
    // cout << "push: " << u << endl;
    pq_.push(u);
  }

  void check_left(const PriorityNode& u) {
    const auto new_cost = u.cost_ + 1000;
    const Node n = {x(u.n_), y(u.n_), left(d(u.n_))};
    auto& entry = dist_[n];
    if (entry == 0 || new_cost < entry) {
      entry = new_cost;
      push(n, new_cost);
    }
  }

  void check_right(const PriorityNode& u) {
    const auto new_cost = u.cost_ + 1000;
    const Node n = {x(u.n_), y(u.n_), right(d(u.n_))};
    auto& entry = dist_[n];
    if (entry == 0 || new_cost < entry) {
      entry = new_cost;
      push(n, new_cost);
    }
  }

  void check_forward(const PriorityNode& u) {
    Node next;
    const auto dir = d(u.n_);
    switch (d(u.n_)) {
    case Direction::North:
      next = {x(u.n_), y(u.n_) - 1, dir};
      break;
    case Direction::East:
      next = {x(u.n_) + 1, y(u.n_), dir};
      break;
    case Direction::South:
      next = {x(u.n_), y(u.n_) + 1, dir};
      break;
    case Direction::West:
      next = {x(u.n_) - 1, y(u.n_), dir};
      break;
    }
    if (maze_[y(next)][x(next)] != '#') {
      const auto new_cost = u.cost_ + 1;
      auto& entry = dist_[next];
      if (entry == 0 || new_cost < entry) {
        entry = new_cost;
        push(next, new_cost);
      }
    }
  }

  void load(istream& istr) {
    string line;
    for (int y = 0; getline(istr, line); ++y) {
      maze_.emplace_back();
      for (int x = 0; x < line.size(); ++x) {
        maze_[y].push_back(line[x]);
        switch (line[x]) {
        case 'S':
          start_ = { x, y, Direction::East };
          break;
        case 'E':
          ends_.emplace(x, y, Direction::North);
          ends_.emplace(x, y, Direction::East);
          ends_.emplace(x, y, Direction::South);
          ends_.emplace(x, y, Direction::West);
          break;
        }
      }
    }
  }

private:
  vector<vector<char>> maze_;
  Node start_;
  set<Node> ends_;
  PQ pq_;
  DistanceMap dist_;
};

int main() {
  Maze maze(cin);
  int i = 0;
  while (!maze.complete() && i++ < 50000) {
    maze.next();
  }
  maze.dump();
}
