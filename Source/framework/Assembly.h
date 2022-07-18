//
// Created by johannes on 4/17/22.
//

#ifndef FRAMEWORK_ASSEMBLY_H
#define FRAMEWORK_ASSEMBLY_H

#define FRAMEWORK_NEAR_JMP_LENGTH 5
#define FRAMEWORK_ABS_JMP_LENGTH 13
#define FRAMEWORK_ABS_PUSH_LENGTH 12

namespace Framework {
	namespace Assembly {
		static void writeNearJmp(void *addr, void *goal) {
			unsigned char jmpInstruction[] = {
				0xE9, 0x0, 0x0, 0x0, 0x0 // jmp goal
			};
			// Calculation for a relative jmp
			void *jmpTarget = reinterpret_cast<void *>(static_cast<char *>(goal) -
				(static_cast<char *>(addr) +
				FRAMEWORK_NEAR_JMP_LENGTH)); // Jumps always start at the rip, which has already increased
			memcpy(jmpInstruction + 1, &jmpTarget, sizeof(jmpInstruction) - 1 /* E9 */);
			memcpy(addr, jmpInstruction, sizeof(jmpInstruction));
		}
		
		/*
		 * The following 2 methods both use r15 as register
		 * It appears, that the System V ABI does not explicitly define a use for r12-r15 + a few others
		 * So gcc does not use it extensively
		 * r15 seems to be our best bet when it comes to a mostly unused register
		 * But it still is a gamble
		 * Making a third parameter, which specifies the register would be a solution, but some
		 * instructions change their encoding, because of addressing attributes
		 * e.g. jmp/call instructions get longer when using certain registers (including r15)
		 * So we might end up wasting a byte, which would be a pretty porely designed code
		 */
		static void writeAbsPush(void *addr, void *value) {
			unsigned char absPushInstruction[] = {
				0x49, 0xBF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //mov r15, value
				0x41, 0x57 //push r15
			};
			memcpy(absPushInstruction + 2, &value, sizeof(void *));
			memcpy(addr, absPushInstruction, FRAMEWORK_ABS_PUSH_LENGTH);
		}

		static void writeAbsJmp(void *addr, void *goal) {
			unsigned char absJumpInstructions[] = {
				0x49, 0xBF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //mov r15, goal
				0x41, 0xFF, 0xE7 //jmp r15
			};
			memcpy(absJumpInstructions + 2, &goal, sizeof(void *));
			memcpy(addr, absJumpInstructions, FRAMEWORK_ABS_JMP_LENGTH);
		}
	}
}

#endif //FRAMEWORK_ASSEMBLY_H
