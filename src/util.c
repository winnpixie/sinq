#include <stdio.h>
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
	char str_tmp[len + 1];

	int idx = 0;
	for (int i = 0; i < len; i++)
	{
		char c = str[i];
		if (c >= '0' && c <= '9')
		{
			str_tmp[idx++] = c;
		}
	}
	str_tmp[idx] = '\0';

	return atoi(str_tmp);
}

int read_line_s(FILE *fp, char *buf, int max_len)
{
	if (max_len == 0)
	{
		return -1;
	}

	int len = -1;
	int c;
	while ((c = fgetc(fp)) != EOF)
	{
		if (len == -1)
		{
			len = 0;
		}

		int eol = c == '\r' || c == '\n';
		if (c == '\r')
		{
			c = fgetc(fp);
			eol = c == EOF || c == '\n';
		}

		if (eol)
		{
			break;
		}

		buf[len++] = c;
		if (max_len > -1 && len >= max_len)
		{
			break;
		}
	}

	if (len > -1)
	{
		buf[len] = '\0';
	}

	return len;
}

int read_line(FILE *fp, char *buf)
{
	return read_line_s(fp, buf, -1);
}
