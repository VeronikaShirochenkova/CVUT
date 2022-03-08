#include "mem_alloc.h"
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>

static inline void *nbrk(void *address);

static inline unsigned syscall2(unsigned w0, unsigned w1)
{
    asm volatile("   mov %%esp, %%ecx    ;"
                 "   mov $1f, %%edx      ;"
                 "   sysenter            ;"
                 "1:                     ;"
                 : "+a"(w0)
                 : "S"(w1)
                 : "ecx", "edx", "memory");
    return w0;
}

static void *nbrk(void *address)
{
    return (void *)syscall2(3, (unsigned)address);
}

#ifndef NULL
#define NULL (void *)0
#endif


typedef struct header {
	size_t size;
	bool is_free;
    struct header *prev;
	struct header *next;
}header_t;

header_t *head = NULL;
header_t *tail = NULL;
void *start_brk;


header_t *get_free_block(size_t size) {

    header_t *current = head;

    while (current) {
        if (current->is_free && current->size >= size) {
            if (current->size == size) {
                return current;
            }

            size_t free_size = current->size - size;
            if (free_size >= (2*sizeof(header_t))) {
                header_t *new;
                new=(header_t*)((void*)(current + 1) + size);
                new->size = free_size - sizeof(header_t);
                new->is_free = true;
                current->size = size;

                if (current == tail) {
                    tail = new;
                    new->next = NULL; 
                }
                else {
                    new->next = current->next; 
                    new->next->prev = new;  
                }
                
                current->next = new;
                new->prev = current;
                return current;
            }
        }
        current = current->next;
    }

    return NULL;
}

void *my_malloc(unsigned long size) {

    size_t total_size;
    header_t *header;
    void *block;

    // if request size is less then zero
    if (size <= 0) { return NULL; }

    // if it's first call of my_malloc
    if (start_brk == NULL) { start_brk = nbrk(0); }

    // check if exist free block of memory of suitable size
    header = get_free_block(size);
    if (header) {
        header->is_free = false;
        return (void*)(header + 1);
    }

    // get total size which need to allocate
    total_size = sizeof(header_t) + size;
    void* start_point = nbrk(0); 

    block = nbrk(start_point + total_size);
    if (block == NULL) { return NULL; }

    // create new block
    header = block;
    header->size = size;
    header->is_free = false;
    header->next = NULL;
    if (tail != NULL) {
        tail->next = header;
        header->prev = tail;  
        tail = header;
    }
    else {
        head = header;
        tail = header;
        header->prev = NULL;
    }
    return (void*)(header + 1);
}

int my_free(void *address)
{
    void *curr_brk;
    header_t *header;
    void *next_brk;

    // if my_free call early than my_malloc
    if (start_brk == NULL) { start_brk = nbrk(0); }

    // invalid address
	curr_brk = nbrk(0);
    if(address >= curr_brk || address < start_brk) {    
        return 1;
    }

    header = (header_t*)address - 1;

    if (curr_brk == ((char*)address + header->size)) {           // if this block is last in queue

        next_brk = curr_brk - (sizeof(header_t) + header->size); // count next point for brk
        if (header->prev != NULL) {
            header->prev->next = NULL;                          // delete pointer to this block
            tail = header->prev; 
        }
        else {
            head = NULL;
            tail = NULL;
        }
        
        nbrk(next_brk);
        return 0;
    }
    else {
        header->is_free = true;
        header_t *next_block = header->next;
        header_t *prev_block = header->prev;
        // if next block is free too
        if (next_block && next_block->is_free) {
            if (tail == next_block) {
                tail = header;
                header->next = NULL;
            }
            else {
                header->next = next_block->next;
                next_block->next->prev = header;
            }
            
            header->size += sizeof(header_t) + next_block->size;
        }
        // if previous block is free too
        if (prev_block && prev_block->is_free) {
            if (tail == header) {
                tail = prev_block;
                prev_block->next = NULL;
            }
            else {
                prev_block->next = header->next;
                header->next->prev = prev_block;
            }
            
            prev_block->size += sizeof(header_t) + header->size;
        }

    }
    return 0;
}