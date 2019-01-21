# Assignment 2: Sorting Algorithms
## Overview
For each file, slowsort and fastsort, I sorted 10 items for the small array, 1000 items for the medium array, and 10,000 items for the large array. On average over 30 trials it took Insertion Sort 0.001 msecs to sort the small array, 2.612 msecs to sort the medium array, and 172.501 msecs to sort the large array. It took Quick Sort 0.002 msecs, 0.344 msecs and 2.960 msecs to sort the same arrays, respectively.

## Choosing Size Constraints
Regarding the size of the arrays:
* The difference in sizes should be large enough that there is a clear difference in the time it takes to sort each array, for both sorting algorithms.
* I feel that the smallest size array that made sense to this test was 10 elements. Given millisecond precision to the thousandths, both algorithms sorted a 10 element array in a measurable length of time.
* The largest size array that made sense should be based on the time complexity of the slower algorithm, in this case Insertion Sort, and the number of trials used. I chose 10,000 elements for this array somewhat arbitrarily, but it worked perfectly because my slowsort program takes a few seconds to fully execute. I wouldn't want to wait much longer than that unless I needed very accurate averages or to compare larger arrays for some reason.

## Time Analysis
It appears that only the 10 element array was sorted faster by Insertion Sort than Quick Sort. On several runs of each program this appears to hold up. So on very small arrays, at least up to 10 elements, Insertion Sort can be faster than Quick Sort. However, on larger arrays Quick Sort is much faster than Insertion Sort.

The average time complexity of Insertion Sort is O(n^2) and Quick Sort is O(n log(n)). We can assume average case complexity because each array was filled with random numbers, so it was unlikely that any array started in reversed sorted order. The observed differences in sorting times supports the theoretical complexity. The Insertion Sort times grow at a much faster rate than the Quick Sort times. For example, the observed time ratio between Insertion Sort and Quick Sort is 7.59 for the medium array and 58.28 for the large array.

## Challenges
The hardest part of this assignment was debugging. Syntax problems were easy to fix because the compiler says which line number the problem is on. However, other bugs are hard to catch because the compiler does not give much information. For example, at one point I kept getting a "segmentation fault (core dump)" when trying to compile. I narrowed down what the problem could be by searching the error online, but it still took some careful review of my code, and pulling out a notebook to trace the execution by hand, to catch the error and fix it. I also found it hard to debug problems with timing the sorting algorithms. I tried using the gdb debugger, which I like a lot, but that doesn't work great for getting an accurate clock time of a function.
