#include "Includes.h"
#include "Reading.h"

int OpenFile(const char *file_name)
{
    int descr = open(file_name, O_RDONLY);
    return descr;
}

int CloseFile(int fd)
{
    int result = close(fd);
    return result;
}

size_t GetFileSize(const char *file_name)
{
    struct stat stat_buf = {};
    stat(file_name, &stat_buf);
    size_t file_size = stat_buf.st_size;

    return file_size;
}

size_t ReadFromFile(char **buf, char **index, size_t file_size, int descr)
{
    if (!index)
        return 0;

    size_t nlines = 0;

    if (descr == EOF)
        return 0;

    *buf = (char *)calloc(file_size + 1, sizeof(char));
    if (!(*buf))
        return 0;

    read(descr, *buf, file_size);

    *(*buf + file_size) = '\0'; 

    nlines = SplitBuf(buf, index);

    return nlines;
}

size_t SplitBuf(char **buf, char **index)
{
    if (!buf)
        return 0;

    size_t i = 0;
    size_t nlines = 1;

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
            //while(isspace(*temp))          ///////  возможно тут проблема с пустыми строками (их лучше записывать и не пропускать)
            //    temp++;  
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

int FreeBuf(char **buf, size_t file_size)
{
    if (!buf)   
        return EOF;

    char *pos = *buf;
    for (size_t i = 0; i < file_size + 1; i++)
    {
        pos[i] = POIZON_CHAR;
    }
    free(*buf);
    *buf = POIZON_CHAR_PTR;

    return 0;
} 

int FreeIndex(char ***index, size_t nlines)
{
    if (!index || !(*index))
        return EOF;

    for (size_t i = 0; i < nlines; i++)
    {
        (*index)[i] = POIZON_CHAR_PTR;
    }

    free(*index);
    *index = POIZON_INDEX_PTR;
    
    return 0;
}

int IsEmptyLine(const char *s)
{
    if (*s == '\0')
        return 1;
        
    while (*s)
    {
        if (!isspace(*s))
            return 0;
        s++;
    }
    return 1;
}