#include <iostream>
#include <vector>
#include <string>
#include <algorithm> // For std::max

/**
 * @brief 判断两个碱基是否可以配对
 * @param base1 第一个碱基
 * @param base2 第二个碱基
 * @return 如果可以配对返回 true，否则返回 false
 */
bool can_pair(char base1, char base2) {
    if ((base1 == 'A' && base2 == 'U') || (base1 == 'U' && base2 == 'A')) {
        return true;
    }
    if ((base1 == 'G' && base2 == 'C') || (base1 == 'C' && base2 == 'G')) {
        return true;
    }
    // Add G-U wobble pair if needed
    if ((base1 == 'G' && base2 == 'U') || (base1 == 'U' && base2 == 'G')) {
         return true;
    }
    return false;
}
/**
 * @brief 计算最大碱基对数，使用动态规划方法
 * @param sequence RNA 序列
 * @return 最大碱基对数
 * @attention 每个对的两端被至少四个插入的碱基所分割
 */
int solve_max_base_pairs_cpp(const std::string& sequence) {
    int n = sequence.length();
    if (n < 6) return 0;

    std::vector<std::vector<int>> opt(n, std::vector<int>(n, 0));

    // 按区间长度递增填表
    for (int len = 1; len <= n; ++len) {
        for (int i = 0; i + len - 1 < n; ++i) {
            int j = i + len - 1;
            // 情形1：区间太短，不能配对
            if (i >= j - 4) {
                opt[i][j] = 0;
                continue;
            }
            // 情形2：bj不配对
            int res = opt[i][j-1];

            // 情形3：bj与bt配对
            for (int t = i; t <= j - 4; ++t) {
                if (can_pair(sequence[t], sequence[j])) {
                    int left = (t > i) ? opt[i][t-1] : 0;
                    int right = (t+1 <= j-1) ? opt[t+1][j-1] : 0;
                    res = std::max(res, 1 + left + right);
                }
            }
            opt[i][j] = res;
        }
    }
    return opt[0][n-1];
}

int main() {
    std::string rna_sequence = "GGGAUCC"; // Example sequence

    // Test cases:
    std::cout << "Sequence: GGGAUCC, Max pairs: " << solve_max_base_pairs_cpp("GGGAUCC") << std::endl; // Expected 3
    std::cout << "Sequence: GCACG, Max pairs: " << solve_max_base_pairs_cpp("GCACG") << std::endl;     // Expected 2
    std::cout << "Sequence: AAAAA, Max pairs: " << solve_max_base_pairs_cpp("AAAAA") << std::endl;     // Expected 0
    std::cout << "Sequence: AUAUAU, Max pairs: " << solve_max_base_pairs_cpp("AUAUAU") << std::endl;   // Expected 3
    std::cout << "Sequence: GGUACC, Max pairs: " << solve_max_base_pairs_cpp("GGUACC") << std::endl;   // Expected 3 (G-C, G-C, U-A)

    return 0;
}
