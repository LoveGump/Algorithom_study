#include <iostream>
#include <vector>
#include <algorithm>
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

    void init(const vector<Request>& requests) {
        for (int i = 1; i <= requestNum; ++i) {
            reqs[i] = requests[i-1];
        }
    }

    void prepare() {
        sort(reqs + 1, reqs + requestNum + 1);
        memset(p, 0, sizeof(p));
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

    void solve() {
        optimal[0] = 0;
        for (int i = 1; i <= requestNum; ++i) {
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

    void output() {
        cout << "[MAX VALUE]: " << optimal[requestNum]
             << "\n[Activities]:\n";
        for (int i = requestNum; i > 0; i = record[i][1]) {
            if (record[i][0] == 1) {
                cout << "activity-" << i << " ("
                     << reqs[i].beginTime << "-" << reqs[i].endTime
                     << ", value=" << reqs[i].value << ")" << endl;
            }
        }
    }

private:
    Request reqs[MAX_REQ_NUM + 1];
    int requestNum;
    int p[MAX_REQ_NUM + 1];
    int optimal[MAX_REQ_NUM + 1];
    int record[MAX_REQ_NUM + 1][2];
};

void runTest(const vector<Request>& requests, const string& testName) {
    cout << "\n=== " << testName << " ===\n";
    DP dp;
    dp.setRequestNum(requests.size());
    dp.init(requests);
    dp.prepare();
    dp.solve();
    dp.output();
}

int main() {
    // 测试用例1：基本测试
    vector<Request> test1 = {
        {1, 4, 5},   // 活动1
        {3, 5, 1},   // 活动2
        {0, 6, 8},   // 活动3
        {4, 7, 4},   // 活动4
        {3, 8, 6},   // 活动5
        {5, 9, 3},   // 活动6
        {6, 10, 2},  // 活动7
        {8, 11, 4}   // 活动8
    };
    runTest(test1, "Test Case 1: Basic Test");

    // 测试用例2：重叠区间
    vector<Request> test2 = {
        {1, 3, 2},   // 活动1
        {2, 4, 3},   // 活动2
        {3, 5, 4},   // 活动3
        {4, 6, 5},   // 活动4
        {5, 7, 6}    // 活动5
    };
    runTest(test2, "Test Case 2: Overlapping Intervals");

    // 测试用例3：高价值区间
    vector<Request> test3 = {
        {1, 4, 10},  // 活动1
        {2, 5, 5},   // 活动2
        {3, 6, 15},  // 活动3
        {4, 7, 7},   // 活动4
        {5, 8, 20}   // 活动5
    };
    runTest(test3, "Test Case 3: High Value Intervals");

    return 0;
}
