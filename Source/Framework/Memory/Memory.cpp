#include "Memory.hpp"

#include <cstring>
#include <sys/mman.h>
#include <unistd.h>
#include <cstdint>

#define FRAMEWORK_TWO_GB 0x7ffffff0

// Finds a memory page, which is at max 2 GB away from begin
#if defined(FRAMEWORK_ENABLE_HOOKING_DETOUR) || defined(FRAMEWORK_ENABLE_HOOKING_PTRSWAP)
void* Framework::Memory::findUnusedMemory(const void* begin) {
	unsigned int pagesize = getpagesize();
	unsigned long currentPage = reinterpret_cast<size_t>(begin) / pagesize;
	unsigned long maxOffset = FRAMEWORK_TWO_GB / pagesize;
	unsigned long offset = 0;
	while (offset < maxOffset) {
		for (int sign = -1; sign <= (offset != 0 ? 1 : -1); sign += 2) {
			if (sign < 0 && offset > currentPage) continue;
			unsigned long page = currentPage + offset * sign;
			void* pointer = mmap(
				reinterpret_cast<void*>(page * pagesize),
				pagesize,
				PROT_READ | PROT_WRITE | PROT_EXEC,
				MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED_NOREPLACE,
				0,
				0);
			if (pointer != MAP_FAILED)
				return pointer;
		}
		offset++;
	}
	return nullptr;
}
#endif

// This changes the protection for the entire memory page
#if defined(FRAMEWORK_ENABLE_RETURN_ADDRESS) || defined(FRAMEWORK_ENABLE_HOOKING_DETOUR)
void Framework::Memory::protect(const void* addr, int prot) {
	unsigned long pagesize = sysconf(_SC_PAGESIZE);
	void* aligned = reinterpret_cast<char*>((reinterpret_cast<uintptr_t>(addr) + pagesize - 1) & ~(pagesize - 1)) - pagesize;
	mprotect(aligned, pagesize, prot);
}
#endif
