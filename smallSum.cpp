#include <iostream>
using namespace std;

static int inArr[100001];
static int help[100001];

long long merge(int l, int m, int r) {
  long long ans = 0;
  for (int j = m + 1, i = l, sum = 0; j <= r; j++) {
    while (i <= m && inArr[i] <= inArr[j]) {
      sum += inArr[i++];
    }
    ans += sum;
  }
  // 正常归并排序

  int i = l;
  int a = l;
  int b = m + 1;
  while (a <= m && b <= r) {
    help[i++] = inArr[a] < inArr[b] ? inArr[a++] : inArr[b++];
  }
  while (a <= m) {
    help[i++] = inArr[a++];
  }
  while (b <= r) {
    help[i++] = inArr[b++];
  }
  for (int i = l; i <= r; i++) {
    inArr[i] = help[i];
  }
  return ans;
}

long long smallSum(int l, int r) {
  if (l == r) {
    return 0;
  }
  int m = (l + r) >> 1;
  return smallSum(l, m) + smallSum(m + 1, r) + merge(l, m, r);
}

int main() {
  int m;
  cin >> m;
  for (int i = 0; i < m; i++) {
    cin >> inArr[i];
  }
  cout << smallSum(0, m - 1);
}