//
// Created by johannes on 4/17/22.
//

#ifndef FRAMEWORK_MEMORY_H
#define FRAMEWORK_MEMORY_H

#include <cstring>
#include <sys/mman.h>
#include <unistd.h>

#define FRAMEWORK_HOOKING_TWO_GB 2147483392

namespace Framework {
    namespace Memory {
        static void *findUnusedMemory(void *begin) { // begin being NULL will result in a random location being picked
            unsigned int pagesize = getpagesize();
            unsigned long currentPage = (size_t) begin / pagesize;
            unsigned long maxOffset = FRAMEWORK_HOOKING_TWO_GB / pagesize;
            unsigned long offset = 0;
            while (offset < maxOffset) {
                for (int sign = -1; sign <= (offset == 0 ? -1 : 1); sign += 2) {
                    if (sign < 0 && offset > currentPage) continue;
                    unsigned long page = currentPage + offset * sign;
                    void *pointer = mmap(reinterpret_cast<void *>(page * pagesize), pagesize,
                                         PROT_READ | PROT_WRITE | PROT_EXEC,
                                         MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED_NOREPLACE, 0, 0);
                    if (pointer != MAP_FAILED) {
                        return pointer;
                    } else {
                    }
                }
                offset++;
            }
            return nullptr;
        }

        static void protect(void *addr, int prot) {
            unsigned long pagesize = sysconf(_SC_PAGESIZE);
            void *aligned = (char *) ((((intptr_t) addr) + pagesize - 1) & ~(pagesize - 1)) - pagesize;
            mprotect(aligned, pagesize, prot);
        }
    }
}

#endif //FRAMEWORK_MEMORY_H
