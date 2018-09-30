#include <algorithm>

using namespace std;

// 两两比较，大的放到后面去，大的元素不断向后，小的元素不断向前，排序完成
template <class RAIter>
void bubbleSort(RAIter begin, RAIter end) {
    while(end-- > begin) {
        for(auto p = begin; p < end;) {
            auto next = p + 1;
            if(*p > *next)
                iter_swap(p, next);
            p = next;
        }
    }
}

template <class RAIter>
void selectSort(RAIter begin, RAIter end) {
    for(; begin != end; ++begin)
        iter_swap(begin, min_element(begin, end));
}

template <class RAIter>
void insertionSort(RAIter begin, RAIter end) {
    for (auto b = begin; ++b != end; begin = b) {
      for (auto c = b; *c < *begin; --c, --begin)
        iter_swap(begin, c);
    }
}

template <class RAIter>
void quickSort(RAIter begin, RAIter end) {
    auto len = end - begin;
    if(len < 2) return;

    auto front = begin, back = prev(end), pivot = begin + len / 2;
    // 以pivot为中心，比pivot小的放到pivot前面，比pivot大的放到pivot后面
    for(;;++front, --back) {
        while(*front < *pivot) ++front; // 从前往后扫描，直到扫到pivot或者比pivot大的元素
        while(*back > *pivot) --back;   // 从后往前扫描，直到扫到pivot或者比pivot小的元素
        if(front >= back) break;        // 如果已经扫描到pivot或者超过扫描范围，跳过
        iter_swap(front, back);
    }

    quickSort(begin, front); // 给前半部分排序
    quickSort(front, end);   // 给后半部分排序
}
