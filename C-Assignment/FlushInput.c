#include <stdio.h>

void flushInput() {
	// Flush the input buffer
	// Source: http://c-faq.com/stdio/stdinflush2.html
	int c;
	while ((c = getchar()) != EOF && c != '\n');
}