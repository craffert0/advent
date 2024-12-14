/* -*- compile-command: "make -f ../../Makefile one.bin && ./one.bin < sample.txt" -*- */

#include <charconv>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

ostream& operator<<(ostream& o, const pair<int, int>& p) {
  return o << '[' << p.first << ',' << p.second << ']';
}

class Robot {
public:
  explicit Robot(const string_view line) {
    // "p=0,4 v=3,-3"

    const auto first_comma = line.find(',');
    from_chars(line.begin() + 2, line.end(), px_);
    from_chars(line.begin() + first_comma + 1, line.end(), py_);

    const auto v = line.find('v');
    const auto second_comma = line.find(',', v);
    from_chars(line.begin() + v + 2, line.end(), vx_);
    from_chars(line.begin() + second_comma + 1, line.end(), vy_);
  }

  pair<int, int> move(int n, int width, int height) const {
    return {(101 * width + px_ + n * vx_) % width, (101 * height + py_ + n * vy_) % height};
  }

private:
  int px_;
  int py_;
  int vx_;
  int vy_;
};

int main() {
  int width, height;

  string line;
  getline(cin, line);
  cout << line << endl;
  istringstream istr(line);
  istr >> width >> height;
  const int midwidth = width / 2;
  const int midheight = height / 2;
  cout << midwidth << ' ' << midheight << endl;

  int ne = 0;
  int nw = 0;
  int se = 0;
  int sw = 0;

  while (getline(cin, line)) {
    const Robot r(line);
    const auto hundred = r.move(100, width, height);
    cout << line << " -> " << hundred;
    if (hundred.first < midwidth) {
      if (hundred.second < midheight) {
        ++nw;
        cout << " nw";
      } else if (hundred.second > midheight) {
        ++sw;
        cout << " sw";
      }
    } else if (hundred.first > midwidth) {
      if (hundred.second < midheight) {
        ++ne;
        cout << " ne";
      } else if (hundred.second > midheight) {
        ++se;
        cout << " se";
      }
    }
    cout << endl;
  }
  cout << "nw=" << nw << " ne=" << ne << " sw=" << sw << " se=" << se << endl;
  cout << ne * nw * se * sw << endl;
}
