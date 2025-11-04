// Author: Ashish Bhambure | WCE Sangli

#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

int partitionSeq(vector<int> &arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSortSequential(vector<int> &arr, int low, int high) {
    if (low < high) {
        int pi = partitionSeq(arr, low, high);
        quickSortSequential(arr, low, pi - 1);
        quickSortSequential(arr, pi + 1, high);
    }
}


long long discardedWork = 0;

pair<int, int> partitionInfo(const vector<int> &arr, int pivot) {
    int left = 0, right = 0;
    for (int val : arr) {
        if (val < pivot) left++;
        else if (val > pivot) right++;
    }
    return {left, right};
}

int balanceScore(int left, int right) {
    return abs(left - right);
}

void speculativeQuickSort(vector<int> &arr) {
    int n = arr.size();
    if (n <= 1) return;

    vector<int> pivots = {arr[0], arr[n / 2], arr[n - 1]};
    vector<pair<int, int>> results(pivots.size());

    #pragma omp parallel for
    for (int i = 0; i < (int)pivots.size(); i++) {
        results[i] = partitionInfo(arr, pivots[i]);
    }

    int bestPivot = pivots[0];
    int bestScore = INT_MAX;
    for (int i = 0; i < (int)pivots.size(); i++) {
        int score = balanceScore(results[i].first, results[i].second);
        if (score < bestScore) {
            bestScore = score;
            bestPivot = pivots[i];
        }
    }

    discardedWork += (pivots.size() - 1);

    vector<int> left, right;
    for (int val : arr) {
        if (val < bestPivot) left.push_back(val);
        else if (val > bestPivot) right.push_back(val);
    }

    speculativeQuickSort(left);
    speculativeQuickSort(right);

    arr.clear();
    arr.insert(arr.end(), left.begin(), left.end());
    arr.push_back(bestPivot);
    arr.insert(arr.end(), right.begin(), right.end());
}

int main() {
    vector<int> arr = {112,13,67,54,978,65,98,444,55,100};

    cout << "Original Array:\n";
    for (auto x : arr) cout << x << " ";
    cout << "\n\n";

    vector<int> arr_seq = arr;
    double start_seq = omp_get_wtime();
    quickSortSequential(arr_seq, 0, arr_seq.size() - 1);
    double end_seq = omp_get_wtime();

    vector<int> arr_spec = arr;
    discardedWork = 0;
    double start_spec = omp_get_wtime();
    speculativeQuickSort(arr_spec);
    double end_spec = omp_get_wtime();

    cout << "Sorted Array (Speculative QuickSort):\n";
    for (auto x : arr_spec) cout << x << " ";
    cout << "\n\n";

    cout << fixed << setprecision(6);
    cout << "Sequential QuickSort Time : " << (end_seq - start_seq) << " sec\n";
    cout << "Speculative QuickSort Time: " << (end_spec - start_spec) << " sec\n";
    cout << "Speedup: " << ((end_seq - start_seq) / (end_spec - start_spec)) << "x\n";
    cout << "Discarded Partitions (wasted work): " << discardedWork << "\n";

    
}
