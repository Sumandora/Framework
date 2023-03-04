#include "PatternScan.hpp"

#include <cstring>
#include <sstream>

#ifdef FRAMEWORK_ENABLE_PATTERN_SCANNING

Pattern::Pattern(const char* signature, const char* mask) noexcept {
	this->signature = signature;
	this->mask = mask;
	
	this->length = strlen(mask);
}

void* Pattern::searchPattern(void* begin, const void* end) {
	unsigned int index = 0;
	while (true) {
		if (end != nullptr && static_cast<char*>(begin) + index + length > end)
			return nullptr;
		if(matchPattern(static_cast<char*>(begin) + index))
			break;
		index++;
	}
	return static_cast<char*>(begin) + index;
}

bool Pattern::matchPattern(void* addr) {
	for (unsigned int i = 0; i < length; i++)
		if (mask[i] == 'x' && *(static_cast<unsigned char*>(addr) + i) != static_cast<unsigned char>(signature[i]))
			return false;
	
	return true;
}

#endif
