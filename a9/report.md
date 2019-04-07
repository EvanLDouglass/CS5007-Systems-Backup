# A9 Report

## Part 1

### Hypotheses

#### Which Index is bigger?

I believe the TypeIndex is bigger than the OffsetIndex given that they have the same number of entries.
Both have the same basic structure, but the OffsetIndex contains integers while the TypeIndex contains full `Movie` structs.
Each `Movie` contains three integers, three strings, and an array of strings.
Thus, every entry in a TypeIndex is several times bigger than an entry in an OffsetIndex.

#### Which Index is faster to search

I think the TypeIndex will be faster to search. Unlike the OffestIndex, the TypeIndex has all of the data it needs to make a report saved in memory already.
The OffsetIndex saves only a reference to a file and line, then has to go grab each line, parse them each into a `Movie` and then output the report.
When we make the search "fair", by searching for a term in the title and an genre - in this case "Seattle" and "Crime" respectively - The TypeIndex will have to do some extra searching through title strings.
But this is much less than the OffsetIndex still has to search through in the files.
All the processing that needs to be done on the fly in an OffsetIndex should make it slower than the TypeIndex.

#### Which Index is faster to build?

For the same reason that the OffsetIndex should be slower to search, the TypeIndex should be slower to build, assuming both have the same number of movies.
Thus the OffsetIndex should be faster to build.
This is because the TypeIndex has to parse each line into a `Movie` at the outset, while the OffsetIndex just needs index files.

Results of the initial benchmarker test are below.
```
22197 entries in the index.
Took 0.309746 seconds to execute.
Memory usage:
Cur Real Mem: 28768     Peak Real Mem: 28768     Cur VirtMem: 33572     PeakVirtMem: 33572


Building the TypeIndex
51 entries in the index.
Took 0.104572 seconds to execute.
Memory usage:
Cur Real Mem: 37216     Peak Real Mem: 37216     Cur VirtMem: 42152     PeakVirtMem: 42152


Searching for "Seattle" with genre == "Crime"
returning movieset
Getting docs for movieset term: "seattle"
genre: Crime
indexType: Results
1 items
        Wasted in Seattle
Took 0.001325 seconds to execute.
Memory usage:
Cur Real Mem: 37216     Peak Real Mem: 37216     Cur VirtMem: 42152     PeakVirtMem: 42152


Searching for "Crime" with "Seattle" in title
indexType: Results
703 items
        Wasted in Seattle
Took 0.000340 seconds to execute.
Memory usage:
Cur Real Mem: 37216     Peak Real Mem: 37216     Cur VirtMem: 42152     PeakVirtMem: 42152


Destroyed All Indexes
Cur Real Mem: 37216     Peak Real Mem: 37216     Cur VirtMem: 42152     PeakVirtMem: 42152
```
These results are slightly misleading at first glance, but seem to confirm my hypotheses.
We can see that building the OffsetIndex took a longer time than the TypeIndex, which is the opposite of what I hypothesized.
However, there are over 22000 more entries in the OffsetIndex and it only took about 3 times longer to build.
This suggests that the OffsetIndex is *much* faster than the TypeIndex to build.

The raw values for memory usage of each index was the exact same the first time I ran this benchmark.
Because of a piazza post, I have since changed the code so that all of the structures are freed at the end of main, instead of as soon as the test was over.
This allows us to see the memory usage of each test though the relative differences.
Doing the math, we can see that for the TypeIndex there is 8448 bytes of "Cur Real Mem" and 8580 bytes of "Cur VirtMem".
These are the same numbers for the "Peak" categories.
Again, while the OffsetIndex uses more memory overall, it also has 22000 more entries and uses only about 3.4 times more real memory and 3.9 times more virtual memory.
We see again that the TypeIndex is *much* more memory intensive than the OffsetIndex.

Moving on to the search numbers, I will ignore the memory here and only focus on the time taken to execute.
When first searching for a term in a movie title, then filtering by genre, the search in this case took 0.001325 seconds.
When first searching for a genre, then filtering by a term in the title, the search took 0.000340 seconds.
It's hard to compare these numbers directly because of the different implementations for each.
Given more time to think of how to make the searches more even, I could make more conclusive statements here.
However, I think the data suggests that my hypothesis was correct, that searching in the TypeIndex was faster.
Given that a `SetOfMovies` contains `Movie` structs, and given that the index is a hashtable, it is very fast to simply grab a `SetOfMovies` given a genre key and then iterate through it to filter out movies based on their title.
The "Title First" search has to do much more processing than that, including iterating through various text files and creating `Movie` structs, and only after that filtering by genre.

## Part 2

From here on I will only be including information on the `MovieSet` Benchmark, as it is the only one affected by multi-threading.

With the machine still using one core, I found that the Benchmark ran slower when using multi-threading than when not.
It also used slightly more memory
```
22197 entries in the index.
Took 0.322125 seconds to execute.
Memory usage:
Cur Real Mem: 29036     Peak Real Mem: 29036     Cur VirtMem: 367184    PeakVirtMem: 375380
```
This is to be expected since the machine is really conducting something more like *pseudo-multi-threading*.
It will naturally take more time because of the overhead needed to create new threads and use mutex locks.

At this point I increased the number of CPUs on my virtual machine to two.
This is the number of CPUs on my actual laptop, so I did not add any more than that.
This is also where things get interesting.
I had thought that increasing the number of CPUs would increase the speed of the Benchmark, but I found the opposite.
```
22197 entries in the index.
Took 0.345632 seconds to execute.
Memory usage:
Cur Real Mem: 29040     Peak Real Mem: 29040     Cur VirtMem: 367184    PeakVirtMem: 506452
```
Surprisingly, this took even longer than the multi-threaded version of this test on a single CPU.
Initially, I thought something was wrong with my implementation, so I did some tests to characterize my program.

First, I needed to confirm that two CPUs were indeed available.
Below are two lines from the output of `lscpu` that show my VM is using two CPUs
```
CPU(s):              2
On-line CPU(s) list: 0,1
``` 

My next thought was that my program wasn't actually multi-threading.
However, I was able to confirm that I am actually multi-threading because when I add some strategic print statements I get something like this 24 times:
```
parsing file: data_small/xap, id: 7
parsing file: data_small/xbf, id: 10
parsing file: data_small/xbe, id: 9
parsing file: data_small/efg/xcn, id: 6
parsing file: data_small/xbd, id: 8
End of thread w/ file id: 7
End of thread w/ file id: 6
End of thread w/ file id: 8
End of thread w/ file id: 10
End of thread w/ file id: 9
``` 

Since the program is multi-threaded, I thought that maybe I was running too many threads for only using 2 CPUs, but when I run the test with only two threads it takes even longer
```
22197 entries in the index.
Took 0.435322 seconds to execute.
Memory usage:
Cur Real Mem: 27844     Peak Real Mem: 27844     Cur VirtMem: 154180    PeakVirtMem: 219716
```

This gets better with 3 and 4 threads, but never gets better than the serial version of the functions.
```
Time with 3 threads:
Took 0.369522 seconds to execute.
Time with 4 threads:
Took 0.360956 seconds to execute.
```

I thought after that the problem might be that I was creating threads of `IndexTheFiles_MT` from `ParseTheFiles_MT`, when I needed to be creating threads of `ParseTheFiles_MT` itself.
I tested this by creating a new branch with git and implementing it.
I still found that the Benchmark took more time than the serial version, although less time than my original implementation.
```
21397 entries in the index.
Took 0.312130 seconds to execute.
Memory usage:
Cur Real Mem: 28028     Peak Real Mem: 28028     Cur VirtMem: 367184    PeakVirtMem: 375380
```
Of course, it is worth noting that there are more entries in this version than the original, so it was not perfect.

At this point, I feel that I have exhausted my avenues of testing.
It is entirely likely that something about my implementation is not correct, and that is why I'm seeing the characteristics shown above.
However, it may also be the nature of the problem.
There is not a ton of work being done in `IndexTheFiles_MT` besides in a function called within it.
That function happens to have a mutex lock on it as well.
So, if the bulk of the work is under a lock, then all the threads will be bottlenecked at that point.
There is also the extra overhead of creating 5 threads during every iteration of a `while` loop in `ParseTheFiles_MT`.
Perhaps both of these factors together lead to a slower execution time.


Created by Evan Douglass
April 03 2019
