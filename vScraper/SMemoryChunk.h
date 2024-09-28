#pragma once
#include <Windows.h>
#include <cstddef>

#ifdef UNICODE
typedef WCHAR TBYTE;
#else
typedef unsigned char TBYTE;
#endif

typedef struct SMemoryChunk {
	TBYTE* data;
	std::size_t dataSize;
	std::size_t usedSize;
	bool isAllocationBlock;

	SMemoryChunk* next;
} SMemoryChunk;