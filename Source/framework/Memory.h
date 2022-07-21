//
// Created by johannes on 4/17/22.
//

#ifndef FRAMEWORK_MEMORY_H
#define FRAMEWORK_MEMORY_H

#include <cstring>
#include <sys/mman.h>
#include <unistd.h>

#define FRAMEWORK_TWO_GB 0x7ffffff0

namespace Framework {
	namespace Memory {
#if defined(ENABLE_HOOKING_DETOUR) || defined(ENABLE_HOOKING_PTRSWAP)
		// finds a memory page, which is at max 2 GB away from begin
		static void *findUnusedMemory(void *begin) {
			unsigned int pagesize = getpagesize();
			unsigned long currentPage = (size_t) begin / pagesize;
			unsigned long maxOffset = FRAMEWORK_TWO_GB / pagesize;
			unsigned long offset = 0;
			while (offset < maxOffset) {
				for (int sign = -1; sign <= (offset != 0 ? 1 : -1); sign += 2) {
					if (sign < 0 && offset > currentPage) continue;
					unsigned long page = currentPage + offset * sign;
					void *pointer = mmap(
						reinterpret_cast<void *>(page * pagesize),
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

#if defined(ENABLE_RETURN_ADDRESS) || defined(ENABLE_HOOKING_DETOUR)
		// This changes the protection for the entire memory page
		static void protect(void *addr, int prot) {
			unsigned long pagesize = sysconf(_SC_PAGESIZE);
			void *aligned = (char *) ((((intptr_t) addr) + pagesize - 1) & ~(pagesize - 1)) - pagesize;
			mprotect(aligned, pagesize, prot);
		}
	}
#endif
}

#endif //FRAMEWORK_MEMORY_H
