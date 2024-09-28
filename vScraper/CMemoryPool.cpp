#include <assert.h>
#include <math.h>
#include "CMemoryPool.h"

static const char FREEED_MEMORY_CONTENT = '\AA';

CMemoryPool::CMemoryPool(const std::size_t& initialMemoryPoolSize, const std::size_t& memoryChunkSize, const std::size_t& mimalMemorySizeToAllocate, bool setMemoryData)
{
	_ptr_first_chunk = nullptr;
	_ptr_last_chunk = nullptr;
	_ptr_cursor_chunk = nullptr;

	_total_memory_pool_size = 0;
	_used_memory_pool_size = 0;
	_free_memory_pool_size = 0;

	_memory_chunk_size = memoryChunkSize;
	_ui_memory_chunk_count = 0;

	_set_memory_data = setMemoryData;
	_minimal_memory_size_to_allocate = mimalMemorySizeToAllocate;

	allocateMemory(initialMemoryPoolSize);
}

void* CMemoryPool::getMemory(const std::size_t& memorySize)
{
	std::size_t bestMemBlockSize = calculateMemoryBlockSize(memorySize);
	SMemoryChunk* chunk = nullptr;

	while (!chunk) {
		chunk = findChunkSuitableToHoldMemory(bestMemBlockSize);
		if (!chunk) {
			//No Chunk Was Found Memory Pool Too Smalle
			bestMemBlockSize = max(bestMemBlockSize, calculateMemoryBlockSize(memorySize));
			allocateMemory(bestMemBlockSize);
		}
	}
	
	_used_memory_pool_size += bestMemBlockSize;
	_free_memory_pool_size -= bestMemBlockSize;
	_ui_object_count++;
	setMemoryChunkValues(chunk, bestMemBlockSize);

	return ((void*)chunk->data);
}

void CMemoryPool::freeMemory(void* memoryBlock, const std::size_t& memoryBlockSize)
{
	SMemoryChunk* chunk = findChunkHoldingPointerTo(memoryBlock);
	if (chunk) {
		freeChunks(chunk);
	}
	else {
		assert(false && "Error : Requested Pointer Not In Memory Pool");
	}

	assert(_ui_object_count > 0 && "Error: Request to delete more memory than allocated");
	_ui_object_count--;
}

void CMemoryPool::freeChunks(SMemoryChunk* chunk)
{
	SMemoryChunk* cursor = chunk;
	unsigned int uiChunkCount = calculateNeededChunks(cursor->usedSize);

	for (unsigned int i = 0; i < uiChunkCount; i++) {
		if (cursor) {
			if (_set_memory_data) {
				memset((void*)cursor->data, FREEED_MEMORY_CONTENT, _memory_chunk_size);
			}

			cursor->usedSize = 0;
		}

		_used_memory_pool_size -= _memory_chunk_size;
		cursor = cursor->next;
	}
}

bool CMemoryPool::allocateMemory(const std::size_t& memorySize)
{
	std::size_t bestMemSizeBlock = calculateMemoryBlockSize(memorySize);

	TBYTE* newMemoryBlock = (TBYTE*)malloc(bestMemSizeBlock);

	unsigned int neededChunks = calculateNeededChunks(memorySize);

	SMemoryChunk* newChunks = (SMemoryChunk*)malloc(neededChunks * sizeof(SMemoryChunk));
	
	assert(((newMemoryBlock) && (newChunks)) && "Error: System is out of memory");

	return linkChunksToData(newChunks, neededChunks, newMemoryBlock);
}

bool CMemoryPool::linkChunksToData(SMemoryChunk* memoryChunks, unsigned int& chunkCount, TBYTE* newMemoryBlock)
{
	SMemoryChunk* newChunk = nullptr;
	unsigned int memoryOffset = 0;
	bool allocationChunkAssigned = false;

	for (int i = 0; i < chunkCount; i++) {
		if (!_ptr_first_chunk) {
			_ptr_first_chunk = setChunkDefaults(&(memoryChunks[0]));
			_ptr_last_chunk = _ptr_first_chunk;
			_ptr_cursor_chunk = _ptr_first_chunk;
		}
		else {
			newChunk = setChunkDefaults(&(memoryChunks[0]));
			_ptr_last_chunk->next = newChunk;
			_ptr_last_chunk = newChunk;
		}

		memoryOffset = (i * ((unsigned int)_memory_chunk_size));
		
		_ptr_last_chunk->data = &(newMemoryBlock[memoryOffset]);
		
		if (!allocationChunkAssigned) {
			_ptr_last_chunk->isAllocationBlock = true;
			allocationChunkAssigned = true;
		}
	}

	return recalcChunkMemorySize(_ptr_first_chunk, _ui_memory_chunk_count);
}

bool CMemoryPool::recalcChunkMemorySize(SMemoryChunk* chunk, unsigned int chunkCount)
{
	unsigned int memoryOffset = 0;
	for (unsigned int i = 0; i < chunkCount; i++) {
		if (chunk) {
			memoryOffset = (i * ((unsigned int)_memory_chunk_size));
			chunk->dataSize = ((unsigned int)_total_memory_pool_size) - static_cast<size_t>(memoryOffset);
			chunk = chunk->next;
		}
		else {
			assert(false && "Error: chunk = NULL");
			return false;
		}
	}
	return true;
}

SMemoryChunk* CMemoryPool::setChunkDefaults(SMemoryChunk* memoryChunk)
{
	memoryChunk->data = nullptr;
	memoryChunk->dataSize = 0;
	memoryChunk->usedSize = 0;
	memoryChunk->isAllocationBlock = false;
	memoryChunk->next = nullptr;

	return memoryChunk;
}


std::size_t CMemoryPool::calculateMemoryBlockSize(const std::size_t& memorySize)
{
	unsigned int uiNeededChunks = calculateNeededChunks(memorySize);
	return std::size_t(uiNeededChunks * _memory_chunk_size);
}

unsigned int CMemoryPool::calculateNeededChunks(const std::size_t& memorySize)
{
	float f = (((float)memorySize) / ((float)_memory_chunk_size));
	return (unsigned int)ceil(f);
}

void CMemoryPool::setMemoryChunkValues(SMemoryChunk* memoryChunk, std::size_t blockSize)
{
	memoryChunk->usedSize = blockSize;
}

SMemoryChunk* CMemoryPool::findChunkSuitableToHoldMemory(std::size_t& memorySize)
{
	SMemoryChunk* cursor = _ptr_cursor_chunk;
	unsigned int uiChunksToSkip, memoryOffset;
	while (cursor) {
		if (cursor = _ptr_last_chunk) {
			//Reached End Of List. Start Over
			cursor = _ptr_first_chunk;
		}

		if (!cursor->usedSize && cursor->dataSize >= memorySize) {
			_ptr_cursor_chunk = cursor;
			return cursor;
		}
		cursor = cursor->next;
	}

	return nullptr;
}

SMemoryChunk* CMemoryPool::findChunkHoldingPointerTo(void* memoryBlockPtr)
{
	SMemoryChunk* cursor = _ptr_first_chunk;
	while (cursor) {
		if (cursor->data == ((TBYTE*)memoryBlockPtr)) break;
		cursor = cursor->next;
	}

	return cursor;
}
