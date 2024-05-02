#include <iostream>
#include <algorithm>
#include <vector>
#include <ctime>
#include <fstream>

#include <initializer_list>

//#include <__config>
#include <initializer_list>
#include <type_traits>
#include <cstring>
#include <utility> // needed to provide swap_ranges.
#include <memory>
#include <functional>
#include <iterator>
#include <cstddef>
#include <bit>
#include <version>

//#include <__debug>

//#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
//#pragma GCC system_header
//#endif

//using namespace std;

namespace std
{
template <class RandomAccessIterator>
void
sort(RandomAccessIterator first, RandomAccessIterator last);
template <class RandomAccessIterator, class Compare>
void
sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp);
template <class RandomAccessIterator>
void
stable_sort(RandomAccessIterator first, RandomAccessIterator last);
template <class RandomAccessIterator, class Compare>
void
stable_sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp);
template <class RandomAccessIterator>
void
partial_sort(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last);
template <class RandomAccessIterator, class Compare>
void
partial_sort(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last, Compare comp);
template <class InputIterator, class RandomAccessIterator>
RandomAccessIterator
partial_sort_copy(InputIterator first, InputIterator last,
                  RandomAccessIterator result_first, RandomAccessIterator result_last);
template <class InputIterator, class RandomAccessIterator, class Compare>
RandomAccessIterator
partial_sort_copy(InputIterator first, InputIterator last,
                  RandomAccessIterator result_first, RandomAccessIterator result_last, Compare comp);
}  // std

template <class _Comp>
struct __comp_ref_type {
    // Pass the comparator by lvalue reference. Or in debug mode, using a
    // debugging wrapper that stores a reference.
#ifndef _LIBCPP_DEBUG
    typedef typename std::add_lvalue_reference<_Comp>::type type;
#else
    typedef __debug_less<_Comp> type;
#endif
};

template <class _T1, class _T2 = _T1>
struct __less
{
        bool operator()(const _T1& __x, const _T1& __y) const {return __x < __y;}

        bool operator()(const _T1& __x, const _T2& __y) const {return __x < __y;}

        bool operator()(const _T2& __x, const _T1& __y) const {return __x < __y;}

        bool operator()(const _T2& __x, const _T2& __y) const {return __x < __y;}
};

template <class _T1>
struct __less<_T1, _T1>
{
        bool operator()(const _T1& __x, const _T1& __y) const {return __x < __y;}
};

template <class _T1>
struct __less<const _T1, _T1>
{
        bool operator()(const _T1& __x, const _T1& __y) const {return __x < __y;}
};

template <class _T1>
struct __less<_T1, const _T1>
{
        bool operator()(const _T1& __x, const _T1& __y) const {return __x < __y;}
};

template <class _Compare, class _ForwardIterator>
unsigned
__sort3(_ForwardIterator __x, _ForwardIterator __y, _ForwardIterator __z, _Compare __c)
{
    unsigned __r = 0;
    if (!__c(*__y, *__x))          // if x <= y
    {
        if (!__c(*__z, *__y))      // if y <= z
            return __r;            // x <= y && y <= z
                               // x <= y && y > z
        std::swap(*__y, *__z);          // x <= z && y < z
        __r = 1;
        if (__c(*__y, *__x))       // if x > y
        {
            std::swap(*__x, *__y);      // x < y && y <= z
            __r = 2;
        }
        return __r;                // x <= y && y < z
    }
    if (__c(*__z, *__y))           // x > y, if y > z
    {
        std::swap(*__x, *__z);          // x < y && y < z
        __r = 1;
        return __r;
    }
    std::swap(*__x, *__y);              // x > y && y <= z
    __r = 1;                       // x < y && x <= z
    if (__c(*__z, *__y))           // if y > z
    {
        std::swap(*__y, *__z);          // x <= y && y < z
        __r = 2;
    }
    return __r;
}                                  // x <= y && y <= z

// stable, 3-6 compares, 0-5 swaps

template <class _Compare, class _ForwardIterator>
unsigned
__sort4(_ForwardIterator __x1, _ForwardIterator __x2, _ForwardIterator __x3,
        _ForwardIterator __x4, _Compare __c)
{
    unsigned __r = __sort3<_Compare>(__x1, __x2, __x3, __c);
    if (__c(*__x4, *__x3))
    {
        std::swap(*__x3, *__x4);
        ++__r;
        if (__c(*__x3, *__x2))
        {
            std::swap(*__x2, *__x3);
            ++__r;
            if (__c(*__x2, *__x1))
            {
                std::swap(*__x1, *__x2);
                ++__r;
            }
        }
    }
    return __r;
}

// stable, 4-10 compares, 0-9 swaps

template <class _Compare, class _ForwardIterator>
    unsigned
    __sort5(_ForwardIterator __x1, _ForwardIterator __x2, _ForwardIterator __x3,
            _ForwardIterator __x4, _ForwardIterator __x5, _Compare __c)
{
    unsigned __r = __sort4<_Compare>(__x1, __x2, __x3, __x4, __c);
    if (__c(*__x5, *__x4))
    {
        std::swap(*__x4, *__x5);
        ++__r;
        if (__c(*__x4, *__x3))
        {
            std::swap(*__x3, *__x4);
            ++__r;
            if (__c(*__x3, *__x2))
            {
                std::swap(*__x2, *__x3);
                ++__r;
                if (__c(*__x2, *__x1))
                {
                    std::swap(*__x1, *__x2);
                    ++__r;
                }
            }
        }
    }
    return __r;
}

template <class _Compare, class _RandomAccessIterator>
void
__insertion_sort_3(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    typedef typename std::iterator_traits<_RandomAccessIterator>::value_type value_type;
    _RandomAccessIterator __j = __first+2;
    __sort3<_Compare>(__first, __first+1, __j, __comp);
    for (_RandomAccessIterator __i = __j+1; __i != __last; ++__i)
    {
        if (__comp(*__i, *__j))
        {
            value_type __t(std::move(*__i));
            _RandomAccessIterator __k = __j;
            __j = __i;
            do
            {
                *__j = std::move(*__k);
                __j = __k;
            } while (__j != __first && __comp(__t, *--__k));
            *__j = std::move(__t);
        }
        __j = __i;
    }
}

template <class _Compare, class _RandomAccessIterator>
bool
__insertion_sort_incomplete(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    switch (__last - __first)
    {
        case 0:
        case 1:
            return true;
        case 2:
            if (__comp(*--__last, *__first))
                std::swap(*__first, *__last);
            return true;
        case 3:
            __sort3<_Compare>(__first, __first+1, --__last, __comp);
            return true;
        case 4:
            __sort4<_Compare>(__first, __first+1, __first+2, --__last, __comp);
            return true;
        case 5:
            __sort5<_Compare>(__first, __first+1, __first+2, __first+3, --__last, __comp);
            return true;
    }
    typedef typename std::iterator_traits<_RandomAccessIterator>::value_type value_type;
    _RandomAccessIterator __j = __first+2;
    __sort3<_Compare>(__first, __first+1, __j, __comp);
    const unsigned __limit = 8;
    unsigned __count = 0;
    for (_RandomAccessIterator __i = __j+1; __i != __last; ++__i)
    {
        if (__comp(*__i, *__j))
        {
            value_type __t(std::move(*__i));
            _RandomAccessIterator __k = __j;
            __j = __i;
            do
            {
                *__j = std::move(*__k);
                __j = __k;
            } while (__j != __first && __comp(__t, *--__k));
            *__j = std::move(__t);
            if (++__count == __limit)
                return ++__i == __last;
        }
        __j = __i;
    }
    return true;
}

//#include <map>
//std::map<int, int> crashing_map;
//int crashing_number = 1000000000;

template <class _Compare, class _RandomAccessIterator>
void
__sort(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    // _Compare is known to be a reference type
    typedef typename std::iterator_traits<_RandomAccessIterator>::difference_type difference_type;
    typedef typename std::iterator_traits<_RandomAccessIterator>::value_type value_type;
    const difference_type __limit = std::is_trivially_copy_constructible<value_type>::value &&
                                            std::is_trivially_copy_assignable<value_type>::value ? 30 : 6;
    while (true)
    {
    __restart:
        difference_type __len = __last - __first;
        switch (__len)
        {
            case 0:
            case 1:
                return;
            case 2:
                if (__comp(*--__last, *__first))
                    std::swap(*__first, *__last);
                return;
            case 3:
                __sort3<_Compare>(__first, __first+1, --__last, __comp);
                return;
            case 4:
                __sort4<_Compare>(__first, __first+1, __first+2, --__last, __comp);
                return;
            case 5:
                __sort5<_Compare>(__first, __first+1, __first+2, __first+3, --__last, __comp);
                return;
        }
        if (__len <= __limit)
        {
            __insertion_sort_3<_Compare>(__first, __last, __comp);
            return;
        }
        // __len > 5
        _RandomAccessIterator __m = __first;
        _RandomAccessIterator __lm1 = __last;
        --__lm1;
        unsigned __n_swaps;
        {
            difference_type __delta;
            if (__len >= 1000)
            {
                __delta = __len/2;
                __m += __delta;
                __delta /= 2;

//                crashing_map[*(__first)] = crashing_number++;
//                crashing_map[*(__first + __delta)] = crashing_number++;
//                crashing_map[*(__m)] = crashing_number++;
//                crashing_map[*(__m+__delta)] = crashing_number++;
//                crashing_map[*(__lm1)] = crashing_number++;
//                *__first = crashing_number;
//                *(__first + __delta) = crashing_number;
//                *(__m) = crashing_number;
//                *(__m+__delta) = crashing_number;
//                *(__lm1) = crashing_number;
//                crashing_number--;

//                if (*(__first + __delta) != *__m || *__m != *(__m + __delta) || *(__m + __delta) != *__lm1) {
//                    break;
//                }
                __n_swaps = __sort5<_Compare>(__first, __first + __delta, __m, __m+__delta, __lm1, __comp);
            }
            else
            {
                __delta = __len/2;
                __m += __delta;

//                crashing_map[*(__first)] = crashing_number++;
//                crashing_map[*(__m)] = crashing_number++;
//                crashing_map[*(__lm1)] = crashing_number++;
//                *__first = crashing_number;
//                *(__m) = crashing_number;
//                *(__lm1) = crashing_number;
//                crashing_number--;

                __n_swaps = __sort3<_Compare>(__first, __m, __lm1, __comp);
            }
        }
        // *__m is median
        // partition [__first, __m) < *__m and *__m <= [__m, __last)
        // (this inhibits tossing elements equivalent to __m around unnecessarily)
        _RandomAccessIterator __i = __first;
        _RandomAccessIterator __j = __lm1;
        // j points beyond range to be tested, *__m is known to be <= *__lm1
        // The search going up is known to be guarded but the search coming down isn't.
        // Prime the downward search with a guard.
        if (!__comp(*__i, *__m))  // if *__first == *__m
        {
            // *__first == *__m, *__first doesn't go in first part
            // manually guard downward moving __j against __i
            while (true)
            {
                if (__i == --__j)
                {
                    // *__first == *__m, *__m <= all other elements
                    // Parition instead into [__first, __i) == *__first and *__first < [__i, __last)
                    ++__i;  // __first + 1
                    __j = __last;
                    if (!__comp(*__first, *--__j))  // we need a guard if *__first == *(__last-1)
                    {
                        while (true)
                        {
                            if (__i == __j)
                                return;  // [__first, __last) all equivalent elements
                            if (__comp(*__first, *__i))
                            {
                                std::swap(*__i, *__j);
                                ++__n_swaps;
                                ++__i;
                                break;
                            }
                            ++__i;
                        }
                    }
                    // [__first, __i) == *__first and *__first < [__j, __last) and __j == __last - 1
                    if (__i == __j)
                        return;
                    while (true)
                    {
                        while (!__comp(*__first, *__i))
                            ++__i;
                        while (__comp(*__first, *--__j))
                            ;
                        if (__i >= __j)
                            break;
                        std::swap(*__i, *__j);
                        ++__n_swaps;
                        ++__i;
                    }
                    // [__first, __i) == *__first and *__first < [__i, __last)
                    // The first part is sorted, sort the secod part
                    // _VSTD::__sort<_Compare>(__i, __last, __comp);
                    __first = __i;
                    goto __restart;
                }
                if (__comp(*__j, *__m))
                {
                    std::swap(*__i, *__j);
                    ++__n_swaps;
                    break;  // found guard for downward moving __j, now use unguarded partition
                }
            }
        }
        // It is known that *__i < *__m
        ++__i;
        // j points beyond range to be tested, *__m is known to be <= *__lm1
        // if not yet partitioned...
        if (__i < __j)
        {
            // known that *(__i - 1) < *__m
            // known that __i <= __m
            while (true)
            {
                // __m still guards upward moving __i
                while (__comp(*__i, *__m))
                    ++__i;
                // It is now known that a guard exists for downward moving __j
                while (!__comp(*--__j, *__m))
                    ;
                if (__i > __j)
                    break;
                std::swap(*__i, *__j);
                ++__n_swaps;
                // It is known that __m != __j
                // If __m just moved, follow it
                if (__m == __i)
                    __m = __j;
                ++__i;
            }
        }
        // [__first, __i) < *__m and *__m <= [__i, __last)
        if (__i != __m && __comp(*__m, *__i))
        {
            std::swap(*__i, *__m);
            ++__n_swaps;
        }
        // [__first, __i) < *__i and *__i <= [__i+1, __last)
        // If we were given a perfect partition, see if insertion sort is quick...
        if (__n_swaps == 0)
        {
            bool __fs = __insertion_sort_incomplete<_Compare>(__first, __i, __comp);
            if (__insertion_sort_incomplete<_Compare>(__i+1, __last, __comp))
            {
                if (__fs)
                    return;
                __last = __i;
                continue;
            }
            else
            {
                if (__fs)
                {
                    __first = ++__i;
                    continue;
                }
            }
        }
        // sort smaller range with recursive call and larger with tail recursion elimination
        if (__i - __first < __last - __i)
        {
            __sort<_Compare>(__first, __i, __comp);
            // _VSTD::__sort<_Compare>(__i+1, __last, __comp);
            __first = ++__i;
        }
        else
        {
            __sort<_Compare>(__i+1, __last, __comp);
            // _VSTD::__sort<_Compare>(__first, __i, __comp);
            __last = __i;
        }
    }
}

template <class _RandomAccessIterator, class _Compare>
    void
    sort2(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    typedef typename __comp_ref_type<_Compare>::type _Comp_ref;
    __sort<_Comp_ref>(__first, __last, _Comp_ref(__comp));
}

template <class _RandomAccessIterator>
    void
    sort1(_RandomAccessIterator __first, _RandomAccessIterator __last)
{
    sort2(__first, __last, __less<typename std::iterator_traits<_RandomAccessIterator>::value_type>());
}

std::vector<int> readVector(size_t max_count) {
    std::vector<int> result;
    result.reserve(max_count);
    for (size_t i = 0; i < max_count; ++i) {
        int elem;
        if (!(std::cin >> elem)) {
            break;
        }
        result.push_back(elem);
    }
    return result;
}

std::vector<int> readVectorFromFile(size_t max_count) {
    std::ifstream in("crashme_algo_6_D_out");
    std::vector<int> result;
    result.reserve(max_count);
    for (size_t i = 0; i < max_count; ++i) {
        int elem;
        if (!(in >> elem)) {
            break;
        }
        result.push_back(elem);
    }
    return result;
}

int data_size = 32767;//1011;//30001; //32768
int best = 1000000000;
std::vector<int> data(data_size);

int main2() {
//    srand(time(nullptr));
//    const size_t max_count = 32768u;
//    auto data = readVectorFromFile(max_count);

    for (int i = 0; i < data_size; ++i) {
        data[i] = i;
    }
    data[0] = 0;
    data[data_size / 4] = best;
    data[data_size / 2] = best;
    data[data_size / 2 + data_size / 4] = best;
    data[data_size - 1] = best;
    int tmp_k = 1;
    for (int i = data_size - 2; i >= data_size / 2 + data_size / 4 + 4; ) {
//        data[i--] = best - tmp_k;
        data[i--] = best / 3;
        data[i--] = best / 3;
        data[i--] = best / 3;
        data[i--] = best - tmp_k;
        tmp_k++;
    }
    int tmp_i = 1;
    for (int i = data_size - 4; ; i -= 4) {
        if (i / 4 <= 0 ||
            i / 2 <= data_size / 4 ||
            i / 2 + i / 4 <= data_size / 2) {
            break;
        }
        data[i / 4] = best - tmp_i;
        data[i / 2] = best - tmp_i;
        data[i / 2 + i / 4] = best - tmp_i;
        tmp_i++;
    }
//    int tmp_i = 1;
//    for (int i = data_size - 2; ; i -= 2) {
//        if (i / 2 <= 0) {
//            break;
//        }
//        data[i / 2] = best - tmp_i;
//        tmp_i++;
//    }

    std::ofstream out("crashme_algo_6_D_out");
    for (const int &element : data) {
        out << element << " ";
    }
    out.close();

    auto start_time = std::clock();

//    std::sort(data.begin(), data.end());
    sort1(data.begin(), data.end());

//    std::ofstream out("crashme_algo_6_D_out");
//    std::cout << "crashing map size: " << crashing_map.size() << "\n";
//    for (const int &element : data_copy) {
//        if (crashing_map.find(element) != crashing_map.end()) {
//            out << crashing_map[element] << " ";
//        } else {
//            out << element << " ";
//        }
//    }
//    out.close();

    auto end_time = std::clock();
    double spent = static_cast<double>(end_time - start_time) / CLOCKS_PER_SEC;
    std::cout << spent << " seconds\n";

    if (!std::is_sorted(data.begin(), data.end())) {
        std::cout << "mmmm...\n";
        return 1;
    }

    if (spent > 0.04) {
        std::cout << "Shit happens\n";
        return 1;
    }
    return 0;
}

int main() {
    std::ofstream out;
//    srand(time(nullptr));
//    int curr_seed = rand();
//    std::cout << "curr_seed: " << curr_seed << "\n";
//    srand(curr_seed);
//    srand(1374912423);

//    out.open("crashme_algo_6_D");
//    for (int i = 0; i < 25; ++i) {
//        out << i << " ";
//    }
//    out.close();
    main2();

//    out.open("crashme_algo_6_D");
//    for (int i = 32768; i >= 0; --i) {
//        out << i + 1000000000 << " ";
//    }
//    out.close();
//    main2();
//
//    out.open("crashme_algo_6_D");
//    for (int i = 32768; i >= 0; --i) {
//        out << rand() << " ";
//    }
//    out.close();
//    main2();
//
//    out.open("crashme_algo_6_D");
//    for (int i = 0; i < 32768; ++i) {
//        out << -rand() << " ";
//    }
//    out.close();
//    main2();

    return 0;
}