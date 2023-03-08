#ifndef FRAMEWORK_MEMORY_H
#define FRAMEWORK_MEMORY_H

namespace Framework {
	namespace Memory {
#if defined(FRAMEWORK_ENABLE_HOOKING_DETOUR) || defined(FRAMEWORK_ENABLE_HOOKING_PTRSWAP)
		void* findUnusedMemory(const void* begin);
#endif

#if defined(FRAMEWORK_ENABLE_RETURN_ADDRESS) || defined(FRAMEWORK_ENABLE_HOOKING_DETOUR)
		void protect(const void* addr, int prot);
#endif
	}
}

#endif
