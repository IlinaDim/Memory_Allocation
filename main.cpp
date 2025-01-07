// main.cpp
#include "CustomAllocator.h"
#include <iostream>
#include <vector>
#include <chrono> // For benchmarking

int main() {
    // Initialize the custom allocator with a pool size of 1 MB
    size_t poolSize = 1024 * 1024; // 1 MB
    CustomAllocator allocator(poolSize);

    // Display initial memory statistics
    std::cout << "Initial Memory Statistics:\n";
    allocator.printStats();

    // Allocate some memory blocks
    std::cout << "\nAllocating 256 bytes...\n";
    void* block1 = allocator.allocate(256);
    allocator.printStats();

    std::cout << "\nAllocating 512 bytes...\n";
    void* block2 = allocator.allocate(512);
    allocator.printStats();

    std::cout << "\nAllocating 128 bytes...\n";
    void* block3 = allocator.allocate(128);
    allocator.printStats();

    // Free one block and display statistics
    std::cout << "\nDeallocating 512-byte block...\n";
    allocator.deallocate(block2);
    allocator.printStats();

    // Allocate a new block to test splitting and merging
    std::cout << "\nAllocating 300 bytes...\n";
    void* block4 = allocator.allocate(300);
    allocator.printStats();

    // Free all blocks
    std::cout << "\nDeallocating all blocks...\n";
    allocator.deallocate(block1);
    allocator.deallocate(block3);
    allocator.deallocate(block4);
    allocator.printStats();

    // Benchmarking custom allocator vs malloc/free
    std::cout << "\nBenchmarking Custom Allocator vs malloc/free:\n";

    const int numAllocations = 10000;
    const size_t allocationSize = 64;

    // Benchmark Custom Allocator
    auto startCustom = std::chrono::high_resolution_clock::now();
    std::vector<void*> customBlocks;
    for (int i = 0; i < numAllocations; ++i) {
        customBlocks.push_back(allocator.allocate(allocationSize));
    }
    for (void* ptr : customBlocks) {
        allocator.deallocate(ptr);
    }
    auto endCustom = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> customDuration = endCustom - startCustom;
    std::cout << "Custom Allocator Time: " << customDuration.count() << " seconds\n";

    // Benchmark malloc/free
    auto startMalloc = std::chrono::high_resolution_clock::now();
    std::vector<void*> mallocBlocks;
    for (int i = 0; i < numAllocations; ++i) {
        mallocBlocks.push_back(std::malloc(allocationSize));
    }
    for (void* ptr : mallocBlocks) {
        std::free(ptr);
    }
    auto endMalloc = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> mallocDuration = endMalloc - startMalloc;
    std::cout << "malloc/free Time: " << mallocDuration.count() << " seconds\n";

    return 0;
}

