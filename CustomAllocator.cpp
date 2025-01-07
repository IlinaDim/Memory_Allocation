// CustomAllocator.cpp
#include "CustomAllocator.h"
#include <cstring> // For memset

// Constructor: Initializes the memory pool and sets up the free list
CustomAllocator::CustomAllocator(size_t poolSize) {
    totalSize = poolSize;
    peakUsage = 0;
    currentUsage = 0;

    // Allocate memory for the pool
    memoryPool = std::malloc(poolSize);
    if (!memoryPool) {
        throw std::bad_alloc();
    }

    // Initialize the free list with a single block
    freeList = static_cast<Block*>(memoryPool);
    freeList->size = poolSize - sizeof(Block);
    freeList->isFree = true;
    freeList->next = nullptr;
}

// Destructor: Frees the memory pool
CustomAllocator::~CustomAllocator() {
    std::free(memoryPool);
}

// Allocates memory of the requested size from the pool
void* CustomAllocator::allocate(size_t size) {
    Block* current = freeList;
    Block* prev = nullptr;

    // First-fit strategy: Find the first free block large enough
    while (current) {
        if (current->isFree && current->size >= size) {
            // Split the block if it's larger than the requested size
            splitBlock(current, size);

            // Mark the block as allocated
            current->isFree = false;
            currentUsage += current->size;
            if (currentUsage > peakUsage) {
                peakUsage = currentUsage;
            }

            // Return a pointer to the usable memory (after the Block metadata)
            return reinterpret_cast<void*>(current + 1);
        }

        prev = current;
        current = current->next;
    }

    // No suitable block found
    throw std::bad_alloc();
}

// Frees a previously allocated memory block
void CustomAllocator::deallocate(void* ptr) {
    if (!ptr) return;

    // Get the block metadata from the pointer
    Block* block = static_cast<Block*>(ptr) - 1;

    // Mark the block as free
    block->isFree = true;
    currentUsage -= block->size;

    // Merge adjacent free blocks to reduce fragmentation
    mergeFreeBlocks();
}

// Displays memory usage statistics
void CustomAllocator::printStats() const {
    std::cout << "Total Memory: " << totalSize << " bytes\n";
    std::cout << "Current Usage: " << currentUsage << " bytes\n";
    std::cout << "Peak Usage: " << peakUsage << " bytes\n";

    // Calculate fragmentation percentage
    size_t largestFreeBlock = 0;
    Block* current = freeList;
    while (current) {
        if (current->isFree && current->size > largestFreeBlock) {
            largestFreeBlock = current->size;
        }
        current = current->next;
    }
    size_t fragmentation = (largestFreeBlock > 0) ? (totalSize - largestFreeBlock - currentUsage) * 100 / totalSize : 0;
    std::cout << "Fragmentation: " << fragmentation << "%\n";
}

// Splits a free block if it's larger than the requested size
void CustomAllocator::splitBlock(Block* block, size_t size) {
    if (block->size > size + sizeof(Block)) {
        // Create a new block for the remaining memory
        Block* newBlock = reinterpret_cast<Block*>(reinterpret_cast<char*>(block) + sizeof(Block) + size);
        newBlock->size = block->size - size - sizeof(Block);
        newBlock->isFree = true;
        newBlock->next = block->next;

        // Update the current block
        block->size = size;
        block->next = newBlock;
    }
}

// Merges adjacent free blocks to reduce fragmentation
void CustomAllocator::mergeFreeBlocks() {
    Block* current = freeList;

    while (current && current->next) {
        if (current->isFree && current->next->isFree) {
            // Merge the current block with the next block
            current->size += current->next->size + sizeof(Block);
            current->next = current->next->next;
        } else {
            current = current->next;
        }
    }
}

