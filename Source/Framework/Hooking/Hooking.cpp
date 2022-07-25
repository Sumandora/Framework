#include "Hooking.hpp"

#include "../Assembly/Assembly.hpp"
#include "../Memory/Memory.hpp"

#include <sys/mman.h>
#include <cstring>

/*
 * There is no disassembler (yet; might never be)
 * So right now we have to hard code the amount of bytes, which have to be copied
 * This can change on recompilation, so in case you are crashing, check if the instructionLength-variable
 * is still set to a valid amount of bytes
 * It also has be bigger than the near jmp length (FRAMEWORK_ABS_JMP_LENGTH)
 * Using a disassembler with this layout should be easy
 * Loop through each instruction add its length onto a variable and as soon as the variable is bigger than FRAMEWORK_ABS_JMP_LENGTH (5)
 * Break out of the loop and pass this value into instructionLength
 */
#ifdef FRAMEWORK_ENABLE_HOOKING_DETOUR
void* Framework::Hooking::detour(void* original, void* hook, int instructionLength) {
	if (FRAMEWORK_NEAR_JMP_LENGTH > instructionLength) return nullptr;
	void* unusedMemory = Memory::findUnusedMemory(original);
	if (unusedMemory) {
		// Jmp into our code
		Assembly::writeAbsJmp(unusedMemory, hook);
		// Copy the stolen bytes
		memcpy(static_cast<char*>(unusedMemory) + FRAMEWORK_ABS_JMP_LENGTH,
			original,
			instructionLength);
		// Write the jmp back into the original code
		Assembly::writeNearJmp(
			static_cast<char*>(unusedMemory) + FRAMEWORK_ABS_JMP_LENGTH +
			instructionLength,
			static_cast<char*>(original) + FRAMEWORK_NEAR_JMP_LENGTH);
		Memory::protect(unusedMemory, PROT_READ | PROT_EXEC);

		Memory::protect(original, PROT_READ | PROT_WRITE | PROT_EXEC);
		// Write near jmp into the code, we are hooking
		Assembly::writeNearJmp(original, unusedMemory);
		/*
		 * In case we copied too many instructions, we can clean up the code by replacing the orphaned bytes with NOPs
		 * This is purely optional, but maybe some debugger goes crazy
		 * On the other side, anticheats shouldn't care about it either, they ban you
		 * as soon as they see the jmp instruction at the beginning, they ban you anyway
		 */
		memset(static_cast<char*>(original) + FRAMEWORK_NEAR_JMP_LENGTH,
			0x90 /* NOP */,
			instructionLength - FRAMEWORK_NEAR_JMP_LENGTH);
		Memory::protect(original, PROT_READ | PROT_EXEC);

		// This is the point after the abs jmp, means right where the stolen bytes begin
		return static_cast<char*>(unusedMemory) + FRAMEWORK_ABS_JMP_LENGTH;
	} else {
		return nullptr;
	}
}
#endif

/*
 * This is a more secure method of hooking, but it requires a jmp/call instruction
 * It works by taking the location of the jmp/call and writing a new pointer, which points
 * to our hook, these hooks has to later call the original or match the return value.
 * This has the side-effect of not being able to hook all calls, but only one at
 */
#ifdef FRAMEWORK_ENABLE_HOOKING_PTRSWAP
void* Framework::Hooking::relativePtrSwap(void* original, void* hook) {
	void* unusedMemory = Memory::findUnusedMemory(original);
	if(unusedMemory) {
		// Jmp into our code
		Assembly::writeAbsJmp(unusedMemory, hook);
		Memory::protect(unusedMemory, PROT_READ | PROT_EXEC);
		Memory::protect(original, PROT_READ | PROT_WRITE | PROT_EXEC);
		// Ptr Swap
		void* realTarget = static_cast<char*>(original) +
			(FRAMEWORK_NEAR_JMP_LENGTH - 1) +
			*reinterpret_cast<int*>(original);
		void *relativeAddress = reinterpret_cast<void *>(static_cast<char*>(unusedMemory) -
			(static_cast<char*>(original) +
			(FRAMEWORK_NEAR_JMP_LENGTH - 1)));
		memcpy(original, &relativeAddress, FRAMEWORK_NEAR_JMP_LENGTH - 1);
		Memory::protect(original, PROT_READ | PROT_EXEC);
		return realTarget;
	} else {
		return nullptr;
	}
}
#endif
