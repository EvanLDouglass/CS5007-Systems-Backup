/*  Modified by Evan Douglass, April 03 2019.
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
#include <sys/time.h>
#include <time.h>
#include <pthread.h>

#include "MovieIndex.h"
#include "FileParser.h"
#include "Movie.h"
#include "DocIdMap.h"
#include "MovieSet.h"

//  Only for NullFree; TODO(adrienne): NullFree should live somewhere else.

#define BUFFER_SIZE 1000

struct indexMTArgs {
  char *file;
  uint64_t doc_id;
  Index index;
};

pthread_mutex_t m_open = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t m_add = PTHREAD_MUTEX_INITIALIZER;

//=======================
// To minimize the number of files we have, I'm
// putting the private function prototypes for
// the fileparser here.

void IndexTheFile(char *file, uint64_t docId, Index index);

void IndexTheFile_MT(void* arguments);


/**
 * \fn Parses the files that are in a provided DocIdMap.
 * Builds an OffsetIndex
 */
int ParseTheFiles(DocIdMap docs, Index index) {
  HTIter iter = CreateHashtableIterator(docs);
  HTKeyValue kv;

  HTIteratorGet(iter, &kv);
  IndexTheFile(kv.value, kv.key, index);

  while (HTIteratorHasMore(iter) != 0) {
    HTIteratorGet(iter, &kv);
    IndexTheFile(kv.value, kv.key, index);
    HTIteratorNext(iter);
  }

  HTIteratorGet(iter, &kv);
  IndexTheFile(kv.value, kv.key, index);

  DestroyHashtableIterator(iter);

  return 0;
}

// Builds an OffsetIndex
void IndexTheFile(char *file, uint64_t doc_id, Index index) {
  FILE *cfPtr;

  if ((cfPtr = fopen(file, "r")) == NULL) {
    printf("File could not be opened\n");
    return;
  } else {
    char buffer[BUFFER_SIZE];
    int row = 0;

    while (fgets(buffer, BUFFER_SIZE, cfPtr) != NULL) {
      Movie *movie = CreateMovieFromRow(buffer);
      if (movie == NULL) {
        continue;
      }
      int result = AddMovieTitleToIndex(index, movie, doc_id, row);
      if (result < 0) {
        fprintf(stderr, "Didn't add MovieToIndex.\n");
      }
      row++;
      DestroyMovie(movie);  // Done with this now
    }
    fclose(cfPtr);
  }
}

/**
 * Parses the files that are in the provided DocIdMap, 
 * utilizing multithreading.
 * Builds an OffsetIndex.
 */
int ParseTheFiles_MT(DocIdMap docs, Index index) {
  HTIter iter = CreateHashtableIterator(docs);
  HTKeyValue kv;

  // Multithreading vars
  pthread_t p1, p2, p3, p4, p5;

  struct indexMTArgs args[5];
  while (1) {
    // Iterate using 5 threads
    // First thread
    HTIteratorGet(iter, &kv);
    args[0].file = kv.value;
    args[0].doc_id = kv.key;
    args[0].index = index;
    pthread_create(&p1, NULL, (void*)IndexTheFile_MT, (void*)&args[0]);
    HTIteratorNext(iter);

    // Second thread
    if (HTIteratorHasMore(iter) != 0) {
      HTIteratorGet(iter, &kv);
      args[1].file = kv.value;
      args[1].doc_id = kv.key;
      args[1].index = index;
      pthread_create(&p2, NULL, (void*)IndexTheFile_MT, (void*)&args[1]);
      HTIteratorNext(iter);
    }

    // Third thread
    if (HTIteratorHasMore(iter) != 0) {
      HTIteratorGet(iter, &kv);
      args[2].file = kv.value;
      args[2].doc_id = kv.key;
      args[2].index = index;
      pthread_create(&p3, NULL, (void*)IndexTheFile_MT, (void*)&args[2]);
      HTIteratorNext(iter);
    }

    // Fourth thread
    if (HTIteratorHasMore(iter) != 0) {
      HTIteratorGet(iter, &kv);
      args[3].file = kv.value;
      args[3].doc_id = kv.key;
      args[3].index = index;
      pthread_create(&p4, NULL, (void*)IndexTheFile_MT, (void*)&args[3]); 
      HTIteratorNext(iter);
    }

    // Last thread
    if (HTIteratorHasMore(iter) != 0) {
      HTIteratorGet(iter, &kv);
      args[4].file = kv.value;
      args[4].doc_id = kv.key;
      args[4].index = index;
      pthread_create(&p5, NULL, (void*)IndexTheFile_MT, (void*)&args[4]); 
      HTIteratorNext(iter);
    }

    // Wait for the threads before moving on
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    pthread_join(p3, NULL);
    pthread_join(p4, NULL);
    pthread_join(p5, NULL);

    if (HTIteratorHasMore(iter) == 0) {
      break;
    }
  }

  DestroyHashtableIterator(iter);
  pthread_mutex_destroy(&m_open);
  pthread_mutex_destroy(&m_add);
  return 0;
}

// Builds an OffsetIndex using multithreading
void IndexTheFile_MT(void* arguments) {
  FILE *cfPtr;
  // Unpack the arguments
  struct indexMTArgs* args = (struct indexMTArgs*) arguments;
  char* file = args->file;
  uint64_t doc_id = args->doc_id;
  Index index = args->index;
printf("file name: %s, id: %ld\n", file, doc_id);
  cfPtr = fopen(file, "r");

  if (cfPtr == NULL) {
    printf("File could not be opened\n");
    return;
  } else {
    char buffer[BUFFER_SIZE];
    int row = 0;

    while (fgets(buffer, BUFFER_SIZE, cfPtr) != NULL) {
      Movie *movie = CreateMovieFromRow(buffer);
      if (movie == NULL) {
        continue;
      }
      pthread_mutex_lock(&m_add);
      int result = AddMovieTitleToIndex(index, movie, doc_id, row);
      pthread_mutex_unlock(&m_add);

      if (result < 0) {
        fprintf(stderr, "Didn't add MovieToIndex.\n");
      }
      row++;
      DestroyMovie(movie);  // Done with this now
    }
    fclose(cfPtr);
  }
printf("End of thread for file %s\n", file);
}

// Takes a linkedlist of movies, and builds a hashtable based on the given field
// Builds a TypeIndex
Index BuildMovieIndex(LinkedList movies, enum IndexField field_to_index) {
  Index movie_index = CreateIndex();
  movie_index->movies = movies;

  LLIter iter = CreateLLIter(movies);
  Movie* cur_movie;
  LLIterGetPayload(iter, (void**)&cur_movie);

  AddMovieToIndex(movie_index, cur_movie, field_to_index);

  while (LLIterHasNext(iter)) {
    LLIterNext(iter);
    LLIterGetPayload(iter, (void**)&cur_movie);
    AddMovieToIndex(movie_index, cur_movie, field_to_index);
  }

  DestroyLLIter(iter);

  return movie_index;
}


// Takes a linkedlist of movies, and builds a hashtable based on the given field
// Adds a movie to a TypeIndex
Index AddToMovieIndex(Index movie_index,
                      LinkedList movies,
                      enum IndexField field_to_index) {
  LLIter iter = CreateLLIter(movies);
  Movie* cur_movie;
  LLIterGetPayload(iter, (void**)&cur_movie);

  AddMovieToIndex(movie_index, cur_movie, field_to_index);
  InsertLinkedList(movie_index->movies, cur_movie);

  while (LLIterHasNext(iter)) {
    LLIterNext(iter);
    LLIterGetPayload(iter, (void**)&cur_movie);
    AddMovieToIndex(movie_index, cur_movie, field_to_index);
    InsertLinkedList(movie_index->movies, cur_movie);
  }

  DestroyLLIter(iter);

  //  NullFree because the payloads now live in the HT
  DestroyLinkedList(movies, NullFree);

  return movie_index;
}

// Returns a LinkedList of Movie structs from the specified file
LinkedList ReadFile(const char* filename) {
  FILE *cfPtr;

  LinkedList movie_list = CreateLinkedList();

  if ((cfPtr = fopen(filename, "r")) == NULL) {
    printf("File could not be opened\n");
    DestroyLinkedList(movie_list, NULL);
    return NULL;
  } else {
    char row[BUFFER_SIZE];

    int i = 0;

    while (!feof(cfPtr)) {
      fgets(row, BUFFER_SIZE, cfPtr);
      i++;
      // Got the line; create a movie from it
      MoviePtr movie = CreateMovieFromRow(row);
      if (movie != NULL) {
        InsertLinkedList(movie_list, movie);
      }
    }
    fclose(cfPtr);
  }
  return movie_list;
}
