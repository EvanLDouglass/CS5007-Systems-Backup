# Questions from TODO tags

I have entered TODO questions as I came across them while completing this assignment.
Each flag is shown as it is in the code with an answer below.

*In FileParser.c:*
```c
// TODO: How to choose?
int numFields = 1000;
```
The `numFields` variable is being used to set the length of an array of strings.
The array holds each word in the title of a movie. Therefore, 1000 words is probably larger than we need.
I'd even consider 10 words to be a fairly long title for a movie or tv show episode.
Of course, people will surprise you sometimes with the choices they make, and I wouldn't be surprised if
there was at least one title in our data that is abnormally long.
I think it is safe to assume there is no title longer than 100 words, but just to be safe, and
because we have plenty of memory avialable, I decided to make `numFields` equal to 150.

Evan Douglass<br>
Created March 24 2019
