#include <iostream>
using namespace std;

int main() {
    int h1, h2, m1, m2, s1, s2, T, T1, T2;
    cin >> h1 >> m1 >> s1 >> h2 >> m2 >> s2;
    T1 = h1 * 3600 + m1 * 60 + s1;
    T2 = h2 * 3600 + m2 * 60 + s2;
    //cout << T1 << " " << T2 << endl;
    if (T1 > T2) {
        T = 86400 - T1 + T2;
    } else {
        T = T2 - T1;
    }
    //cout << T << endl;
    cout << T / 3600 << " " << (T - (T / 3600) * 3600) / 60 << " " << (T - (T / 3600) * 3600) % 60;
}