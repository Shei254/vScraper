#pragma once
#include "SMemoryChunk.h"

class CMemoryPool
{
public:
	CMemoryPool(const std::size_t& initialMemoryPoolSize, const std::size_t& memoryChunkSize, const std::size_t& mimalMemorySizeToAllocate, bool setMemoryData);
	void* getMemory(const std::size_t& memorySize);
	void freeMemory(void* memoryBlock, const std::size_t& memoryBlockSize);
private:
	SMemoryChunk* _ptr_first_chunk;
	SMemoryChunk* _ptr_last_chunk;
	SMemoryChunk* _ptr_cursor_chunk;

	std::size_t _total_memory_pool_size;
	std::size_t _used_memory_pool_size;
	std::size_t _free_memory_pool_size;

	std::size_t _memory_chunk_size;
	std::size_t _ui_memory_chunk_count;
	std::size_t _ui_object_count;
	std::size_t _minimal_memory_size_to_allocate;

	bool _set_memory_data;

	bool allocateMemory(const std::size_t& memorySize);
	bool linkChunksToData(SMemoryChunk* memoryChunks, unsigned int& chunkCount, TBYTE* newMemoryBlock);
	SMemoryChunk* setChunkDefaults(SMemoryChunk* memoryChunk);
	bool recalcChunkMemorySize(SMemoryChunk* chunk, unsigned int chunkCount);

	std::size_t calculateMemoryBlockSize(const std::size_t& memorySize);
	unsigned int calculateNeededChunks(const std::size_t& memorySize);
	
	void setMemoryChunkValues(SMemoryChunk* memoryChunk, std::size_t blockSize);
	SMemoryChunk* findChunkSuitableToHoldMemory(std::size_t& memorySize);
	SMemoryChunk* findChunkHoldingPointerTo(void* memoryBlockPtr);

	void freeChunks(SMemoryChunk* chunk);
};

