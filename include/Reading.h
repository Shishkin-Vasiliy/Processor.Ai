#ifndef HEADER_H
#define HEADER_H

#define MAXLINES 10000
#define MAXLEN 1000

#define POIZON_INDEX_PTR (char **) 66
#define POIZON_CHAR_PTR (char *) 13
#define POIZON_CHAR '@'

int OpenFile(const char *file_name);
int GetFileSize(const char *file_name);
int ReadFromFile(char **buf, char **index, int file_size, int descr);
int SplitBuf(char **buf, char **index);
void PrintStrings(char **index, size_t nlines, const char *REASON);
int FreeBuf(char **buf, int file_size);
int FreeIndex(char ***index, int nlines);

#endif