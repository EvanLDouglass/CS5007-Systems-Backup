/* Modified by Evan Douglass, March 21 2019 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

#include "Hashtable.h"
#include "Hashtable_priv.h"
#include "LinkedList.h"
#include "Assert007.h"

// a free function that does nothing
static void NullFree(void *freeme) { }

static void FreeKVP(void *freeme) {
  free(freeme);
}

Hashtable CreateHashtable(int num_buckets) {
  if (num_buckets == 0)
    return NULL;

  Hashtable ht = (Hashtable)malloc(sizeof(struct hashtableInfo));

  if (ht == NULL) {
    return NULL;
  }

  ht->num_buckets = num_buckets;
  ht->num_elements = 0;
  ht->buckets =
      (LinkedList*)malloc(num_buckets * sizeof(LinkedList));

  if (ht->buckets == NULL) {
    free(ht);
    return NULL;
  }

  for (int i=  0; i < num_buckets; i++) {
    ht->buckets[i] = CreateLinkedList();
    if (ht->buckets[i] == NULL) {
      // Need to free everything and then return NULL
      for (int j = 0; j < i; j++) {
        DestroyLinkedList(ht->buckets[j], NULL);
      }
      free(ht);
      return NULL;
    }
  }
  return ht;
}


void DestroyHashtable(Hashtable ht, ValueFreeFnPtr valueFreeFunction) {
  // Go through each bucket, freeing each bucket
  for (int i = 0; i < ht->num_buckets; i++) {
    LinkedList bucketlist = ht->buckets[i];
    HTKeyValuePtr nextKV;

    // Free the values in the list; then free the list
    if (NumElementsInLinkedList(bucketlist) > 0) {
      LLIter list_iter = CreateLLIter(bucketlist);

      LLIterGetPayload(list_iter, (void**)&nextKV);
      valueFreeFunction(nextKV->value);

      // Now loop through the rest
      while (LLIterHasNext(list_iter) == 1) {
        LLIterNext(list_iter);
        LLIterGetPayload(list_iter, (void**)&nextKV);
        valueFreeFunction(nextKV->value);
      }
      DestroyLLIter(list_iter);
    }
    DestroyLinkedList(bucketlist, FreeKVP);
  }

  // free the bucket array within the table record,
  // then free the table record itself.
  free(ht->buckets);
  free(ht);
}

// Looks for a key in a given linked list. Copies the HTKeyValue
// in the payload if there is one, or makes it NULL if not. Optionally
// removes the matched LL node using the dummy free function at the top
// of this file.
// INPUT
//   list: a linked list (ht bucket)
//   key: an integer key to look for
//   old_kvp: an HTKeyValue pointer to put the old kvp into when found
//   remove: an int indicating if a found kvp's node should be deleted
//     1 - delete, 0 - don't delete
// Returns 0 if find was successful, -1 if key was not found, 1 if node was
//   deleted.
int FindInList(LinkedList list, uint64_t key, HTKeyValue *old_kvp, int remove) {
  Assert007(list != NULL);
  old_kvp->key = key;

  // If list is empty, nothing found
  if (NumElementsInLinkedList(list) == 0) {
    old_kvp->value = NULL;
    return -1;
  }

  // If at least one element, try to find matching key
  LLIter iter = CreateLLIter(list);
  HTKeyValue* kvpTemp;
  int moved = 0;
  while (moved == 0) {
    LLIterGetPayload(iter, (void**)&kvpTemp);
    // Test for key match
    if (kvpTemp->key == key) {
      old_kvp->value = kvpTemp->value;
      // Delete node if needed
      if (remove == 1) {
        LLIterDelete(iter, (LLPayloadFreeFnPtr)&FreeKVP);
      }
      // Delete iter if not already gone
      DestroyLLIter(iter);
      iter = NULL;
      return remove;  // given int is same as return
    }
    moved = LLIterNext(iter);
  }

  // Nothing was found if this point reached
  DestroyLLIter(iter);
  iter = NULL;
  old_kvp->value = NULL;
  return -1;
}

int PutInHashtable(Hashtable ht,
                   HTKeyValue kvp,
                   HTKeyValue *old_key_value) {
  Assert007(ht != NULL);

  int insert_bucket;
  LinkedList insert_chain;

  ResizeHashtable(ht);

  // calculate which bucket we're inserting into,
  // get the list
  insert_bucket = HashKeyToBucketNum(ht, kvp.key);
  insert_chain = ht->buckets[insert_bucket];

  // STEP 1: Finish the implementation of the put.
  // This is a fairly complex task, so you might decide you want
  // to define/implement a helper function hat helps you find
  // and optionally remove a key within a chain, rather than putting
  // all that logic inside here. You might also find that your helper(s)
  // can be reused in step 2 and 3.

  // Make payload for HT
  HTKeyValue* kvpPayload = (HTKeyValue*)malloc(sizeof(HTKeyValue));
  if (kvpPayload == NULL) {  // malloc failure, no more memory
    free(kvpPayload);
    return 1;
  }
  kvpPayload->key = kvp.key;
  kvpPayload->value = kvp.value;

  // Test if key exists already and delete if there.
  // FindInList tests for an empty list, so I don't need to here.
  // It also copies the found HTKeyValue into the provided pointer..
  int found = FindInList(insert_chain, kvp.key, old_key_value, 1);
  InsertLinkedList(insert_chain, (void*)kvpPayload);
  // Return depending on result of "search and destroy"
  if (found == -1) {  // key not found
    return 0;
  } else {            // key found and node deleted
    return 2;
  }
}

int HashKeyToBucketNum(Hashtable ht, uint64_t key) {
  return key % ht->num_buckets;
}

// -1 if not found; 0 if success
int LookupInHashtable(Hashtable ht, uint64_t key, HTKeyValue *result) {
  Assert007(ht != NULL);

  // STEP 2: Implement lookup
  int insert_bucket;
  LinkedList insert_chain;

  // calculate which bucket we're inserting into,
  // get the list
  insert_bucket = HashKeyToBucketNum(ht, key);
  insert_chain = ht->buckets[insert_bucket];

  return FindInList(insert_chain, key, result, 0);  // don't delete
}


int NumElemsInHashtable(Hashtable ht) {
  int res = 0;
  for (int i = 0; i < ht->num_buckets; i++) {
    res += NumElementsInLinkedList(ht->buckets[i]);
  }
  return res;
}


int RemoveFromHashtable(Hashtable ht, uint64_t key, HTKeyValuePtr junkKVP) {
  // STEP 3: Implement Remove
  Assert007(ht != NULL);

  int insert_bucket;
  LinkedList insert_chain;

  // calculate which bucket we're inserting into,
  // get the list
  insert_bucket = HashKeyToBucketNum(ht, key);
  insert_chain = ht->buckets[insert_bucket];

  int found = FindInList(insert_chain, key, junkKVP, 1);  // delete
  if (found == 1 && junkKVP != NULL) {
    return 0;
  } else {
    return -1;
  }
}

uint64_t FNVHash64(unsigned char *buffer, unsigned int len) {
  // This code is adapted from code by Landon Curt Noll
  // and Bonelli Nicola:
  //
  // http://code.google.com/p/nicola-bonelli-repo/
  static const uint64_t FNV1_64_INIT = 0xcbf29ce484222325ULL;
  static const uint64_t FNV_64_PRIME = 0x100000001b3ULL;
  unsigned char *bp = (unsigned char *) buffer;
  unsigned char *be = bp + len;
  uint64_t hval = FNV1_64_INIT;
  /*
   * FNV-1a hash each octet of the buffer
   */
  while (bp < be) {
    /* xor the bottom with the current octet */
    hval ^= (uint64_t) * bp++;
    /* multiply by the 64 bit FNV magic prime mod 2^64 */
    hval *= FNV_64_PRIME;
  }
  /* return our new hash value */
  return hval;
}


uint64_t FNVHashInt64(uint64_t makehash) {
  unsigned char buf[8];
  int i;
  for (i = 0; i < 8; i++) {
    buf[i] = (unsigned char) (makehash & 0x00000000000000FFULL);
    makehash >>= 8;
  }
  return FNVHash64(buf, 8);
}


void ResizeHashtable(Hashtable ht) {
  Assert007(ht != NULL);

  // Resize if the load factor is > 3.
  if (ht->num_elements < 3 * ht->num_buckets)
    return;

  // This is the resize case.  Allocate a new hashtable,
  // iterate over the old hashtable, do the surgery on
  // the old hashtable record and free up the new hashtable
  // record.
  Hashtable newht = CreateHashtable(ht->num_buckets * 9);
  // Give up if out of memory.
  if (newht == NULL)
    return;

  // Loop through the old ht with an iterator,
  // inserting into the new HT.
  HTIter it = CreateHashtableIterator(ht);
  if (it == NULL) {
    // Give up if out of memory.
    DestroyHashtable(newht, &NullFree);
    return;
  }

  HTKeyValue item;
  HTIteratorGet(it, &item);
  HTKeyValue old_kv;

  if (PutInHashtable(newht, item, &old_kv) == 1) {
    // failure, free up everything, return.
    DestroyHashtableIterator(it);
    DestroyHashtable(newht, &NullFree);
    return;
  }

  while (HTIteratorHasMore(it) != 0) {
    HTIteratorNext(it);

    HTKeyValue item;
    HTIteratorGet(it, &item);
    HTKeyValue old_kv;

    if (PutInHashtable(newht, item, &old_kv) == 1) {
      // failure, free up everything, return.
      DestroyHashtableIterator(it);
      DestroyHashtable(newht, &NullFree);
      return;
    }
  }
  // Worked!  Free the iterator.
  DestroyHashtableIterator(it);
  // Sneaky: swap the structures, then free the new table,
  // and we're done.
  {
    struct hashtableInfo tmp;
    tmp = *ht;
    *ht = *newht;
    *newht = tmp;
    DestroyHashtable(newht, &NullFree);
  }
  return;
}


// ==========================
// Hashtable Iterator
// ==========================

// Returns NULL on failure, non-NULL on success.
HTIter CreateHashtableIterator(Hashtable table) {
  if (NumElemsInHashtable(table) == 0) {
    return NULL;
  }
  HTIter iter = (HTIter)malloc(sizeof(HTIterRecord));
  if (iter == NULL) {
    return NULL;  // Couldn't malloc
  }
  iter->ht = table;
  iter->which_bucket = 0;
  while (NumElementsInLinkedList(iter->ht->buckets[iter->which_bucket]) == 0) {
    iter->which_bucket++;
  }
  iter->bucket_iter = CreateLLIter(iter->ht->buckets[iter->which_bucket]);

  return iter;
}

void DestroyHashtableIterator(HTIter iter) {
  // Destroy the list iter
  free(iter->bucket_iter);
  iter->ht = NULL;
  free(iter);
}

// Moves to the next element; does not return.
int HTIteratorNext(HTIter iter) {
  // Step 4: Implement HTIteratorNext
  Assert007(iter != NULL);
  LLIter iterLL = iter->bucket_iter;

  // Case: There are more elements in this bucket
  if (LLIterHasNext(iterLL) == 1) {
    LLIterNext(iterLL);
    return 0;

  // Case: iter on last element in bucket
  } else {
    // Check subsequent buckets lengths
    int i = iter->which_bucket + 1;
    while (i < (iter->ht->num_buckets)) {
      // If a bucket has at least 1 element, make an iter for that bucket
      if ((iter->ht->buckets[i] != NULL) &&
         (NumElementsInLinkedList(iter->ht->buckets[i]) > 0)) {
        iter->bucket_iter = CreateLLIter(iter->ht->buckets[i]);
        iter->which_bucket = i;

        // Destroy previous iter
        int success = DestroyLLIter(iterLL);
        if (success != 0) {
          return 1;
        }
        iterLL = NULL;

        return 0;
      }
      i++;
    }
    // If no other elements, return 1
    return 1;
  }
}

int HTIteratorGet(HTIter iter, HTKeyValuePtr dest) {
  Assert007(iter != NULL);
  // Step 6 -- implement HTIteratorGet.
  LLIter iterLL = iter->bucket_iter;
  void* temp;

  int result = LLIterGetPayload(iterLL, (void**)&temp);
  if (result == 0) {  // success
    HTKeyValuePtr tempKV = (HTKeyValuePtr)temp;
    dest->key = tempKV->key;
    dest->value = tempKV->value;
  }
  return result;
}

//  0 if there are no more elements.
int HTIteratorHasMore(HTIter iter) {
  if (iter->bucket_iter == NULL) {
    return 0;
  }

  if (LLIterHasNext(iter->bucket_iter) == 1)
    return 1;

  // No more in this iter; are there more buckets?
  int i = iter->which_bucket + 1;
  while (i < (iter->ht->num_buckets)) {
    // Make sure one of them has elements in it
    if ((iter->ht->buckets[i] != NULL) &&
        (NumElementsInLinkedList(iter->ht->buckets[i]) > 0)) {
      return 1;
    }
    i++;
  }

  return 0;
}
