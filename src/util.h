#ifndef UTIL_H
#define UTIL_H

const int index_of(const char *str, const char *sub);
const int extract_int(const char *str);
const int readline_s(FILE *fp, char *buf, int max_len);
const int readline(FILE *fp, char *buf);

#endif
