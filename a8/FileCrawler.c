/*  Modified by Evan Douglass, March 23 2019.
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
#include <sys/stat.h>
#include <dirent.h>

#include "FileCrawler.h"
#include "DocIdMap.h"
#include "LinkedList.h"


void CrawlFilesToMap(char *dir, DocIdMap map) {
  struct stat s;

  struct dirent **namelist;
  int n;
  n = scandir(dir, &namelist, 0, alphasort);

  // TODO: use namelist to find all the files and put them in map. (DONE)
  // NOTE: There may be nested folders.
  // Be sure to lookup how scandir works. Don't forget about memory use.
  
  // For each name in namelist, need to:
  //   test if name is a directory with stat struct
  //   if it is, recursively call this function (ignore . & ..)
  //   if it isn't, add it to map

  for (i = 0; i < n; i++) {
    char* name = namelist[i]->d_name;  // name of entry

    // Test for dir found on stackoverflow at:
    // https://stackoverflow.com/questions/3828192/
    // checking-if-a-directory-exists-in-unix-system-call
    if (stat(name, &s) == 0 && S_ISDIR(s.st_mode)) {
      // Also test for cur & parent directory (seperated for readability)
      if (name != "." && name != "..") {
        CrawlFilesToMap(name, map);
      }
    }

    // For files:
    // Map needs malloc'ed name => done in scandir
    PutFileInMap(name, map);
  }
  // Still need to free namelist
  free(namelist);
}
