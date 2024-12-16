/* -*- compile-command: "make -f ../../Makefile two.bin two.bin && ./two.bin < sample3.txt" -*- */

#include <deque>
#include <iostream>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <vector>

using namespace std;

class Warehouse {
public:
  explicit Warehouse(istream& istr) {
    string line;
    for (int y = 0; getline(istr, line) && !line.empty(); ++y) {
      boxes_.emplace_back();
      for (int x = 0; x < line.size(); ++x) {
        switch (line[x]) {
        case '@':
          robot_x_ = x * 2;
          robot_y_ = y;
        case '.':
          boxes_[y].push_back('.');
          boxes_[y].push_back('.');
          break;
        case 'O':
          boxes_[y].push_back('[');
          boxes_[y].push_back(']');
          break;
        case '#':
          boxes_[y].push_back('#');
          boxes_[y].push_back('#');
          break;
        }
      }
    }
  }

  void move(char c) {
    switch (c) {
    case '>':
      movex(1);
      break;
    case 'v':
      movey(1);
      break;
    case '<':
      movex(-1);
      break;
    case '^':
      movey(-1);
      break;
    }
  }

  void dump() const {
    for (int y = 0; y < boxes_.size(); ++y) {
      for (int x = 0; x < boxes_[y].size(); ++x) {
        if (x == robot_x_ && y == robot_y_) {
          cout << '@';
        } else {
          cout << boxes_[y][x];
        }
      }
      cout << endl;
    }
  }

  int64_t sum() const {
    int64_t result = 0;
    for (int y = 0; y < boxes_.size(); ++y) {
      for (int x = 0; x < boxes_[y].size(); ++x) {
        if (boxes_[y][x] == '[') {
          result += 100 * y + x;
        }
      }
    }
    return result;
  }

private:
  void movex(int dx) {
    const int y = robot_y_;
    if (const auto spacex = find_spacex(dx)) {
      for (int x = *spacex; x != robot_x_; x -= dx) {
        boxes_[y][x] = boxes_[y][x - dx];
      }
      robot_x_ += dx;
    }
  }

  void movey(int dy) {
    if (const auto spacey = find_spacey(dy)) {
      int y = robot_y_ + dy * spacey->size();
      for (const auto& xs : *spacey) {
        for (int x : xs) {
          boxes_[y + dy][x] = boxes_[y][x];
          boxes_[y][x] = '.';
        }
        y -= dy;
      }
      robot_y_ += dy;
    }
  }

  optional<int> find_spacex(int dx) {
    const auto& row = boxes_[robot_y_];
    int x = robot_x_ + dx;
    for (; row[x] == '[' || row[x] == ']';
         x += dx)
      ;
    if (row[x] == '.') {
      return x;
    } else {
      return nullopt;
    }
  }

  optional<deque<set<int>>> find_spacey(int dy) {
    deque<set<int>> v;
    set<int> xs;
    xs.insert(robot_x_);
    int y = robot_y_;
    while (!xs.empty()) {
      y += dy;
      set<int> next;
      for (int x : xs) {
        switch (boxes_[y][x]) {
        case '[':
          next.insert(x);
          next.insert(x + 1);
          break;
        case ']':
          next.insert(x);
          next.insert(x - 1);
          break;
        case '#':
          return nullopt;
        }
      }
      v.push_front(next);
      xs = next;
    }
    return v;
  }

private:
  vector<vector<char>> boxes_;
  int robot_x_;
  int robot_y_;
};

int main() {
  Warehouse w(cin);
  w.dump();

  string line;
  while (getline(cin, line)) {
    for (char c : line) {
      cout << c << endl;
      w.move(c);
      // w.dump();
    }
  }
  w.dump();
  cout << w.sum() << endl;
}
