# Questions from TODO tags

I have entered TODO questions as I came across them while completing this assignment.
Each flag is shown as it is in the code with an answer below.

**In MovieIndex.c:**
```c
// TODO: How to choose?
int numFields = 1000;
```
The `numFields` variable is being used to set the length of an array of strings.
The array holds each word in the title of a movie.
Therefore, 1000 words is probably larger than we need.
I'd even consider 10 words to be a fairly long title for a movie or tv show episode.
Of course, people will surprise you sometimes with the choices they make, and I wouldn't be surprised if there was at least one title in our data that is abnormally long.
I think it is safe to assume there is no title longer than 100 words, but just to be safe, and because we have plenty of memory avialable, I decided to make `numFields` equal to 150.

```c
// TODO: What to do if there are multiple genres?
```
This TODO is in the AddMovieToIndex function.
It is asking how to deal with a movie if it has multiple genres.
I believe that if a movie has multiple genres, we need to index it by each genre seperately.
This is because if we were to search for comedies, for example, we want everything tagged as a comedy to appear.
If we indexed by all the genres as one string, then a search for comedy would not include things that are listed as action,adventure,comedy, for example.
In order to make this work I will have to rewrite the function so that it handles genre indexing as a totally seperate task.

```c
// TODO: how to deal with multiple genres??
```
This TODO is in the ComputeKey function.
It is different from the last in that it requires that we think about a different problem.
That is, we need to compute the key of each genre individually.
The way I decided to deal with this is to use a different function to work with genre indexing entirely.
The ComputeKey function now only deals with Year, Type, and Id fields.

Evan Douglass<br>
Created March 24 2019
