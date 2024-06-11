#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>
#include <Windows.h>

class MergeSort {
public:
    void Sort(std::vector<int>& arr) {
        if (arr.size() <= 1) return;
        std::vector<int> left(arr.begin(), arr.begin() + arr.size() / 2);
        std::vector<int> right(arr.begin() + arr.size() / 2, arr.end());

        std::thread leftThread(&MergeSort::Sort, this, std::ref(left));
        std::thread rightThread(&MergeSort::Sort, this, std::ref(right));

        leftThread.join();
        rightThread.join();

        Merge(arr, left, right);
    }

private:
    void Merge(std::vector<int>& arr, const std::vector<int>& left, const std::vector<int>& right) {
        size_t i = 0, j = 0, k = 0;
        while (i < left.size() && j < right.size()) {
            if (left[i] <= right[j]) {
                arr[k++] = left[i++];
            }
            else {
                arr[k++] = right[j++];
            }
        }
        while (i < left.size()) {
            arr[k++] = left[i++];
        }
        while (j < right.size()) {
            arr[k++] = right[j++];
        }
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    std::vector<int> arr(10);
    std::cout << "Введи 10 целых чисел:" << std::endl;
    for (int& x : arr) {
        std::cin >> x;
    }

    MergeSort mergeSort;

    std::thread mergeSortThread([&mergeSort, &arr]() {
        mergeSort.Sort(arr);
        });

    mergeSortThread.join();

    std::cout << "Отсортированный массив:" << std::endl;
    for (const int& x : arr) {
        std::cout << x << " ";
    }
    std::cout << std::endl;

    return 0;
}
