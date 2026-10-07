#include "Includes.h"
#include "Reading.h"

int OpenFile(const char *file_name)
{
    int descr = open(file_name, O_RDONLY);
    return descr;
}

int GetFileSize(const char *file_name)
{
    struct stat stat_buf = {};
    stat(file_name, &stat_buf);
    int file_size = stat_buf.st_size;

    return file_size;
}

int ReadFromFile(char **buf, char **index, int file_size, int descr)
{
    if (!index)
        return EOF;

    int nlines = 0;

    if (descr == EOF)
        return EOF;

    *buf = (char *)calloc(file_size + 1, sizeof(char));
    if (!(*buf))
        return EOF;

    read(descr, *buf, file_size);
    //
    *(*buf + file_size) = '\0'; 

    nlines = SplitBuf(buf, index);

    return nlines;
}

int SplitBuf(char **buf, char **index)
{
    if (!buf)
        return EOF;

    int i = 0;
    int nlines = 1;

    char *temp = *buf;
    index[i] = temp;

    while (*temp != '\0')
    {
        if (*temp == '\n')
        {
            *temp = '\0';
            temp++;
            i++;
            nlines++;
            while(isspace(*temp))
                temp++;  
            index[i] = temp;
            continue;
        }
        else
            temp++;            
    }
    return nlines;
}

void PrintStrings(char **index, size_t nlines, const char *REASON)
{
    assert(index);
    assert(*index);

    size_t i = 0;

    printf("%s\n\n", REASON);
    for (; i < nlines; i++)
        printf("%s\n", index[i]);
    printf("***************************************\n\n\n\n\n\n\n\n\n\n\n");
}

int FreeBuf(char **buf, int file_size)
{
    if (!buf)   
        return EOF;

    char *pos = *buf;
    for (int i = 0; i < file_size + 1; i++)
    {
        pos[i] = POIZON_CHAR;
    }
    free(*buf);
    *buf = POIZON_CHAR_PTR;

    return 0;
} 

int FreeIndex(char ***index, int nlines)
{
    if (!(*index) || !index)
        return EOF;

    for (int i = 0; i < nlines; i++)
    {
        (*index)[i] = POIZON_CHAR_PTR;
    }

    free(*index);
    *index = POIZON_INDEX_PTR;
    
    return 0;
}