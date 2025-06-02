#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <set>
#include <algorithm>
#include <random>
using namespace std;

// 生成均匀分布的序列
vector<int> generate_uniform_sequence(int length, int range) {
    vector<int> seq;
    for (int i = 0; i < length; ++i) {
        seq.push_back(rand() % range + 1);
    }
    return seq;
}

// 生成Zipf分布的序列
vector<int> generate_zipf_sequence(int length, int range, double alpha = 1.2) {
    vector<int> seq;
    vector<double> probs(range);
    double sum = 0.0;
    
    // 计算Zipf分布的概率
    for (int i = 0; i < range; ++i) {
        probs[i] = 1.0 / pow(i + 1, alpha);
        sum += probs[i];
    }
    
    // 归一化概率
    for (int i = 0; i < range; ++i) {
        probs[i] /= sum;
    }
    
    // 生成序列
    random_device rd;
    mt19937 gen(rd());
    discrete_distribution<> d(probs.begin(), probs.end());
    
    for (int i = 0; i < length; ++i) {
        seq.push_back(d(gen) + 1);
    }
    return seq;
}

// 生成局部性序列
vector<int> generate_locality_sequence(int length, int range, int window_size) {
    vector<int> seq;
    for (int i = 0; i < length; i += window_size) {
        int base = rand() % (range - window_size + 1) + 1;
        for (int j = 0; j < window_size && i + j < length; ++j) {
            seq.push_back(base + j);
        }
    }
    return seq;
}

/**
 * @brief 生成测试操作序列
 * @param seq 页面访问序列
 * @param value_range 值范围
 * @param out 输出流
 */
void generate_test_ops(const vector<int>& seq, int value_range, ostream& out) {
    set<int> used_keys;
    vector<int> put_keys;
    
    // 第一阶段：填充缓存
    for (int key : seq) {
        if (used_keys.count(key) == 0) {
            int value = rand() % value_range + 1;
            out << "put " << key << " " << value << endl;
            put_keys.push_back(key);
            used_keys.insert(key);
        }
    }
    
    // 第二阶段：生成操作序列
    for (int key : seq) {
        if (rand() % 2 == 0) {  // 50%概率put
            int value = rand() % value_range + 1;
            out << "put " << key << " " << value << endl;
        } else {  // 50%概率get
            out << "get " << key << endl;
        }
    }
    out << "end" << endl;
}

int main() {
    srand((unsigned)time(0));
    vector<int> scales = {50, 100, 200, 500, 1000};
    int key_range = 20;    // 键的范围
    int value_range = 100; // 值的范围
    
    for (int scale : scales) {
        // 生成三种分布的测试数据
        vector<int> uniform_seq = generate_uniform_sequence(scale, key_range);
        vector<int> zipf_seq = generate_zipf_sequence(scale, key_range);
        vector<int> locality_seq = generate_locality_sequence(scale, key_range, 5);
        
        // 保存测试数据
        ofstream fout_uniform("test/" + to_string(scale) + "_uniform.in");
        ofstream fout_zipf("test/" + to_string(scale) + "_zipf.in");
        ofstream fout_locality("test/" + to_string(scale) + "_locality.in");
        
        generate_test_ops(uniform_seq, value_range, fout_uniform);
        generate_test_ops(zipf_seq, value_range, fout_zipf);
        generate_test_ops(locality_seq, value_range, fout_locality);
        
        fout_uniform.close();
        fout_zipf.close();
        fout_locality.close();
    }
    
    cout << "✅ 测试数据已生成并保存在 test 目录下！" << endl;
    return 0;
}