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
22197 entries in the index.
Took 0.340117 seconds to execute.
Memory usage:
Cur Real Mem: 28744     Peak Real Mem: 28744     Cur VirtMem: 33572     PeakVirtMem: 33572


Building the TypeIndex
10 entries in the index.
Took 0.112338 seconds to execute.
Memory usage:
Cur Real Mem: 36928     Peak Real Mem: 36928     Cur VirtMem: 41756     PeakVirtMem: 41756


Destroyed All Indexes
Cur Real Mem: 36928     Peak Real Mem: 36928     Cur VirtMem: 41756     PeakVirtMem: 41756
```
These results are slightly misleading at first glance, but seem to confirm my hypotheses.
We can see that building the OffsetIndex took a longer time than the TypeIndex, which is the opposite of what I hypothesized.
However, there are nearly 2000 times more entries in the OffsetIndex and it only took about 3.5 times longer to build.
This suggests that the OffsetIndex is *much* faster than the TypeIndex to build.

The raw values for memory usage of each index was the exact same the first time I ran this benchmark.
Because of a piazza post, I have since changed the code so that all of the structures are freed at the end of main, instead of as soon as the test was over.
This allows us to see the memory usage of each test though the relative differences.
Doing the math, we can see that for the TypeIndex there is 8184 bytes of "Cur Real Mem" and 8184 bytes of "Cur VirtMem".
These are the same numbers for the "Peak" categories.
Again, while the OffsetIndex uses more memory overall, it also has 2000 more entries and uses only (about) 3.5 times more real memory and 4.1 times more virtual memory.
We see again that the TypeIndex is *much* more memory intensive than the OffsetIndex.

Created by Evan Douglass
April 03 2019
