#include <stdio.h>
#include <stdlib.h>
#include "myalloc.h"

/* change me to 1 for more debugging information
 * change me to 0 for time testing and to clear your mind
 */
#define DEBUG 1

void *__heap = NULL;
node_t *__head = NULL;


inline header_t *get_header(void *ptr)
{
   return (header_t *)(ptr - sizeof(header_t));
}

inline void print_header(header_t *header)
{
   printf("[header_t @ %p | buffer @ %p size: %lu magic: %08lx]\n",
          header,
          ((void *)header + sizeof(header_t)),
          header->size,
          header->magic);
}

inline void print_node(node_t *node)
{
   printf("[node @ %p | free region @ %p size: %lu next: %p]\n",
          node,
          ((void *)node + sizeof(node_t)),
          node->size,
          node->next);
}

void print_freelist_from(node_t *node)
{
   printf("\nPrinting freelist from %p\n", node);
   while (node != NULL)
   {
      printf("Point of NO return NODE: %d NODE->NEXT: %d \n", node, node->next);
      print_node(node);
      node = node->next;
   }
}

inline void coalesce_freelist(node_t *listhead)
{
//<<<<<<< HEAD
//	/* coalesce all neighboring free regions in the free list */
//
//	if (DEBUG) printf("In coalesce freelist...\n");
//	node_t *target = listhead;
//	node_t *node = target->next;
//	node_t *prev = target;
//    node_t *newHead = NULL;
//
//    while (node != NULL) {
//        
//    }
//	for (node_t *target = __head; target != NULL;) {
//           int *neighbor = (int*)target + sizeof(header_t) +
//           target->size;
//           if (neighbor == (int*)target->next) {
//              node_t *node_next = target->next->next;
//              target->size += sizeof(header_t) + target->next->size;
//              target->next = node_next;
//              //And now retry it since target can be coalesced with it's neighbor
//              continue;
//           } else {
//              target = target->next;
//           }
//        }
//           /* traverse the free list, coalescing neighboring regions!
//	 * some hints:
//	 * --> it might be easier if you sort the free list first!
//	 * --> it might require multiple passes over the free list!
//	 * --> it might be easier if you call some helper functions from here
//	 * --> see print_free_list_from for basic code for traversing a
//	 *     linked list!
//	 */
//=======
   /* coalesce all neighboring free regions in the free list */

   if (DEBUG) printf("In coalesce freelist...\n");

   node_t *start = listhead;
   node_t *next = start->next;
   int list_size = 0;
   perror("Start counting");

   while(start != NULL){
      ++list_size;
      start = start->next;
   }
   int* sort_list[list_size];
   int* shortest;
   int* previous_shortest = 0;

   perror("starting for loop");
   for(int i = 0; i < list_size; ++i){
      start = listhead;
      while(start->next != NULL ){
         if(start < start->next && start > previous_shortest){
            shortest = start;
         }
         else{
            shortest = start->next;
         }
         start = start->next;
      }
      sort_list[i] = shortest;
      previous_shortest = shortest;
   }

   perror("End for loop");
   int j = 0 ;

   while( j < list_size){
      printf("%p \n", sort_list[j]);
      j++;
   }
   
   while(0<1){
   }

   node_t *target = listhead;
   node_t *node = target->next;
   node_t *prev = target;
   node_t *newHead = NULL;

   //while (node != NULL) {

      //}
      for (node_t *target = __head; target != NULL;) {
         perror("Inside for loop");
      int *neighbor = (int*)target + sizeof(header_t) +
         target->size;
      printf("neighbor: %p && next: %p\n", neighbor, (int*)target->next);
      if (neighbor == (int*)target->next) {
         perror("Inside if");
         node_t *node_next = target->next->next;
         target->size += sizeof(header_t) + target->next->size;
         target->next = node_next;
         //And now retry it since target can be coalesced with it's  neighbor
      continue;
      } else {
         target = target->next;
      }
   }
   /* traverse the free list, coalescing neighboring regions!
    * some hints:
    * --> it might be easier if you sort the free list first!
    * --> it might require multiple passes over the free list!
    * --> it might be easier if you call some helper functions from
   here
   * --> see print_free_list_from for basic code for traversing a
   *     linked list!
   */
}

void destroy_heap()
{
   /* after calling this the heap and free list will be wiped
    * and you can make new allocations and frees on a "blank slate"
    */
   free(__heap);
   __heap = NULL;
   __head = NULL;
}

/* In reality, the kernel or memory allocator sets up the initial
 * heap. But in
 * our memory allocator, we have to allocate our heap manually, using
 * malloc().
 * YOU MUST NOT ADD MALLOC CALLS TO YOUR FINAL PROGRAM!
 */
inline void init_heap()
{
   /* FOR OFFICE USE ONLY */

   if ((__heap = malloc(HEAPSIZE)) == NULL)
   {
      printf("Couldn't initialize heap!\n");
      exit(1);
   }

   __head = (node_t*)__heap;
   __head->size = HEAPSIZE - sizeof(header_t);
   __head->next = NULL;

   if (DEBUG) printf("heap: %p\n", __heap);
   if (DEBUG) print_node(__head);

}

void *first_fit(size_t req_size)
{
//	void *ptr = NULL; /* pointer to the match that we'll return */
//
//	if (DEBUG) printf("In first_fit with size: %u and freelist @ %p\n",
//					  (unsigned)req_size, __head);
//
//	node_t *listitem = __head; /* cursor into our linked list */
//	node_t *prev = NULL; /* if listitem is __head, then prev must be null */
//	header_t *alloc = NULL; /* a pointer to a header you can use for your allocation */
//    char *a;
//
//	/* traverse the free list from __head! when you encounter a region that
//	 * is large enough to hold the buffer and required header, use it!
//	 * If the region is larger than you need, split the buffer into two
//	 * regions: first, the region that you allocate and second, a new (smaller)
//	 * free region that goes on the free list in the same spot as the old free
//	 * list node_t.
//	 *
//	 * If you traverse the whole list and can't find space, return a null
//	 * pointer! :(
//	 *
//	 * Hints:
//	 * --> see print_freelist_from to see how to traverse a linked list
//	 * --> remember to keep track of the previous free region (prev) so
//	 *     that, when you divide a free region, you can splice the linked
//	 *     list together (you'll either use an entire free region, so you
//	 *     point prev to what used to be next, or you'll create a new
//	 *     (smaller) free region, which should have the same prev and the next
//	 *     of the old region.
//	 * --> If you divide a region, remember to update prev's next pointer!
//	 */
//        if (req_size == 0) {
//           return NULL;
//        } else {
////            printf("REQ SIZE : %lu", req_size);
//            size_t sizeToAlloc = req_size + (size_t) sizeof(header_t);
//            int count = 0;
//            void *newtempAddress = NULL;
//            size_t tempSize;
//            while (listitem != NULL) {
//                newtempAddress = listitem;
//                tempSize = listitem->size;
//                count += 1;
//                if (listitem->size - sizeof(header_t) >= sizeToAlloc) {
////                    printf("INSIDE FIRST IF");
//                    alloc = newtempAddress;
//                    alloc->size = req_size;
//                    alloc->magic = HEAPMAGIC;
//                    printf("PRINTINGHEADER");
//                    print_header(alloc);
//                    printf("PRINTING HEADPOINTER");
//                    print_node(__head);
//
//                    ptr = (void *) ((long unsigned)alloc + sizeof(header_t));
//                    printf("PRINTING PTR %p/n", ptr);
//                    break;
//                } else {
//                    prev = listitem;
//                    listitem = listitem->next;
//                }
//            }
//            if (tempSize > sizeToAlloc) {
//                //splitting
////                printf("INSIDE SPLITTING");
//                node_t *newFree = (newtempAddress + 2 * sizeof(header_t) + alloc->size +sizeof(a));
//                if (prev != NULL) {
//                    prev->next = newFree;
//                } else {
//                    prev = newFree;
//                }
//                newFree->size = (long unsigned)(tempSize - sizeToAlloc - sizeof(header_t));
//                printf("NEWFREESIZE: %lu\n", newFree->size);
//                newFree->next = listitem->next;
//                __head = (newFree - sizeof(header_t));
////                printf("NEWFREESIZE: %lu\n", newFree->size);
//                __head->size = newFree->size;
//                //printf("NEWHEADSIZE: %lu\n", __head->size);
//                __head->next = NULL;
//            } else if (tempSize == sizeToAlloc) {
//                // perfect match and no split
//                printf("INSIDE NOT SPLITTING");
//                prev->next = listitem->next;
//                __head = listitem->next;
//                __head->size = __head->size - (long unsigned) (sizeToAlloc);
//                __head->next->next = listitem->next->next;
//            }
//            //return ptr;
//        }
//    printf("PRINTINGHEADER");
//    print_header(alloc);
//    printf("PRINTING HEADPOINTER");
//    print_node(__head);
//	if (DEBUG) printf("Returning pointer: %p\n", ptr);
//
//                 return ptr;
//=======
   void *ptr = NULL; /* pointer to the match that we'll return */

   if (DEBUG) printf("In first_fit with size: %u and freelist @ %p\n",
                     (unsigned)req_size, __head);

   node_t *listitem = __head; /* cursor into our linked list */
   node_t *prev = NULL; /* if listitem is __head, then prev must be
                           null */
   header_t *alloc = NULL; /* a pointer to a header you can use for
                              your allocation */
   char *a;

   /* traverse the free list from __head! when you encounter a region
   that
   * is large  enough to hold the buffer and required header, use it!
   * If the region is larger than you need, split the buffer into two
   * regions: first, the region that you allocate and second, a new
   (smaller)
   * free region that goes on the free list in the same spot as the
   old free
   * list node_t.
   *
   * If you traverse the whole list and can't find space, return a
   * null
   * pointer! :(
   *
   * Hints:
   * --> see print_freelist_from to see how to traverse a linked list
   * --> remember to keep track of the previous free region (prev) so
   *     that, when you divide a free region, you can splice the
   * linked
   *     list together (you'll either use an entire free region, so
   * you
   *     point prev to what used to be next, or you'll create a new
   *     (smaller) free region, which should have the same prev and
   * the next
   *     of the old region.
   * --> If you divide a region, remember to update prev's next
   * pointer!
   */
   if (req_size <= 0) {
      return NULL;
   } else {
                 printf("REQ SIZE : %lu \n", req_size);
      size_t sizeToAlloc = req_size  + (size_t) sizeof(header_t);
      int count = 0;
      void *newtempAddress = NULL;
      size_t tempSize;
      perror("Before while");
      while (listitem != NULL) {
         newtempAddress = listitem;
         tempSize = listitem->size;
         count += 1;
         perror("Before IF");
         if (listitem->size - sizeof(header_t) >= sizeToAlloc) {
                                printf("INSIDE FIRST IF \n");
            alloc = newtempAddress;
            alloc->size = req_size;
            alloc->magic = HEAPMAGIC;
            printf("PRINTING HEADER");
            print_header(alloc);
            printf("PRINTING HEADPOINTER");
            print_node(__head);

            /*CHANGED adding the size of a header makes the magic
              number correct but creates a sys dump */
            ptr = (void *) ((long unsigned)alloc + sizeof(header_t));
            printf("PRINTING PTR %p\n", ptr);

            header_t* t = (header_t *)(ptr - sizeof(header_t));
            printf("PTR IN FIRST FIT: %08lx\n", t->magic);
            printf("PTR SIZE in FIRST FIT: %lu\n", t->size);
            break;
         } else {
            perror("Get the next element cause no space");
            prev = listitem;
            listitem = listitem->next;
         }
      }
      perror("Out of the While loop");
      if (tempSize > sizeToAlloc) {
         //splitting
         printf("INSIDE SPLITTING \n");
         node_t *newFree = (newtempAddress + 2 * sizeof(header_t) +
                            alloc->size + sizeof(a));
         if (prev != NULL) {
            prev->next = newFree;
         } else {
            prev = newFree;
         }
         //CHANGED potential size conversion error??
         newFree->size = (long unsigned)(tempSize - sizeToAlloc); //- sizeof(header_t));
         printf("NEW FREE SIZE: %lu\n", newFree->size);
         printf("SIZE OF TEMP: %ld -- SIZE TO ALLOC: %ld -- SIZE OF HEAD: %ld \n", tempSize, sizeToAlloc, sizeof(header_t));
         newFree->next = listitem->next;
         __head = (newFree);
      __head->size = newFree->size;
      //printf("NEWHEADSIZE: %lu\n", __head->size);
      __head->next = NULL;
   } else if (tempSize == sizeToAlloc) {
      // perfect match and no split
      printf("INSIDE NOT SPLITTING");
      prev->next = listitem->next;
      __head = listitem->next;
      __head->size = __head->size - (long unsigned) (sizeToAlloc);
      __head->next->next = listitem->next->next;
   }
      else{
         perror("NO IF NO ELSE IF");
      }
   //return ptr;
}
   perror("LETS PRINT THIS ALLOC eh?");
printf("PRINTING HEADER");
if(alloc != NULL){
   print_header(alloc);
}
perror("LETS PRINT THIS HEAD PTR eh?");
printf("PRINTING HEADPOINTER");
print_node(__head);
if (DEBUG) printf("Returning pointer: %p\n", ptr);

if(ptr != NULL){
   header_t* t = (header_t *)(ptr - sizeof(header_t));
   printf("PRINTING RETURNED POINTER MAGIC: %08lx \n", t->magic);
   printf("PRINTING RETURNED POINTER SIZE: %ld \n", t->size);
}
return ptr;
}

/* myalloc returns a void pointer to size bytes or NULL if it can't.
 * myalloc will check the free regions in the free list, which is
 * pointed to by
 * the pointer __head.
 */

void *myalloc(size_t size)
{
   if (DEBUG) printf("\nIn myalloc:\n");
   void *ptr = NULL;

   /* initialize the heap if it hasn't been */
   if (__heap == NULL) {
      if (DEBUG) printf("*** Heap is NULL: Initializing ***\n");
      init_heap();
   }

   /* perform allocation */
   if (size == 0) {
      return NULL;
   }

   /* search __head for first fit */
   if (DEBUG) printf("Going to do allocation.\n");

   ptr = first_fit(size); /* all the work really happens in first_fit
   */

   if (DEBUG) printf("__head is now @ %p\n", __head);

   return ptr;

}

/* myfree takes in a pointer _that was allocated by myfree_ and
 * deallocates it,
 * returning it to the free list (__head) like free(), myfree()
 * returns
 * nothing.  If a user tries to myfree() a buffer that was already
 * freed, was
 * allocated by malloc(), or basically any other use, the behavior is
 * undefined.
 */
void myfree(void *ptr)
{

   if (DEBUG) printf("\nIn myfree with pointer %p\n", ptr);

   header_t *header = get_header(ptr); /* get the start of a header
                                          from a pointer */

   if (DEBUG) { print_header(header); }

   if (header->magic != HEAPMAGIC) {
      printf("Header is missing its magic number!!\n");
      printf("It should be '%08lx'\n", HEAPMAGIC);
      printf("But it is '%08lx'\n", header->magic);
      printf("The heap is corrupt!\n");
      return;
   }

   /* free the buffer pointed to by ptr!
    * To do this, save the location of the old head (hint, it's
   __head).
   * Then, change the allocation header_t to a node_t. Point __head
   * at the new node_t and update the new head's next to point to the
   * old head. Voila! You've just turned an allocated buffer into a
   * free region!
   */

   /* save the current __head of the freelist */
   /* ??? */
   node_t *oldhead = __head;
   //        node_t *deallocated = header + sizeof(header_t);
   //        deallocated->size = header->size;

   /* now set the __head to point to the header_t for the buffer being
      freed */
   /* ??? */
   __head = (node_t*) header;
   //__head->size = oldhead->size + sizeof(header_t) + header->size;
   /* set the new head's next to point to the old head that you saved
      */
   /* ??? */
   __head->next = oldhead;
   //    __head->next = deallocated;
   //    deallocated->next = oldhead;
   /* PROFIT!!! */

}
