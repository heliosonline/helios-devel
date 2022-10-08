#pragma once


struct LuaMemPerfData
{
	// Allocations from global heap
	int GlobalAllocs = 0;
	// Deallocations from global heap
	int GlobalFrees = 0;

	// Allocations from pool
	int PoolAllocs = 0;
	// Deallocations from pool
	int PoolFrees = 0;
	// Reused slots from pool
	int PoolReuses = 0;
	// Global allocations (when out of memory)
	int PoolOOMAllocs = 0;
	// Count of unused slots in pool
	int PoolSlotsFree;
	// Count of used slots in pool
	int PoolSlotsUsed = 0;
	// Average bytes used in slots
	int PoolAvgSlotUsg = 0;

	// Skipped reallocations
	int SkippedReallocs = 0;
};


class LuaMemPerf
{
public:
	LuaMemPerfData* GetPerfData();

protected:
	LuaMemPerfData m_Perf;
};


class LuaMemGlobal : LuaMemPerf
{
public:
	static void* Alloc(void* ud, void* ptr, size_t osize, size_t nsize);

	void* Reallocate(void* ptr, size_t osize, size_t nsize);
	void* Allocate(size_t size, int alignment = 0);
	void Deallocate(void* ptr, size_t size);
};


class LuaMemPool : LuaMemPerf
{
public:
	LuaMemPool(size_t poolsize = 0, int alignment = 0);
	~LuaMemPool();

public:
	static void* Alloc(void* ud, void* ptr, size_t osize, size_t nsize);

	void* Reallocate(void* ptr, size_t osize, size_t nsize);
	void* Allocate(size_t size);
	void Deallocate(void* ptr, size_t size);

private:
	// fallback allocator
	LuaMemGlobal m_GlobalAlloc;

	// pool
	void* m_Begin;
	void* m_End;
	char* m_Current;

	// config
	int m_Alignment;
	int m_SlotSize;
	size_t m_PoolSize;

	// freelist
	struct FreeList
	{
		FreeList* m_Next;
	};
	FreeList* m_FreeListHead = nullptr;
};
