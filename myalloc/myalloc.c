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
      print_node(node);
      node = node->next;
   }
}

inline void coalesce_freelist(node_t *listhead)
{
   /* coalesce all neighboring free regions in the free list */

   if (DEBUG) printf("In coalesce freelist...\n");

   node_t *start = listhead;
   node_t *next = start->next;
   int list_size = 0;

   //count the list
   while(start != NULL){
      ++list_size;
      start = start->next;
   }
   
   node_t* sort_list[list_size];
   

   start = listhead;
   printf("Printing neighbors:\n");
   while(start!= NULL){
      print_node(start);
      print_header(start);
         start = start->next;
   }


   //reset the list
   start = listhead;
   //write the values into sort_list in any order
   for(int z = 0; z < list_size; z++){
      sort_list[z] = start;
      start = start->next;
   }
   printf("******************************\n");
   printf("Sorting by neighboring regions\n");
      start = listhead;

      //REFERENCE: http://www.sanfoundry.com/c-program-sort-array-ascending-order/
      for (int i = 0; i < list_size; ++i)
      {
         for (int j = i + 1; j < list_size; ++j)
         {
            if (sort_list[i]->next > sort_list[j]->next)
            {
               node_t *a =  sort_list[i];
               sort_list[i] = sort_list[j];
               sort_list[j] = a;
            }
         }
      }
      printf("SORTED LIST:\n");
      int j = 0;
   while( j < list_size){
      printf("%p \n", sort_list[j]);
      j++;
   }
   printf("******************************\n");
   
   printf("Coalescing\n");
   
   node_t *head = listhead;
   node_t *target = target->next;
   //node_t *prev = target;
   //node_t *newHead = NULL;
   
   for(int i = 0; i < list_size; ++i){
      print_node(sort_list[i]);
      //can be coalesced
      if(sort_list[i+1]->next!=NULL && sort_list[i]->next!=NULL){//eat the next neighbor
         head = sort_list[i];
         target = sort_list[i+1];
         printf("Head: %p  Eats: %p \n", head,(int*)target);
         head->size += target->size + sizeof(header_t);
         head->next = target->next;
      }
   }

      printf("******************************\n");

      printf("Printing neighbors after coalescing:\n");
      while(start!= NULL ){
         print_node(start);
         print_header(start);
         start = start->next;
      }
      printf("******************************\n");
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
   void *ptr = NULL; /* pointer to the match that we'll return */
   node_t *listitem = __head; /* cursor into our linked list */
   node_t *prev = NULL; /* if listitem is __head, then prev must be
                           null */
   header_t *alloc = NULL; /* a pointer to a header you can use for
                              your allocation */
    printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
   printf("Printing FREELIST at beginning of Myalloc\n");
   node_t *start = listitem;
   while(start!= NULL){
      print_node(start);
      print_header(start);
      start = start->next;
   }

   //allocate
   if (req_size <= 0) {
      return NULL;
   } else {
      //            printf("REQ SIZE : %lu \n", req_size);
      size_t sizeToAlloc = req_size  + (size_t) sizeof(header_t);
      int count = 0;
      void *newtempAddress = NULL;
      size_t tempSize;
      while (listitem != NULL) {
         newtempAddress = listitem;
         tempSize = listitem->size;
         count += 1;
         if (listitem->size >= sizeToAlloc) {
             if (tempSize > sizeToAlloc) {
                 printf("INSIDE SPLITTING \n");

                 //?? netempaddress messing up??
                 node_t *newFree = ((void*)newtempAddress + sizeToAlloc);

                 //CHANGED potential size conversion error??
                 newFree->size = (long unsigned)(tempSize - sizeToAlloc); //- sizeof(header_t));
                 printf("NEW FREE SIZE: %lu\n", newFree->size);
                 printf("SIZE OF TEMP: %ld -- SIZE TO ALLOC: %ld -- SIZE OF HEAD: %ld \n", tempSize, sizeToAlloc, sizeof(header_t));

                 //??
                 if (prev != NULL) {
                     printf("**************LJK");
                     prev->next = newFree;
                     newFree->next = listitem->next;
                 } else {
                     __head = (newFree);
                     newFree->next = listitem->next;
                 }
             } else if (tempSize == sizeToAlloc) {
                 // perfect match and no split
                 printf("INSIDE NOT SPLITTING");
                 //?? why always split at the head?
                 prev->next = listitem->next;
                 __head = listitem->next;
                 __head->size = __head->size - (long unsigned) (sizeToAlloc);
                 __head->next->next = listitem->next->next;
             }
             else{
                 printf("NO FIT FOR THIS ALLOCATION!");
             }
            alloc = newtempAddress;
            alloc->size = req_size;
            alloc->magic = HEAPMAGIC;
            printf("PRINTING ALLOC HEADER\n");
            print_header(alloc);
            printf("PRINTING ALLOC\n");
            print_node(alloc);

            printf("Setting PTR to ALLOC\n");
            ptr = (void *)alloc + sizeof(header_t);

            //ALLOC is not setting the pointer to the correct header
            //testing
            printf("***************************************\n");
            printf("PRINTING THE RETURNING PTR %p\n", ptr -16);
            //header_t* t = ((header_t *)(ptr-16) );
            header_t* t = (header_t *)(ptr - sizeof(header_t));
            print_header(t);
            print_node(t);
            printf("HEADER MAGIC: %08lx HEAPMAGIC: %08lx\n",t->magic, HEAPMAGIC);
            //t->magic  = HEAPMAGIC;
            //t->size = sizeof(header_t);

            printf("Printing HEAD right before returning\n");
            node_t *fart = __head;
            while(fart!= NULL){
               print_node(fart);
               fart = fart->next;
            }
            printf("RETURNING PTR!\n\n");
             return ptr;
         } else {
            perror("Get the next element cause no space");
            prev = listitem;
            listitem = listitem->next;
         }
      }
      
      //splitting
//      if (tempSize > sizeToAlloc) {
//         printf("INSIDE SPLITTING \n");
//
//         //?? netempaddress messing up??
//         node_t *newFree = (newtempAddress + sizeof(header_t) +
//                            alloc->size + sizeof(a));
//
//         //CHANGED potential size conversion error??
//         newFree->size = (long unsigned)(tempSize - sizeToAlloc); //- sizeof(header_t));
//         printf("NEW FREE SIZE: %lu\n", newFree->size);
//         printf("SIZE OF TEMP: %ld -- SIZE TO ALLOC: %ld -- SIZE OF HEAD: %ld \n", tempSize, sizeToAlloc, sizeof(header_t));
//
//         //??
//          if (prev != NULL) {
//              printf("**************LJK");
//              prev->next = newFree;
//              newFree->next = listitem->next;
//          } else {
//              __head = (newFree);
//              newFree->next = listitem;
//          }
//
//         __head->size = newFree->size;
//      //printf("NEWHEADSIZE: %lu\n", __head->size);
//         if(__head != NULL){
//            printf("YYXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
//            //Test here
//         }
//         __head->next = NULL;
//   } else if (tempSize == sizeToAlloc) {
//      // perfect match and no split
//      printf("INSIDE NOT SPLITTING");
//      //?? why always split at the head?
//      prev->next = listitem->next;
//      __head = listitem->next;
//      __head->size = __head->size - (long unsigned) (sizeToAlloc);
//      __head->next->next = listitem->next->next;
//   }
//      else{
//         printf("NO FIT FOR THIS ALLOCATION!");
//      }
}


if (DEBUG) printf("Returning pointer: %p\n", ptr);

//if(ptr != NULL){
//
//   //?? TODO: pointer not being set correctly?
//   header_t* t = (header_t *)(ptr - sizeof(header_t));
//   node_t * rtn_node = (node_t *) (ptr-16); 
//   printf("PRINTING RETURNED POINTER\n");
//   print_header(t);
//   print_node(rtn_node);
//}
//
//
//printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
//printf("Printing FREELIST at ending of Myalloc\n");
//node_t *end = __head;
//while(end!= NULL){
//   print_node(end);
//   print_header(end);
//   end = end->next;
//
//}


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


   
   printf("\n$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
   printf("HEAD:");
   print_node(__head);
   printf(" HEAD->NEXT");
   print_node(__head->next);
   
}
