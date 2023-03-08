#ifndef FRAMEWORK_PATTERNSCAN_H
#define FRAMEWORK_PATTERNSCAN_H

#ifdef FRAMEWORK_ENABLE_PATTERN_SCANNING

class Pattern {
private:
	const char* signature;
	const char* mask;

	const unsigned int length;

public:
	Pattern(const char* signature, const char* mask) noexcept;

	void* searchPattern(void* begin, const void* end = nullptr) const;
	bool matchPattern(void* addr) const;
};

#endif
#endif
