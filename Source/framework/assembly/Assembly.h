//
// Created by johannes on 4/17/22.
//

#ifndef FRAMEWORK_ASSEMBLY_H
#define FRAMEWORK_ASSEMBLY_H

#define FRAMEWORK_HOOKING_NEAR_JMP_INSTRUCTION_LENGTH 5
#define FRAMEWORK_HOOKING_ABS_JMP_INSTRUCTION_LENGTH 13
#define FRAMEWORK_HOOKING_ABS_PUSH_INSTRUCTION_LENGTH 12

namespace Framework {
    namespace Assembly {
        static void writeNearJmp(void *addr, void *goal) {
            unsigned char jmpInstruction[] = {
                    0xE9, 0x0, 0x0, 0x0, 0x0 // jmp goal
            };
            void *jmpTarget = reinterpret_cast<void *>(static_cast<char *>(goal) -
                                                       (static_cast<char *>(addr) +
                                                        FRAMEWORK_HOOKING_NEAR_JMP_INSTRUCTION_LENGTH));
            memcpy(jmpInstruction + 1, &jmpTarget, sizeof(jmpInstruction) - 1 /* E9 */);
            memcpy(addr, jmpInstruction, sizeof(jmpInstruction));
        }

        static void writeAbsPush(void *addr, void *value) {
            unsigned char absPushInstruction[] = {
                    0x49, 0xBF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //mov r15, value
                    0x41, 0x57 //push r15
            };
            memcpy(absPushInstruction + 2, &value, sizeof(void *));
            memcpy(addr, absPushInstruction, FRAMEWORK_HOOKING_ABS_PUSH_INSTRUCTION_LENGTH);
        }

        static void writeAbsJmp(void *addr, void *goal) {
            unsigned char absJumpInstructions[] = {
                    0x49, 0xBF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //mov r15, goal
                    0x41, 0xFF, 0xE7 //jmp r15
            };
            memcpy(absJumpInstructions + 2, &goal, sizeof(void *));
            memcpy(addr, absJumpInstructions, FRAMEWORK_HOOKING_ABS_JMP_INSTRUCTION_LENGTH);
        }
    }
}

#endif //FRAMEWORK_ASSEMBLY_H
