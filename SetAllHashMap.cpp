// filepath: ~/code/Algorithm/SetAllHashMap.cpp
// link:https://www.nowcoder.com/practice/7c4559f138e74ceb9ba57d76fd169967
#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

int main() {
  // key - value - cnt;
  unordered_map<long long, pair<long long, int>> map;
  int n;
  cin >> n;
  int num;
  long long key, value;
  int setAllTime = -1;
  long long setAllValue = 0;
  int cnt = 0;
  while (n--) {
    cin >> num;
    switch (num) {
      case 1:
        cin >> key >> value;
        map[key] = {value, cnt};
        cnt++;
        break;
      case 2:
        cin >> key;
        if (map.find(key) != map.end()) {
          if (map[key].second > setAllTime) {
            cout << map[key].first << endl;
          } else {
            cout << setAllValue << endl;
          }
          cnt++;
          break;
        }
        cout << -1 << endl;  // 通常返回一个表示"不存在"的值
        cnt++;
        break;
      default:
        cin >> value;
        setAllTime = cnt;
        setAllValue = value;
        cnt++;
        break;
    }
  }
}
