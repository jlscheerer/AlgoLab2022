#include <bits/stdc++.h>

using namespace std;

struct IWord {
  int index, word;
};

long solve(int n, vector<IWord> &words) {
  vector<int> counts(n);
  int i = 0, j = 0, rem = n;
  long ans = INT_MAX;
  for (; i < (int)words.size(); ++i) {
    rem -= counts[words[i].word] == 0;
    ++counts[words[i].word];
    while (rem == 0 && j < i && counts[words[j].word] > 1) {
      --counts[words[j].word];
      j++;
    }
    long span = (long)words[i].index - words[j].index + 1;
    if (rem == 0 && span < ans) {
      ans = span;
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
    int total_words = 0;
    for (int i = 0; i < n; ++i) {
      cin >> m[i];
      total_words += m[i];
    }
    vector<IWord> words;
    words.reserve(total_words);
    for (int i = 0; i < n; ++i) {
      for (int pi = 0; pi < m[i]; ++pi) {
        int index;
        cin >> index;
        words.push_back({index, i});
      }
    }
    sort(words.begin(), words.end(),
         [](const auto &a, const auto &b) { return a.index < b.index; });
    cout << solve(n, words) << '\n';
  }
}