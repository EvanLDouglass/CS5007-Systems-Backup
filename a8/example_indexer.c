/*  Modified by Evan Douglass, March 26 2019.
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
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>

#include "htll/LinkedList.h"
#include "MovieIndex.h"
#include "Movie.h"
#include "MovieReport.h"
#include "FileParser.h"


int main(int argc, char* argv[]) {
  // TODO: Parse command-line args to index based on the correct field (DONE)
  if (argc < 3) {
    printf("Wrong number of arguments.\n");
    printf("usage: example_indexer.c <field_to_index> <directory_to_crawl>\n");
    return 0;
  }
  // Determine which flag was picked, defaults to genre
  int indexBy;
  int opt = getopt(argc, argv, "tyg");
  switch (opt) {
    case 't':
      indexBy = Type;
      break;
    case 'y':
      indexBy = Year;
      break;
    case 'g':
    default:
      indexBy = Genre;
  }
  
  // Assumes the 2nd argument is the filename
  printf("file: %s\n", argv[2]);
  LinkedList movie_list = ReadFile(argv[2]);
  Index index = BuildMovieIndex(movie_list, indexBy);

  // TODO: Output report to file, rather than terminal (see MovieReport.h)
  PrintReport(index);

  DestroyTypeIndex(index);

  return 0;
}

