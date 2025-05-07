#include <iostream>
using namespace std;

const int MAX_REQ_NUM = 30;

struct Request {
  int beginTime;
  int endTime;
  int value;
};

bool operator<(const Request& r1, const Request& r2) {
  return r1.endTime < r2.endTime;
}

class DP {
public:
  void setRequestNum(const int& requestNum) {
    this->requestNum = requestNum;
  }
  void init() {
    for (int i = 1; i <= requestNum; ++i) {
      cin >> reqs[i].beginTime >> reqs[i].endTime >> reqs[i].value;
    }
  }
  // 预备，根据结束时间对所有请求排序，初始化数组p
  void prepare() {
    // 按照结束时间排序
    sort(reqs + 1, reqs + requestNum + 1);
    // 初始化p数组
    memset(p, 0, sizeof(p));
    // 计算每个请求的前一个请求
    for (int i = 1; i <= requestNum; ++i) {
      for (int j = i-1; j > 0; --j) {
        if (reqs[j].endTime <= reqs[i].beginTime) {
          p[i] = j;
          break;
        }
      }
    }
    memset(record, 0, sizeof(record));
  }
  // 动态规划算法
  void solve() {
    optimal[0] = 0;
    for (int i = 1; i <= requestNum; ++i) {
      // opt = max(opt[i-1], opt[p[i]] + reqs[i].value)
      if (optimal[p[i]] + reqs[i].value >= optimal[i-1]) {
        optimal[i] = optimal[p[i]] + reqs[i].value;
        record[i][0] = 1;
        record[i][1] = p[i];
      } else {
        optimal[i] = optimal[i-1];
        record[i][0] = 0;
        record[i][1] = i-1;
      }
    }
  }
  // 输出结果
  void output() {
    cout << "[MAX VALUE]: " << optimal[requestNum]
         << "\n[Activities]:\n";
    for (int i = requestNum; i > 0; i = record[i][1]) {
      if (record[i][0] == 1) {
        cout << "activity-" << i << endl;
      }
    }
  }
private:
  Request reqs[MAX_REQ_NUM + 1]; // 请求数组
  int requestNum; // 请求数量
  int p[MAX_REQ_NUM + 1]; // 记录每个请求的前一个请求
  int optimal[MAX_REQ_NUM + 1]; //  最优值数组
  int record[MAX_REQ_NUM + 1][2]; // 记录选择的请求
};

int main() {
  int requestNum;
  DP dp;
  cin >> requestNum;
  dp.setRequestNum(requestNum);
  dp.init();
  dp.prepare();
  dp.solve();
  dp.output();
  return 0;
}