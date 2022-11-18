#include <algorithm>
#include <memory>

#include <LuaHelper/LuaMem.h>


#if 0
#	include <iostream>
#	define PRINTN(text) std::cout << text << std::endl
#	define PRINT(text) std::cout << text
#else
#	define PRINTN(text) void(0)
#	define PRINT(text) void(0)
#endif


#define LUAMEM_ALIGNMENT 16
#define LUAMEM_SLOTSIZE  64
#define LUAMEM_NUMSLOTS  512  // 32kb
#define LUAMEM_POOLSIZE (LUAMEM_SLOTSIZE * LUAMEM_NUMSLOTS)


// ============================================================================
// == class LuaMemPerf
// ============================================================================


LuaMemPerfData* LuaMemPerf::GetPerfData()
{
	// calculate average slot usage
	int count = m_Perf.PoolAllocs + m_Perf.PoolReuses + m_Perf.PoolOOMAllocs;
	int _PoolAvgSlotUsg = m_Perf.PoolAvgSlotUsg / count;

	return nullptr;
}


// ============================================================================
// == class LuaMemGlobal
// ============================================================================


void* LuaMemGlobal::Alloc(void* ud, void* ptr, size_t osize, size_t nsize)
{
	LuaMemGlobal* allocator = static_cast<LuaMemGlobal*>(ud);

	if (nsize == 0)
	{
		if (ptr != nullptr)
			allocator->Deallocate(ptr, osize);
		return nullptr;
	}
	else
	{
		if (ptr == nullptr)
			return allocator->Allocate(nsize);
		else
			return allocator->Reallocate(ptr, osize, nsize);
	}
}


void* LuaMemGlobal::Reallocate(void* ptr, size_t osize, size_t nsize)
{
	if (osize == nsize)
	{
		m_Perf.SkippedReallocs++;
		return ptr;
	}

	void* newPtr = Allocate(nsize);
	memcpy(newPtr, ptr, std::min(osize, nsize));
	Deallocate(ptr, osize);
	return newPtr;
}


// see: https://www.appsloveworld.com/cplus/100/80/what-can-i-use-instead-of-stdaligned-alloc-in-ms-visual-studio-2013
void* LuaMemGlobal::Allocate(size_t size, int alignment)
{
	m_Perf.GlobalAllocs++;

	// adjust alignment (min alignof void*)
	if (alignment < alignof(void*))
		alignment = alignof(void*);

	// allocate memory
	std::size_t space = size + alignment - 1;
	void* allocated_mem = ::operator new(space + sizeof(void*));
	void* aligned_mem = static_cast<void*>(static_cast<char*>(allocated_mem) + sizeof(void*));

	// allign (aligned_mem)
	std::align(alignment, size, aligned_mem, space);

	// store real pointer for deallocation
	*(static_cast<void**>(aligned_mem) - 1) = allocated_mem;

	return aligned_mem;
}


void LuaMemGlobal::Deallocate(void* ptr, size_t size)
{
	(void)size; // not used

	m_Perf.GlobalFrees++;

	::operator delete(*(static_cast<void**>(ptr) - 1));

}


// ============================================================================
// == class LuaMemPool
// ============================================================================


LuaMemPool::LuaMemPool(size_t poolsize, int alignment)
{
	// min: 8 - max: m_SlotSize
	m_Alignment = std::min(std::max(alignment, LUAMEM_ALIGNMENT), LUAMEM_SLOTSIZE);

	// fixed: 64 bytes
	m_SlotSize = LUAMEM_SLOTSIZE;

	// multiple of m_SlotSize
	poolsize = (poolsize == 0) ? LUAMEM_POOLSIZE : poolsize;
	m_PoolSize = m_SlotSize * ((poolsize + m_SlotSize - 1) / m_SlotSize);

	// allocate the pool
	PRINTN("Allocate(pool) GLOBAL");
	PRINTN("Pool: " << (m_PoolSize / 1024) << " kb");
	m_Perf.GlobalAllocs++;
	m_Begin = m_GlobalAlloc.Allocate(m_PoolSize + m_Alignment, m_Alignment);
	m_End = (void*)((size_t)m_Begin + m_PoolSize);
	m_Current = (char*)m_Begin;

	// perf data
	m_Perf.PoolSlotsFree = (int)(m_PoolSize / m_SlotSize);
}


LuaMemPool::~LuaMemPool()
{
	// free the pool
	PRINTN("Deallocate(pool) GLOBAL");
	m_Perf.GlobalFrees++;
	m_GlobalAlloc.Deallocate(m_Begin, m_PoolSize + m_Alignment);

	PRINTN("===== Statistics =====");
	PRINTN("Global Alloc: ... " << m_Perf.GlobalAllocs);
	PRINTN("Global Free: .... " << m_Perf.GlobalFrees);
	PRINTN("Pool Alloc: ..... " << m_Perf.PoolAllocs);
	PRINTN("Pool Free: ...... " << m_Perf.PoolFrees);
	PRINTN("Pool Reuse: ..... " << m_Perf.PoolReuses);
	PRINTN("OutOfMemory: .... " << m_Perf.PoolOOMAllocs);
	PRINTN("Slots Free: ..... " << m_Perf.PoolSlotsFree);
	PRINTN("Slots Used: ..... " << m_Perf.PoolSlotsUsed);
	PRINTN("Avg. Slot usg: .. " << (m_Perf.PoolAvgSlotUsg / m_Perf.PoolAllocs));
	PRINTN("Skipped Reallocs: " << m_Perf.SkippedReallocs);
}


void* LuaMemPool::Alloc(void* ud, void* ptr, size_t osize, size_t nsize)
{
	LuaMemPool* allocator = static_cast<LuaMemPool*>(ud);

	if (nsize == 0)
	{
		if (ptr != nullptr)
			allocator->Deallocate(ptr, osize);
		return nullptr;
	}
	else
	{
		if (ptr == nullptr)
			return allocator->Allocate(nsize);
		else
			return allocator->Reallocate(ptr, osize, nsize);
	}
}


void* LuaMemPool::Reallocate(void* ptr, size_t osize, size_t nsize)
{
	if (osize == nsize)
	{
		PRINTN("Reallocate(" << osize << ", " << nsize << ") SKIPPED");
		m_Perf.SkippedReallocs++;
		return ptr;
	}

	PRINTN("Reallocate(" << osize << ", " << nsize << ")");
	PRINT("    ");
	void* newPtr = Allocate(nsize);
	memcpy(newPtr, ptr, std::min(osize, nsize));
	PRINT("    ");
	Deallocate(ptr, osize);
	return newPtr;
}


void* LuaMemPool::Allocate(size_t size)
{
	// global
	if (size > m_SlotSize)
	{
		PRINTN("Allocate(" << size << ") GLOBAL");
		m_Perf.GlobalAllocs++;
		return m_GlobalAlloc.Allocate(size, m_Alignment);
	}
	// pool
	else
	{
		m_Perf.PoolAvgSlotUsg += (int)size;

		// try to reuse previously freed blocks
		if (m_FreeListHead)
		{
			void* ptr = m_FreeListHead;
			m_FreeListHead = m_FreeListHead->m_Next;
			PRINTN("Allocate(" << size << ") POOL-REUSED");
			m_Perf.PoolReuses++;
			m_Perf.PoolSlotsFree--;
			m_Perf.PoolSlotsUsed++;
			return ptr;
		}
		// allocate a new block
		else
		{
			// get next aligned pos
			//m_Current = (char*)(((size_t)m_Current + (m_Alignment - 1)) & ~(m_Alignment - 1));

			// out of memory
			if (m_Current + m_SlotSize > m_End)
			{
				PRINTN("Allocate(" << size << ") GLOBAL  ==OUT-OF-MEMORY==");
				m_Perf.PoolOOMAllocs++;
				m_Perf.GlobalAllocs++;
				return m_GlobalAlloc.Allocate(size, m_Alignment);
			}

			void* ptr = m_Current;
			m_Current += m_SlotSize;
			PRINTN("Allocate(" << size << ") POOL-NEW");
			m_Perf.PoolAllocs++;
			m_Perf.PoolSlotsFree--;
			m_Perf.PoolSlotsUsed++;
			return ptr;
		}
	}
}


void LuaMemPool::Deallocate(void* ptr, size_t size)
{
	// global
	if ((size > m_SlotSize) || (ptr < m_Begin) || (ptr > m_End))
	{
		PRINTN("Deallocate(" << size << ") GLOBAL");
		m_Perf.GlobalFrees++;
		m_GlobalAlloc.Deallocate(ptr, size);
	}
	// pool
	else
	{
		FreeList* newHead = static_cast<FreeList*>(ptr);
		newHead->m_Next = m_FreeListHead;
		m_FreeListHead = newHead;
		PRINTN("Deallocate(" << size << ") POOL-FREELISTING");
		m_Perf.PoolFrees++;
		m_Perf.PoolSlotsFree++;
		m_Perf.PoolSlotsUsed--;
	}
}
