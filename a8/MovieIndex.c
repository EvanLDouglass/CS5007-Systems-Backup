/*  Modified by Evan Douglass, March 24 2019.
 *
 *  Created by Adrienne Slaughter
 *  CS 5007 Spring 2019
 *  Northeastern University, Seattle
 *
 *  This is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  It is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  See <http://www.gnu.org/licenses/>.
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "MovieIndex.h"
#include "htll/LinkedList.h"
#include "htll/Hashtable.h"
#include "Movie.h"
#include "MovieSet.h"

void DestroyMovieSetWrapper(void *movie_set) {
  DestroyMovieSet((MovieSet)movie_set);
}

void toLower(char *str, int len) {
  for (int i = 0; i < len; i++) {
    str[i] = tolower(str[i]);
  }
}

int AddMovieTitleToIndex(Index index,
                         Movie *movie,
                         uint64_t doc_id,
                         int row_id) {
  // Put in the index
  HTKeyValue kvp;

  // TODO: How to choose? (DONE)
  int numFields = 150;

  char *token[numFields];
  char *rest = movie->title;

  int i = 0;
  token[i] = strtok_r(rest, " ", &rest);
  while (token[i] != NULL) {
    toLower(token[i], strlen(token[i]));
    i++;
    token[i] = strtok_r(rest, " ", &rest);
  }

  for (int j = 0; j < i; j++) {
    // If this key is already in the hashtable, get the MovieSet.
    // Otherwise, create a MovieSet and put it in.
    int result = LookupInHashtable(index,
                          FNVHash64((unsigned char*)token[j],
                                    (unsigned int)strlen(token[j])), &kvp);
    HTKeyValue old_kvp;

    if (result < 0) {
      kvp.value = (void*) CreateMovieSet(token[j]);
      kvp.key = FNVHash64((unsigned char*)token[j], strlen(token[j]));
      PutInHashtable(index, kvp, &old_kvp);
    }

    AddMovieToSet((MovieSet)kvp.value, doc_id, row_id);
  }

  return 0;
}

// Gets genres from the given movie and adds the movie
// to an index under each genre seperately
// Returns 0 if successful or a 1-indexed number representing
// which genre the error occured at.
int AddGenresToIndex(Index index, Movie *movie) {
  int num_genres = NumElementsInLinkedList(movie->genres);
  LLIter iter = CreateLLIter(movie->genres);
  for (int i = 0; i < num_genres; i++) {
    char* genre;
    int result;
    HTKeyValue kvp, old_kvp;

    // Get a genre
    result = LLIterGetPayload(iter, (void**)&genre);
    if (result == 1) {
      return i + 1;  // return 1-indexed genre on fail
    }

    // Look for existing keys
    uint64_t key = FNVHash64((unsigned char*)genre, strlen(genre));
    result = LookupInHashtable(index, key, &kvp);

    if (result < 0) {
      // Need to add a new kvp
      kvp.value = (void*) CreateSetOfMovies(genre);
      kvp.key = key;
      PutInHashtable(index, kvp, &old_kvp);
    }
    // There is an existing kvp
    AddMovieToSetOfMovies((SetOfMovies)kvp.value, movie);

    LLIterNext(iter);
  }
  return 0;
}

int AddMovieToIndex(Index index, Movie *movie, enum IndexField field) {
  if (field == Genre) {
    return AddGenresToIndex(index, movie);
  }

  // Put in the index
  HTKeyValue kvp;
  HTKeyValue old_kvp;

  // If this key is already in the hashtable, get the MovieSet.
  // Otherwise, create a MovieSet and put it in.
  int result = LookupInHashtable(index,
                                 ComputeKey(movie, field),
                                 &kvp);

  if (result < 0) {
    char* doc_set_name;
    char year_str[10];
    switch (field) {
      case Type:
        doc_set_name = movie->type;
        break;
      case Year:
        snprintf(year_str, sizeof(year_str), "%d", movie->year);
        doc_set_name = year_str;
        break;
      case Id:
        doc_set_name = movie->id;
        break;
      case Genre:
        // TODO: What to do if there are multiple genres?
        // Handle genre indexing as a completely different case
        break;
    }
    kvp.value = (void*)CreateSetOfMovies(doc_set_name);
    kvp.key = ComputeKey(movie, field);
    PutInHashtable(index, kvp, &old_kvp);
  }
  // TODO: Something needs to happen here
  AddMovieToSetOfMovies((SetOfMovies)kvp.value, movie);  

  return 0;
}

// This function has been modified so that calling it with Genre as
// the field will do nothing and return 1
uint64_t ComputeKey(Movie* movie, enum IndexField which_field) {
  switch(which_field) {
    case Year:
      return FNVHashInt64(movie->year);
      break;
    case Type:
      return FNVHash64((unsigned char*)movie->type, strlen(movie->type));
      break;
    case Id:
      return FNVHash64((unsigned char*)movie->id, strlen(movie->id));
      break;
    case Genre:
      // TODO: how to deal with multiple genres??
      // This is handled by a different function
      return 1u;
  }
  return -1u;
}


MovieSet GetMovieSet(Index index, const char *term) {
  HTKeyValue kvp;
  char lower[strlen(term)+1];
  strcpy(lower, term);
  toLower(lower, strlen(lower));
  int result = LookupInHashtable(index,
                                 FNVHash64((unsigned char*)lower,
                                           (unsigned int)strlen(lower)),
                                 &kvp);
  if (result < 0) {
    printf("term couldn't be found: %s \n", term);
    return NULL;
  }
  return (MovieSet)kvp.value;
}


int DestroyIndex(Index index) {
  DestroyHashtable(index, &DestroyMovieSetWrapper);
  return 0;
}

Index CreateIndex() {
  return CreateHashtable(128);
}
