#ifndef HEADER_H
#define HEADER_H

#define MAXLINES 10000
#define MAXLEN 1000

#define POIZON_INDEX_PTR (char **) 66
#define POIZON_CHAR_PTR (char *) 13
#define POIZON_CHAR '@'


int OpenFile(const char *file_name);
size_t GetFileSize(const char *file_name);
size_t ReadFromFile(char **buf, char **index, size_t file_size, int descr);
size_t SplitBuf(char **buf, char **index);
void PrintStrings(char **index, size_t nlines, const char *REASON);
int FreeBuf(char **buf, size_t file_size);
int FreeIndex(char ***index, size_t nlines);
int IsEmptyLine(const char *s);

#endif