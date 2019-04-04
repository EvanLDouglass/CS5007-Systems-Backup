# A9 Report

## Hypotheses

### Which Index is bigger?

I believe the TypeIndex is bigger than the OffsetIndex given that they have the same number of entries.
Both have the same basic structure, but the OffsetIndex contains integers while the TypeIndex contains full `Movie` structs.
Each `Movie` contains three integers, three strings, and an array of strings.
Thus, every entry in a TypeIndex is several times bigger than an entry in an OffsetIndex.

### Which Index is faster to search

I think the TypeIndex will be faster to search. Unlike the OffestIndex, the TypeIndex has all of the data it needs to make a report saved in memory already.
The OffsetIndex saves only a reference to a file and line, then has to go grab each line, parse them each into a `Movie` and then output the report.
All the processing that needs to be done on the fly in an OffsetIndex should make it slower than the TypeIndex.

### Which Index is faster to build?

For the same reason that the OffsetIndex should be slower to search, the TypeIndex should be slower to build, assuming both have the same number of movies.
Thus the OffsetIndex should be faster to build.
This is because the TypeIndex has to parse each line into a `Movie` at the outset, while the OffsetIndex just needs index files.

Results of the initial benchmarker test are below.
```
Created DocIdMap
Cur Real Mem: 772       Peak Real Mem: 772       Cur VirtMem: 4532      PeakVirtMem: 4532


Building the OffsetIndex
Parsing and indexing files...
22197 entries in the index.
Took 0.306884 seconds to execute.
Memory usage:
Cur Real Mem: 28916     Peak Real Mem: 28916     Cur VirtMem: 31592     PeakVirtMem: 31592
Destroyed OffsetIndex
Cur Real Mem: 28916     Peak Real Mem: 28916     Cur VirtMem: 31592     PeakVirtMem: 31592


Building the TypeIndex
10 entries in the index.
Took 0.089401 seconds to execute.
Memory usage:
Cur Real Mem: 28916     Peak Real Mem: 28916     Cur VirtMem: 31592     PeakVirtMem: 31592
Destroyed TypeIndex
Cur Real Mem: 28916     Peak Real Mem: 28916     Cur VirtMem: 31592     PeakVirtMem: 31592


Destroyed DocIdMap
Cur Real Mem: 28916     Peak Real Mem: 28916     Cur VirtMem: 31592     PeakVirtMem: 31592
```
These results are slightly misleading at first glance, but seem to confirm my hypotheses.
We can see that building the OffsetIndex took a longer time than the TypeIndex, which is the opposite of what I hypothesized.
However, there are nearly 2000 times more entries in the OffsetIndex and it only took about 3.5 times longer to build.
This suggests that the OffsetIndex is *much* faster than the TypeIndex to build.

The raw values for memory usage of each index is, interestingly, the exact same.
However, there are only 10 entries in the TypeIndex and over 2200 in the OffsetIndex.
We see again that the TypeIndex is *much* more memory intensive than the OffsetIndex.

Created by Evan Douglass
April 03 2019
