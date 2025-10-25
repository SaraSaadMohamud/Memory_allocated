# Simple Heap Allocator in C

## Overview

This project implements a basic **heap memory allocator** in C, providing custom versions of `malloc` and `free` functions (`HeapMalloc` and `HeapFree`). It manages a fixed-size heap (640 KB) using two lists of memory chunks: one for allocated blocks and one for freed blocks. The allocator uses a **first-fit** strategy to allocate memory from free chunks and supports splitting large free chunks when necessary. However, it does not yet implement garbage collection or merging of adjacent free chunks (see [Limitations](#limitations)).

The code is educational, demonstrating core concepts of dynamic memory management without relying on the standard library's allocator. It includes debugging utilities like chunk list dumping and assertions for safety.

### Key Components
- **Heap**: A static array `heap[HEAP_CAPACITY]` (640,000 bytes) initialized to zero.
- **Chunk Structure**: `Heap_Chunk` holds a `void* start` (pointer to the chunk's start) and `size_t size` (length in bytes).
- **Chunk Lists**: `Chunk_List` manages arrays of up to 1,024 chunks (`CHUNK_LIST_CAP`), with a `count` for the active number of chunks.
  - `alloced_chunk`: Tracks allocated memory blocks.
  - `freed_chunk`: Tracks available free blocks (initially the entire heap).
- **UNIMPLEMENTED Macro**: A debug helper that aborts with a message if an unimplemented feature is called.

## How It Works

### Data Structures
- **Heap Initialization**: The heap starts as a single free chunk covering the entire 640 KB array. `heap_size` tracks the used heap size (though it's not actively used in this implementation).
- **Chunk Lists**: Both allocated and free chunks are stored in sorted arrays (by start address) for efficient searching and insertion. Insertions use a simple insertion sort to maintain order.

### Memory Allocation (`HeapMalloc(size)`)
1. If `size == 0`, return `NULL`.
2. Iterate through `freed_chunk` in order (first-fit).
3. Find the first free chunk where `size >= requested size`.
4. Remove the free chunk from `freed_chunk`.
5. Allocate the requested `size` from the start of the chunk and insert it into `alloced_chunk`.
6. If there's a remainder (`tail_size > 0`), insert the tail as a new free chunk immediately after the allocated portion.
7. Return the start pointer of the allocated chunk.
8. If no suitable free chunk is found, return `NULL` (allocation failure).

### Memory Deallocation (`HeapFree(ptr)`)
1. If `ptr == NULL`, do nothing.
2. Search `alloced_chunk` backward (from newest to oldest) for a matching start pointer using `ChunkListFind`.
3. Assert the chunk exists (panics on failure).
4. Insert the entire chunk (start and size) into `freed_chunk`.
5. Remove it from `alloced_chunk`.

**Note**: Freed chunks are not merged with adjacent free chunks, which can lead to fragmentation.

### Utility Functions
- **`ChunkListDump(list)`**: Prints the chunks in a list, showing start addresses and sizes. Used for debugging.
- **`ChunkListFind(list, ptr)`**: Linear search (backward) for a chunk by start pointer. Returns index or -1.
- **`ChunkListRemove(list, index)`**: Shifts elements to remove a chunk by index, decrementing count.
- **`ChunkListInsert(list, ptr, size)`**: Appends the new chunk, then insertion-sorts the list by start address to keep it ordered.
- **`HeapCollect()`**: Placeholder for garbage collection (currently empty; marked as TODO).

### Insertion Sort in `ChunkListInsert`
The list is kept sorted by chunk start address. After appending a new chunk, it bubbles the new element leftward if it's out of order. This ensures `ChunkListFind` can rely on positional hints (though it's still linear-time).
## Flowchart
 <img width="1024" height="1536" alt="image" src="https://github.com/user-attachments/assets/97fd6c9d-55f3-49d1-9321-a006065fed24" />

## Future Improvements
- Implement `HeapCollect` for merging adjacent free chunks (sort frees and scan for contiguous blocks).
- Add alignment and padding support.
- Use a better data structure (e.g., red-black tree) for chunks to enable logarithmic operations.
- Error handling: Return errors instead of `NULL`/assert.
- Tests: Add unit tests for edge cases (e.g., max chunks, fragmentation).

## License
This code is provided as-is for educational purposes. Feel free to fork and improve! (No formal license specified.)
