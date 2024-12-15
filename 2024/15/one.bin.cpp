/* -*- compile-command: "make -f ../../Makefile one.bin one.bin && ./one.bin < sample.txt" -*- */

#include <iostream>
#include <optional>
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
        if (line[x] == '@') {
          boxes_[y].push_back('.');
          robot_x_ = x;
          robot_y_ = y;
        } else {
          boxes_[y].push_back(line[x]);
        }
      }
    }
  }

  void move(char c) {
    switch (c) {
    case '>':
      move(1, 0);
      break;
    case 'v':
      move(0, 1);
      break;
    case '<':
      move(-1, 0);
      break;
    case '^':
      move(0, -1);
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
        if (boxes_[y][x] == 'O') {
          result += 100 * y + x;
        }
      }
    }
    return result;
  }

private:
  void move(int dx, int dy) {
    if (const auto space = find_space(dx, dy)) {
      for (int x = space->first, y = space->second;
           x != robot_x_ || y != robot_y_;
           x -= dx, y -= dy) {
        boxes_[y][x] = boxes_[y - dy][x - dx];
      }
      robot_x_ += dx;
      robot_y_ += dy;
    } else {
    }
  }

  optional<pair<int, int>> find_space(int dx, int dy) {
    int x = robot_x_ + dx, y = robot_y_ + dy;
    while (boxes_[y][x] == 'O') {
      x += dx, y += dy;
    }
    if (boxes_[y][x] == '.') {
      return pair<int, int>(x, y);
    } else {
      return nullopt;
    }
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
