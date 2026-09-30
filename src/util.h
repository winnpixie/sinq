#ifndef UTIL_H
#define UTIL_H

const int str_indexof(const char *str, const char *sub);

const int extract_int(const char *str);
const double extract_fp(const char *str);

const int io_readline_s(FILE *fp, char *buf, int max_len);
const int io_readline(FILE *fp, char *buf);

#endif
