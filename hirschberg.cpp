#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

/**
 * @brief 判断字符是否为元音字母
 * @param ch 字符
 * @return 如果是元音字母返回 true，否则返回 false
 */
bool is_vowel(char ch) {
    ch = tolower(ch);
    return ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u';
}
/**
 * @brief 计算两个字符的匹配分数
 * @param a 第一个字符
 * @param b 第二个字符
 * @return 匹配分数
 * @details 匹配分数规则：
 * 1. 相同字符得分0
 * 2. 元音与元音或辅音与辅音得分1
 * 3. 元音与辅音得分3
 */
int match_score(char a, char b) {
    if (a == b) return 0;
    if (is_vowel(a) && is_vowel(b)) return 1;
    if (!is_vowel(a) && !is_vowel(b)) return 1;
    return 3;
}

/**
 * @brief 使用动态规划计算序列比对的最小罚分
 * @param A 第一个序列
 * @param B 第二个序列
 * @param gap_penalty 缺口罚分
 * @return
 */
vector<int> nw_score(const string& A, const string& B, int gap_penalty) {
    int m = A.size(), n = B.size();
    // prev 和 curr 用于存储上一行和当前行的分数
    vector<int> prev(n + 1), curr(n + 1);

    for (int j = 0; j <= n; ++j) prev[j] = j * gap_penalty;

    for (int i = 1; i <= m; ++i) {
        curr[0] = i * gap_penalty;
        for (int j = 1; j <= n; ++j) {
            int cost = match_score(A[i - 1], B[j - 1]);
            curr[j] = min({
                prev[j - 1] + cost,
                prev[j] + gap_penalty,
                curr[j - 1] + gap_penalty
            });
        }
        prev = curr;
    }
    return curr;
}

// Hirschberg递归
void hirschberg(const string& A, const string& B, int gap_penalty, string& alignA, string& alignB) {
    int m = A.size(), n = B.size();
    if (m == 0) {
        alignA += string(n, '-');
        alignB += B;
        return;
    }
    if (n == 0) {
        alignA += A;
        alignB += string(m, '-');
        return;
    }
    if (m == 1 || n == 1) {
        // 直接用常规DP
        vector<vector<int>> dp(m + 1, vector<int>(n + 1));
        for (int i = 0; i <= m; ++i) dp[i][0] = i * gap_penalty;
        for (int j = 0; j <= n; ++j) dp[0][j] = j * gap_penalty;
        for (int i = 1; i <= m; ++i) {
            for (int j = 1; j <= n; ++j) {
                int cost = match_score(A[i - 1], B[j - 1]);
                dp[i][j] = min({
                    dp[i - 1][j - 1] + cost,
                    dp[i - 1][j] + gap_penalty,
                    dp[i][j - 1] + gap_penalty
                });
            }
        }
        // 回溯
        int i = m, j = n;
        string a, b;
        while (i > 0 && j > 0) {
            if (dp[i][j] == dp[i - 1][j - 1] + match_score(A[i - 1], B[j - 1])) {
                a = A[i - 1] + a;
                b = B[j - 1] + b;
                --i; --j;
            } else if (dp[i][j] == dp[i - 1][j] + gap_penalty) {
                a = A[i - 1] + a;
                b = '-' + b;
                --i;
            } else {
                a = '-' + a;
                b = B[j - 1] + b;
                --j;
            }
        }
        while (i > 0) { a = A[i - 1] + a; b = '-' + b; --i; }
        while (j > 0) { a = '-' + a; b = B[j - 1] + b; --j; }
        alignA += a;
        alignB += b;
        return;
    }

    int mid = m / 2;
    // 计算左半部分的分数
    vector<int> scoreL = nw_score(A.substr(0, mid), B, gap_penalty);
    string revA = A.substr(mid);
    reverse(revA.begin(), revA.end());
    // 计算右半部分的分数
    string revB = B;
    reverse(revB.begin(), revB.end());
    vector<int> scoreR = nw_score(revA, revB, gap_penalty);

    int split = 0, min_score = scoreL[0] + scoreR[n];
    for (int j = 0; j <= n; ++j) {
        int s = scoreL[j] + scoreR[n - j];
        if (s < min_score) {
            min_score = s;
            split = j;
        }
    }

    hirschberg(A.substr(0, mid), B.substr(0, split), gap_penalty, alignA, alignB);
    hirschberg(A.substr(mid), B.substr(split), gap_penalty, alignA, alignB);
}

int main() {
    string seq1 = "name";
    string seq2 = "mean";
    int gap_penalty = 2;
    string alignA, alignB;
    hirschberg(seq1, seq2, gap_penalty, alignA, alignB);

    // 计算总罚分
    int total = 0;
    for (size_t i = 0; i < alignA.size(); ++i) {
        if (alignA[i] == '-' || alignB[i] == '-') total += gap_penalty;
        else total += match_score(alignA[i], alignB[i]);
    }

    cout << "最小罚分: " << total << endl;
    cout << "比对结果:" << endl;
    cout << alignA << endl;
    cout << alignB << endl;
    return 0;
}
