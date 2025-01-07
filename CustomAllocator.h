// CustomAllocator.h
#ifndef CUSTOM_ALLOCATOR_H
#define CUSTOM_ALLOCATOR_H

#include <cstddef>
#include <iostream>

class CustomAllocator {
private:
    struct Block {
        size_t size;       // Size of the block
        bool isFree;       // Block status (free or allocated)
        Block* next;       // Pointer to the next block
    };

    void* memoryPool;       // Pointer to the beginning of the memory pool
    Block* freeList;        // Pointer to the free list (linked list of blocks)
    size_t totalSize;       // Total size of the memory pool
    size_t peakUsage;       // Peak memory usage
    size_t currentUsage;    // Current memory usage

public:
    // Constructor: Initializes the memory pool and sets up the free list
    CustomAllocator(size_t poolSize);

    // Destructor: Frees the memory pool
    ~CustomAllocator();

    // Allocates memory of the requested size from the pool
    void* allocate(size_t size);

    // Frees a previously allocated memory block
    void deallocate(void* ptr);

    // Displays memory usage statistics
    void printStats() const;

private:
    // Splits a free block if it's larger than the requested size
    void splitBlock(Block* block, size_t size);

    // Merges adjacent free blocks to reduce fragmentation
    void mergeFreeBlocks();
};

#endif

