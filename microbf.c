/*
 * Adapted from Robert de Bath's Debian bug report
 * https://bugs.debian.org/cgi-bin/bugreport.cgi?bug=768885#5
 * Presumably written by him, but it hardly matters.
 *
 * My changes:
 * How EOF is interpreted (he had no change, I have input 0).
 * # Adds support for input embedded within program. Previously it was ignored.
 * Spaces for readability
 * Tabs
 * == 0 for readability and clarity instead of implicit boolean conversion
 * # indicates a planned implementation or change that has not yet been changed.
 *
*/


#define _POSIX_C_SOURCE 200809L // To enable getdelim
#include <stdio.h>

#include <string.h>

int main (int argc, char *argv[]) {
	char *b = 0, *p;
	unsigned char t[65536] = {0};
	unsigned short m = 0;
	int i = 0;
	FILE *fp = argc > 1 ? fopen(argv[1], "r") : stdin;
	if (fp == NULL) {
		perror(argv[1]);
		return -1;
	}

	size_t r = 0;
	if (getdelim(&b, &r, argc > 1 ? '\0' : '!', fp) < 0) {
		perror(argv[1]);
		return -1;
	}

	if (b != 0 && r > 0)
		for (p = b; *p != 0; p++)
			switch (*p) {
			case '>': m++; break;
			case '<': m--; break;
			case '+': t[m]++; break;
			case '-': t[m]--; break;
			case '.': putchar(t[m]); break;
			case ',':
				int c = getchar();
				t[m] = c == EOF ? 0 : c;
				break;
			case '[':
				if (t[m]==0)
					while ((i += (*p == '[') - (*p == ']')) && p[1]) p++;
				break;
			case ']':
				if (t[m] != 0)
					while ((i += (*p == ']') - (*p == '[')) && p > b) p--;
				break;
			}
	return 0;
}
