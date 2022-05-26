//
// Created by johannes on 4/17/22.
//

#ifndef FRAMEWORK_RETURNADDR_H
#define FRAMEWORK_RETURNADDR_H

namespace Framework {
    namespace ReturnAddr {
        static Pattern jmp_r14_pattern(
                "\x41\xFF\xE6",
                "xxx"
        );
        static void *jmp_r14_addr = nullptr;

        namespace {
            Pattern call_instruction(
                    "\xFF\x00\x00\x00\x00\x00\x90",
                    "x?????x"
            );

            __attribute((noinline)) void *rip() {
                return __builtin_extract_return_addr(__builtin_return_address(0));
            }
        }

        template<typename Ret, typename... Args>
        static __attribute((noinline, optimize("O0"))) auto invoke(void *method, Args ...args) -> Ret {
            static bool mutated = false;
            static void *afterCall;

            if (!mutated) {
                void *instruction = rip();

                void *base = PatternScan::searchPattern(call_instruction, instruction);

                int callRegisterOffset = 1;

                if (*(static_cast<unsigned char *>(base) - 1) == 0x41) {
                    base = static_cast<char *>(base) - 1;
                    callRegisterOffset++;
                }

                int length = 0;
                while (*(static_cast<unsigned char *>(base) + length) != 0x90)
                    length++;

                Memory::protect(base, PROT_READ | PROT_WRITE | PROT_EXEC);

                memcpy(static_cast<char *>(base) + 12, base, length);

                Assembly::writeAbsPush(base, static_cast<char *>(jmp_r14_addr));

                *(static_cast<char *>(base) + FRAMEWORK_HOOKING_ABS_PUSH_INSTRUCTION_LENGTH +
                  callRegisterOffset) += 0x10; // call to jmp
                afterCall =
                        static_cast<char *>(base) + FRAMEWORK_HOOKING_ABS_PUSH_INSTRUCTION_LENGTH + callRegisterOffset +
                        1;

                Memory::protect(base, PROT_READ | PROT_EXEC);
                mutated = true;
            }

            __asm volatile(
                    "mov %0,%%r14;\n"
                    : : "r"(afterCall)
                    );
            ((Ret (*)(Args...)) method)(args...);
            __asm("nop"); // we need 12 more bytes for the push instruction
            __asm("nop");
            __asm("nop");
            __asm("nop");
            __asm("nop");
            __asm("nop");
            __asm("nop");
            __asm("nop");
            __asm("nop");
            __asm("nop");
            __asm("nop");
            __asm("nop"); // This one seems to be useless, but keep it for safety reasons
        }
    }
}
#endif //FRAMEWORK_RETURNADDR_H
