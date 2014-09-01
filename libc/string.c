#include <string.h>

size_t strlen(char const *s)
{
	size_t ret = 0;

	while(s[ret] != '\0') {
		++ret;
	}

	return ret;
}

int strcmp(char const *s1, char const *s2)
{
	size_t i;
	for(i = 0; s1[i] != '\0' && s2[i] != '\0'; ++i) {
		if(s1[i] != s2[i])
			break;
	}

	if(s1[i] < s2[i])
		return -1;
	else if(s1[i] == s2[i])
		return 0;

	return 1;
}

int strncmp(const char *s1, const char *s2, size_t n)
{
	size_t i;

	if(n == 0)
		return 0;

	for(i = 0; i < n - 1 && s1[i] != '\0' && s2[i] != '\0'; ++i) {
		if(s1[i] != s2[i])
			break;
	}

	if(s1[i] < s2[i])
		return -1;
	else if(s1[i] == s2[i])
		return 0;

	return 1;
}

char *strcpy(char *dest, char const *src)
{
	size_t i;

	for(i = 0; src[i] != '\0'; ++i)
		dest[i] = src[i];

	dest[i] = '\0';

	return dest;
}

char *strncpy(char *dest, char const *src, size_t n)
{
	size_t i;

	for(i = 0; i < n && src[i] != '\0'; ++i)
		dest[i] = src[i];

	if(i < n)
		dest[i] = '\0';

	return dest;
}

char* strcat(char *dest, const char *src)
{
	size_t dest_len = strlen(dest);
	size_t i;

	for (i = 0 ; src[i] != '\0' ; i++)
		dest[dest_len + i] = src[i];
	dest[dest_len + i] = '\0';

	return dest;
}

char* strncat(char *dest, const char *src, size_t n)
{
	size_t dest_len = strlen(dest);
	size_t i;

	for (i = 0 ; i < n && src[i] != '\0' ; i++)
		dest[dest_len + i] = src[i];
	dest[dest_len + i] = '\0';

	return dest;
}

int atoi(const char *nptr)
{
	size_t i;
	int res = 0, nb;

	for(i = 0; nptr[i] != 0; ++i) {
		nb = nptr[i] - '0';
		res = res * 10 + nb;
	}

	return res;
}
