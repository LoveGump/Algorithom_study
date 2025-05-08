// filepath: ~/code/Algorithm/FreqStack.cpp
// link:
#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>
using namespace std;

class FreqStack {
   public:
    unordered_map<int, int> freq;
    unordered_map<int, stack<int>> freqToVals;
    int maxFreq;

    FreqStack() { maxFreq = 0; }

    void push(int val) {
        int currf = ++freq[val];
        freqToVals[currf].push(val);
        maxFreq = currf > maxFreq ? currf : maxFreq;
    }

    int pop() {
        if (maxFreq == 0) return -1;
        int val = freqToVals[maxFreq].top();
        freqToVals[maxFreq].pop();

        freq[val]--;
        if (freqToVals[maxFreq].empty()) {
            maxFreq--;
        }

        return val;
    }
};

/**
 * Your FreqStack object will be instantiated and called as such:
 * FreqStack* obj = new FreqStack();
 * obj->push(val);
 * int param_2 = obj->pop();
 */
