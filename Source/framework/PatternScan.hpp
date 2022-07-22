#ifndef FRAMEWORK_PATTERNSCAN_H
#define FRAMEWORK_PATTERNSCAN_H

#include <cstring>
#include <sstream>

#ifdef ENABLE_PATTERN_SCANNING

class Pattern {
public:
	const char *signature;
	const char *mask;

	unsigned int length;

	Pattern(const char *signature, const char *mask) noexcept {
		this->signature = signature;
		this->mask = mask;

		length = strlen(this->mask);
	}
};

namespace Framework {
	namespace PatternScan {
		static void *searchPattern(Pattern pattern, void *begin, const void *end = nullptr) {
			unsigned int index = 0;
			while (true) {
				if (end != nullptr && static_cast<char *>(begin) + index + pattern.length > end) return nullptr;
				bool found = true;
				for (unsigned int i = 0; i < pattern.length; i++)
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
	}
}

#endif
#endif //FRAMEWORK_PATTERNSCAN_H
