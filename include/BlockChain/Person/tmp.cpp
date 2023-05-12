#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int main(int argc, char *argv[]) {

  vector<int> v = {23, 10, 49, 50, 13, 12, 9, 45, 33, 17, 2, 21, 6, 1, 5};
  vector<int> max_heap;

  for (auto x : v) {
    max_heap.push_back(x);
    push_heap(max_heap.begin(), max_heap.end());
  }

  for (auto x : max_heap)
    cout << x << " ";

  cout << "\n\n";

  vector<int> min_heap;
  for (auto x : v) {
    min_heap.push_back(x);
    // min_heap push with greater
    push_heap(min_heap.begin(), min_heap.end(), greater<int>());
  }
  for (auto x : min_heap)
    cout << x << " ";

  return 0;
}
