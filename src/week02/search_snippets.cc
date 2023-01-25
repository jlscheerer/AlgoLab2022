#include <bits/stdc++.h>

using namespace std;

struct Word {
  int id, pos;
};

int solve(int n, int m, vector<Word> &words) {
  vector<int> cnts(n);
  int j = 0, rem = n, ans = INT_MAX;
  for (int i = 0; i < m; ++i) {
    if (++cnts[words[i].id] == 1) {
      --rem;
    }
    while (j < i && cnts[words[j].id] > 1) {
      --cnts[words[j].id];
      ++j;
    }
    if (rem == 0) {
      ans = min(ans, words[i].pos - words[j].pos + 1);
    }
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    int n;
    cin >> n;
    vector<int> m(n);
    int num_words = 0;
    for (int i = 0; i < n; ++i) {
      cin >> m[i];
      num_words += m[i];
    }
    vector<Word> words;
    words.reserve(num_words);
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < m[i]; ++j) {
        int position;
        cin >> position;
        words.push_back({i, position});
      }
    }
    sort(words.begin(), words.end(),
         [](const auto &w1, const auto &w2) { return w1.pos < w2.pos; });
    cout << solve(n, num_words, words) << '\n';
  }
}