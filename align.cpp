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
 */
int match_score(char a, char b) {
    // 匹配分数规则：
    // 1. 相同字符得分0
    if (a == b) return 0;
    // 2. 元音与元音或辅音与辅音得分1
    if (is_vowel(a) && is_vowel(b)) return 1;
    if (!is_vowel(a) && !is_vowel(b)) return 1;
    // 3. 元音与辅音得分3
    return 3;
}

/**
 * @brief 使用动态规划进行序列比对
 * @param seq1 第一个序列
 * @param seq2 第二个序列
 * @param gap_penalty 缺口罚分
 * @return 最小罚分
 */
int sequence_alignment(const string& seq1, const string& seq2, int gap_penalty = 2) {
    int m = seq1.size(), n = seq2.size();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));

    // 初始化
    for (int i = 0; i <= m; ++i) dp[i][0] = i * gap_penalty;
    for (int j = 0; j <= n; ++j) dp[0][j] = j * gap_penalty;

    // 动态规划填表
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            int cost = match_score(seq1[i - 1], seq2[j - 1]);
            dp[i][j] = min({
                dp[i - 1][j - 1] + cost, // 匹配/替换
                dp[i - 1][j] + gap_penalty, // seq1 匹配一个空
                dp[i][j - 1] + gap_penalty  // seq2 匹配一个空
            });
        }
    }

    // 回溯输出比对路径
    string align1, align2;
    int i = m, j = n;
    while (i > 0 && j > 0) {
        int score = dp[i][j];
        if (score == dp[i - 1][j - 1] + match_score(seq1[i - 1], seq2[j - 1])) {
            align1 = seq1[i - 1] + align1;
            align2 = seq2[j - 1] + align2;
            --i; --j;
        } else if (score == dp[i - 1][j] + gap_penalty) {
            align1 = seq1[i - 1] + align1;
            align2 = '-' + align2;
            --i;
        } else {
            align1 = '-' + align1;
            align2 = seq2[j - 1] + align2;
            --j;
        }
    }
    while (i > 0) {
        align1 = seq1[i - 1] + align1;
        align2 = '-' + align2;
        --i;
    }
    while (j > 0) {
        align1 = '-' + align1;
        align2 = seq2[j - 1] + align2;
        --j;
    }

    // 输出结果
    cout << "最小罚分: " << dp[m][n] << endl;
    cout << "比对结果:" << endl;
    cout << align1 << endl;
    cout << align2 << endl;

    return dp[m][n];
}

int main() {
    string seq1 = "name";
    string seq2 = "mean";
    sequence_alignment(seq1, seq2);
    return 0;
}
