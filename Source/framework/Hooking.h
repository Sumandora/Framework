//
// Created by johannes on 4/17/22.
//

#ifndef FRAMEWORK_HOOKING_H
#define FRAMEWORK_HOOKING_H

#include "Assembly.h"
#include "Memory.h"

namespace Framework {
	namespace Hooking {
		static void *hookFunc(void *original, void *hook, int instructionLength) {
			if (FRAMEWORK_NEAR_JMP_LENGTH > instructionLength) return nullptr;
			void *unusedMemory = Memory::findUnusedMemory(original);
			if (unusedMemory) {
				Assembly::writeAbsJmp(unusedMemory, hook);
				memcpy(static_cast<char *>(unusedMemory) + FRAMEWORK_ABS_JMP_LENGTH,
					original,
					instructionLength);
				Assembly::writeNearJmp(
					static_cast<char *>(unusedMemory) + FRAMEWORK_ABS_JMP_LENGTH +
					instructionLength,
					static_cast<char *>(original) + FRAMEWORK_NEAR_JMP_LENGTH);
				Memory::protect(unusedMemory, PROT_READ | PROT_EXEC);

				Memory::protect(original, PROT_READ | PROT_WRITE | PROT_EXEC);
				Assembly::writeNearJmp(original, unusedMemory);
				memset(static_cast<char *>(original) + FRAMEWORK_NEAR_JMP_LENGTH,
					0x90 /* NOP */,
					instructionLength - FRAMEWORK_NEAR_JMP_LENGTH);
				Memory::protect(original, PROT_READ | PROT_EXEC);

				return static_cast<char *>(unusedMemory) + FRAMEWORK_ABS_JMP_LENGTH;
			} else {
				return nullptr;
			}
		}
	}
}

#endif //FRAMEWORK_HOOKING_H
