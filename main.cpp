#include "bits/stdc++.h"
using namespace std;
#define endl '\n'
template <typename T>
struct Fenwick {
    int n;
    std::vector<T> a;
    Fenwick(int n) : n(n), a(n) {}
    void add(int x, T v) {
        for (int i = x + 1; i <= n; i += i & -i) {
            a[i - 1] += v;
        }
    }
    T sum(int x) {
        T ans = 0;
        for (int i = x; i > 0; i -= i & -i) {
            ans += a[i - 1];
        }
        return ans;
    }
    T rangeSum(int l, int r) {
        return sum(r) - sum(l);
    }
};
int main() {

    int n, m;
    cin >> n >> m;

    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    Fenwick<int> fenwick(n);

    for (int i = 0; i < n; i++) {
        fenwick.add(i, a[i]);
    }
    while (m--) {
        int op;
        cin >> op;
        int x, k;
        cin >> x >> k;
        if (op == 1) {
            fenwick.add(x - 1, k);
        } else {
            cout << fenwick.rangeSum(x - 1, k) << endl;
        }
    }

    return 0;
}