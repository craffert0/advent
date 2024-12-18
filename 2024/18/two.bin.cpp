/* -*- compile-command: "make -f ../../Makefile two.bin && ./two.bin 7 12 < sample.txt" -*- */

#include <iostream>
#include <map>
#include <queue>
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

using DistanceMap = map<Node, bool>;

using Q = queue<Node>;

class Maze {
public:
  Maze(int size)
      : start_(0, 0), end_(size - 1, size - 1) {
    maze_ = vector<vector<char>>(size, vector<char>(size, '.'));
  }

  void add(const Node& n) {
    maze_[y(n)][x(n)] = '#';
  }

  void reset() {
    seen_ = vector<vector<bool>>(maze_.size(), vector<bool>(maze_.size(), false));
    q_ = Q();
    push(start_);
  }

  void run() {
    while (!q_.empty() && !reached_end()) {
      next();
    }
  }

  bool reached_end() const {
    return seen_[y(end_)][x(end_)];
  }

  void dump() const {
    for (const auto& row : maze_) {
      for (const char c : row) {
        cout << c;
      }
      cout << endl;
    }
    cout << x(end_) << ", " << y(end_) << ": " << seen_[y(end_)][x(end_)] << endl;
  }

private:
  void next() {
    const auto n = q_.front();
    q_.pop();
    // cout << "pop: " << u << endl;
    check(left(n));
    check(right(n));
    check(up(n));
    check(down(n));
  }

  void push(const Node& n) {
    // cout << "push: " << n << endl;
    q_.push(n);
  }

  void check(const Node& n) {
    if (valid(n) && maze_[y(n)][x(n)] != '#' && !seen_[y(n)][x(n)]) {
      seen_[y(n)][x(n)] = true;
      push(n);
    }
  }

  bool valid(const Node& n) const {
    return x(n) >= 0 && y(n) >= 0 && x(n) < maze_.size() && y(n) < maze_.size();
  }

private:
  vector<vector<char>> maze_;
  Node start_;
  Node end_;
  Q q_;
  vector<vector<bool>> seen_;
};

static Node next(istream& istr) {
  int x, y;
  char c;
  istr >> x >> c >> y;
  return Node(x, y);
}

int main(int ac, const char* av[]) {
  if (ac != 3) {
    cerr << "usage: maze size count" << endl;
    return 1;
  }
  const int size = atoi(av[1]);
  const int count = atoi(av[2]);
  Maze maze(size);
  int i = 0;
  for (; i < count; ++i) {
    maze.add(next(cin));
  }
  Node n;
  do {
    ++i;
    n = next(cin);
    cout << n << endl;
    maze.add(n);
    maze.reset();
    maze.run();
  } while (maze.reached_end());
  maze.dump();
  cout << i << endl;
  cout << x(n) << ',' << y(n) << endl;
}
