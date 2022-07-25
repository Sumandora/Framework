#include "PatternScan.hpp"

#include <cstring>
#include <sstream>

#ifdef FRAMEWORK_ENABLE_PATTERN_SCANNING

//TODO merge

void* Framework::PatternScan::searchPattern(Pattern pattern, void* begin, const void* end) {
	unsigned int index = 0;
	unsigned int length = strlen(pattern.mask);
	while (true) {
		if (end != nullptr && static_cast<char *>(begin) + index + length > end) return nullptr;
		bool found = true;
		for (unsigned int i = 0; i < length; i++)
			if (pattern.mask[i] == 'x' &&
				*(static_cast<unsigned char *>(begin) + index + i) !=
				static_cast<unsigned char>(pattern.signature[i])) {
				found = false;
				break;
			}
		if (found)
			break;
		index++;
	}
	return static_cast<char *>(begin) + index;
}

#endif
