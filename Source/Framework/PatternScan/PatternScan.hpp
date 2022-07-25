#ifndef FRAMEWORK_PATTERNSCAN_H
#define FRAMEWORK_PATTERNSCAN_H

#ifdef FRAMEWORK_ENABLE_PATTERN_SCANNING

class Pattern {
public:
	const char *signature;
	const char *mask;

	unsigned int length;

	Pattern(const char *signature, const char *mask) noexcept {
		this->signature = signature;
		this->mask = mask;
	}
};

namespace Framework {
	namespace PatternScan {
		void *searchPattern(Pattern pattern, void *begin, const void *end = nullptr);
	}
}

#endif
#endif
