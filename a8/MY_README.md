# Questions from TODO tags

```c
FileParser.c:135: // TODO: How big to make this hashtable? How to decide? What to think about?
MovieIndex.c:50: // TODO: How big to make this hashtable? How to decide? What to think about?
```
Both of these TODOs reference the same thing.
That is, How big should we make the hashtable in the Index type.
The default value is 128.
I think that is an appropriate number because it is roughly the number of years that "moving pictures" have existed.
Between the different things we can index on - Type, Year, Genre - I think there are more values for year than an of the other choices.
There are a lot of different formats of video, and a lot of niche generes you can find, but I don't think they reach the same bredth as Year.
Thus, with 128 buckets, we can be relatively certain that our load factor will be reasonable, as there is about one bucket per year and whe should not need any more than that.

```c
MovieIndex.c:87: // TODO: How to choose this number? What are the pros and cons of choosing the wrong number?
```
In this case the number is referring to a variable called `numFields`, which is set to `1000`.
`numFields` represents the number of words in a title of a movie.
The number 1000 is somewhat arbitrary, but is large enough to ensure that any title can fit in that array when split up by word. It is a common thing in schools to be asked to write a 1000 word essay, so we can be pretty sure that no title will overflow the array. On the otherhand, we will have way more space in this array than needed for just about every title, so that is a lot of wasted memory.
However, since we are programming on modern computers, memory is not so much of an issue when it comes to 1000 spaces of a char* array.

```c
FileParser.c:154: // TODO: What to do if there is a collision?
```
The way my functions are set up, this question is taken care of in `AddMovieToSetOfMovies`, which is called inside `AddMovieToIndex`.
Inside `AddMovieToSetOfMovies`, the set - a linked list - is searched to determine if the movie already exists in it, which is determined by the id field of the movie.
If the movie does exist in the set, then nothing happens, if it doesn't, then it is added to the linked list.
The second part of this TODO asks how we can change the sytstem to do this check for us.
What I've described here is exactly how we can change the system.
This also makes the `if` statement that this TODO is nested in irrelevant, and in my final code I have deleted it.

```c
main.c:95: // TODO: Now that you have all the search results, print them out nicely.
```
We can do this fairly easily by building an index of the movies we have created and then printing a report to the console.
I decided to index the movies by Type because I feel it is the most relavent index type when searching by title.
I don't have any particular reason to feel this way, it is just how I would like to organize things.

Evan Douglass<br>
Created March 24 2019
