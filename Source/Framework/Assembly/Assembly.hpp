#ifndef FRAMEWORK_ASSEMBLY_H
#define FRAMEWORK_ASSEMBLY_H

#define FRAMEWORK_NEAR_JMP_LENGTH 5
#define FRAMEWORK_ABS_JMP_LENGTH 12
#define FRAMEWORK_ABS_PUSH_LENGTH 11

namespace Framework {
	namespace Assembly {
#ifdef FRAMEWORK_ENABLE_HOOKING_DETOUR
		void writeNearJmp(void* addr, void* goal);
#endif

#ifdef FRAMEWORK_ENABLE_RETURN_ADDRESS
		void writeAbsPush(void* addr, void* value);
#endif

#if defined(FRAMEWORK_ENABLE_HOOKING_DETOUR) || defined(FRAMEWORK_ENABLE_HOOKING_PTRSWAP)
		void writeAbsJmp(void* addr, void* goal);
#endif
	}
}

#endif
