#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "MovieIndex.h"
#include "MovieReport.h"
#include "Movie.h"
#include "MovieSet.h"
#include "htll/LinkedList.h"
#include "htll/Hashtable.h"


//  Prints a report of an index, assuming the key
//  is a field/string and the value is a SetOfMovies.
void PrintReport(Index index) {
  OutputReport(index, stdout);
}

void OutputListOfMovies(LinkedList movie_list, char *desc, FILE *file) {
  fprintf(file, "%s: %s\n", "indexType", desc);
  fprintf(file, "%d items\n", NumElementsInLinkedList(movie_list));
  LLIter iter = CreateLLIter(movie_list);
  if (iter == NULL) {
    fprintf(file, "iter null for some reason.. \n");
    return;
  }
  Movie *movie;

  LLIterGetPayload(iter, (void**)&movie);
  if (movie->title != NULL)
    fprintf(file, "\t%s\n", movie->title);
  else
    fprintf(file, "title is null\n");

  while (LLIterHasNext(iter)) {
    LLIterNext(iter);
    LLIterGetPayload(iter, (void**)&movie);
    if (movie->title != NULL)
      fprintf(file, "\t%s\n", movie->title);
    else
      fprintf(file, "title is null\n");
  }

  DestroyLLIter(iter);
}

// Same as output list of movies, but filters by a term in the title
void OutputListOfMoviesFilterBy(LinkedList movie_list, char *desc, FILE *file, char* filter) {
  fprintf(file, "%s: %s\n", "indexType", desc);
  fprintf(file, "%d items\n", NumElementsInLinkedList(movie_list));
  LLIter iter = CreateLLIter(movie_list);
  if (iter == NULL) {
    fprintf(file, "iter null for some reason.. \n");
    return;
  }
  Movie *movie;

  LLIterGetPayload(iter, (void**)&movie);
  if (movie->title != NULL && (strstr(movie->title, filter) != NULL))
    fprintf(file, "\t%s\n", movie->title);

  while (LLIterHasNext(iter)) {
    LLIterNext(iter);
    LLIterGetPayload(iter, (void**)&movie);
    if (movie->title != NULL && (strstr(movie->title, filter) != NULL))
      fprintf(file, "\t%s\n", movie->title);
  }

  DestroyLLIter(iter);
}

// Assumes the value of the index is a SetOfMovies
void OutputReport(Index index, FILE* output) {
  // Create Iter
  HTIter iter = CreateHashtableIterator(index->ht);
  HTKeyValue movie_set;

  HTIteratorGet(iter, &movie_set);

  OutputListOfMovies(((SetOfMovies)movie_set.value)->movies,
                     ((SetOfMovies)movie_set.value)->desc,
                     output);

  while (HTIteratorHasMore(iter)) {
    HTIteratorNext(iter);
    HTIteratorGet(iter, &movie_set);
    OutputListOfMovies(((SetOfMovies)movie_set.value)->movies,
                       ((SetOfMovies)movie_set.value)->desc,
                       output);
  }

  DestroyHashtableIterator(iter);
}

void SaveReport(Index index, char* filename) {
  // Open file
  FILE * fp;
  fp = fopen(filename, "w+");

  // Pass to OutputReport
  OutputReport(index, fp);

  // Close file
  fclose(fp);
}
