#ifndef FRAMEWORK_HOOKING_H
#define FRAMEWORK_HOOKING_H

namespace Framework {
	namespace Hooking {
#ifdef FRAMEWORK_ENABLE_HOOKING_DETOUR
		void* detour(void* original, const void* hook, int instructionLength);
#endif

#ifdef FRAMEWORK_ENABLE_HOOKING_PTRSWAP
		void* relativePtrSwap(void* original, const void* hook);
#endif
	}
}

#endif
