#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>
#include <string.h>

int index_of(const char *str, const char *sub)
{
	char *idx = strstr(str, sub);
	if (idx == NULL)
	{
		return -1;
	}

	return idx - str;
}

int extract_int(const char *str)
{
	int len = strlen(str);
	char *tmp = malloc(sizeof(char) * len);
	if (tmp == NULL)
	{
		return -1;
	}

	int idx = 0;
	for (int i = 0; i < len; i++)
	{
		char c = str[i];
		if (c >= '0' && c <= '9')
		{
			tmp[idx++] = c;
		}
	}
	tmp[idx] = '\0';

	int nval = atoi(tmp);
	free(tmp);

	return nval;
}

#endif