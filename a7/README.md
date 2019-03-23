```FileParser```:
Responsible for reading in a file, checking for errors, and parse out movies.


```Movie```:
A struct holding all the Movie info (id, title, type, genre, etc.)


```MovieIndex```:
A hashtable that indexes movies according to specified field. Basically wraps a hashtable with functionality specific to adding/removing/destroying with MovieSets. The key is the description for the document set, and the value is the document set.


```MovieReport```:
Responsible for printing out a report given a MovieIndex, to the command line or file.


```MovieSet```:
A set of Movies that are connected somehow: they have the same value for a given field. If the field is Year, all the movies in the set are made in the same year. It consists of a description and a LinkedList of Movies.

The basic tasks you need to complete:

* Modify main to accept flags on input and do the right thing.
* Modify Movie struct to hold an array of genres
* Modify MovieSet to ensure no duplicates of movies
* Bulk up tests

To do this, go through each file, and identify the "TODO" tags (listed below; generated with ```grep -n TODO *.c```). 

## A list of the TODOs and some more details

### TODOs where you should change the code/implement something

* example_indexer.c:17:  // TODO: Parse command-line args to index based on the correct field
* example_indexer.c:20:  // TODO: Output report to file, rather than terminal (see MovieReport.h)
* FileParser.c:45:  // TODO: Check that there is at least one movie
* Movie.c:14:  // TODO: Populate/Initialize movie.
* Movie.c:66:  // TODO: Change such that genres is an array, not just a string.
* MovieIndex.c:47:        // TODO: What to do if there are multiple genres? (Also make a note of what you did in the README)
* MovieIndex.c:71:      // TODO: how to deal with multiple genres?? 
* MovieReport.c:57:  // TODO: Implement this.
* MovieReport.c:63:  // TODO: Implement this. You might utilize OutputReport.

### TODOs that you should address in your README.md

* FileParser.c:37:  // TODO: How big to make this hashtable? How to decide? What to think about?
* FileParser.c:55:      // TODO: What to do if there is a collision?
* MovieIndex.c:47:        // TODO: What to do if there are multiple genres? (Also implement it)

# My Answers
__*How big to make this hashtable? How to decide? What to think about?*__
My main goal in deciding how big to make this hashtable is to minimize collisions while also minimizing the space in memory that it takes up. However, I don't care as much about memory space as I do about collisions and speed performance because we are working on modern computers. It is also worth noting that every time an element is added to the hashtable, our implementation will check if the load factor is over 3, and resize it by 9x if it is. So, once everything has been loaded up, we can be sure our load factor is a reasonable size. At this point my main concern is how many resizes will happen while we are loading data. Starting with a small hashtable could greatly increase the time to read all the data.

Now, our movie index will be made up of keys and MovieSets, so any movie with the same key will be added to a set of similar movies. This means our initial size should be based off of the largest possible field set. The largest field set is probably either Year or Genre. According to Google, the first moving picture was made in 188, 131 years ago. While there are a lot of niche genres, I'd guess year is the bigger field set. At this point the chosen size is fairly arbitrary, but I think a reasonable size is twice the amount of the largest field set. To play it safe, I've chosen to start the hashtable at a size of 300.

Evan Douglass
Started March 22 2019
