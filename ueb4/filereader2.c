#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "linebuffer.h"
#include "fileindex.h"

FileIndex *fi_new(const char *filepath, const char *separator)
{
    int fd = open(filepath, O_RDWR | O_CREAT, 0640);
    int sectionEnd, lineStart = 0;
    int fieIndex = 1;
    FileIndex *newFI = calloc(1, sizeof(FileIndex));
    FileIndexEntry *newFIE = calloc(1, sizeof(FileIndexEntry));
    LineBuffer *b = buf_new(fd, "\n");
    char line[LINEBUFFERSIZE];
    char *linepointer = line;

    newFI->filepath = filepath;
    newFI->entries = newFIE;
    newFI->nEntries++;

    newFIE->nr = fieIndex;

    while (1)
    {
        lineStart = buf_readline(b, linepointer, LINEBUFFERSIZE);
        if (lineStart < 0)
        {
            newFIE->size = sectionEnd - newFIE->seekpos;
            newFI->totalSize += newFIE->size;
            break;
        }

        if (!strcmp(linepointer, separator))
        {
            lineStart = buf_readline(b, linepointer, LINEBUFFERSIZE);
            newFIE->seekpos = lineStart;
            newFIE->lines += 1;
            sectionEnd = buf_where(b);
        }
        else if (!strcmp(linepointer, ""))
        {
            lineStart = buf_readline(b, linepointer, LINEBUFFERSIZE);
            if(lineStart<0){
                continue;
            }
            if (!strcmp(linepointer, separator))
            {
                newFIE->next = calloc(1, sizeof(FileIndexEntry));
                newFI->nEntries++;
                newFIE->size = sectionEnd - newFIE->seekpos;
                newFI->totalSize += newFIE->size;
                newFIE->next->nr = ++fieIndex;
                newFIE = newFIE->next;
                lineStart = buf_readline(b, linepointer, LINEBUFFERSIZE);
                newFIE->seekpos = lineStart;
                newFIE->lines += 1;
                sectionEnd = buf_where(b);
            }
            else
            {
                newFIE->lines += 2;
                sectionEnd = buf_where(b);
            }
        }
        else
        {
            newFIE->lines += 1;
            sectionEnd = buf_where(b);
        }
    }
    buf_dispose(b);
    return newFI;
}

void fi_dispose(FileIndex *fi)
{
    FileIndexEntry *cfi = fi->entries;
    FileIndexEntry *pfi = NULL;

    while (cfi != NULL)
    {
        pfi = cfi;
        cfi = cfi->next;
        free(pfi);
    }

    free(fi);
}

FileIndexEntry *fi_find(FileIndex *fi, int n)
{
    FileIndexEntry *found = fi->entries;
    while (--n)
    {
        if (found->next)
        {
            found = found->next;
        }
        else
        {
            return NULL;
        }
    }
    return found;
}

int fi_compactify(FileIndex *fi)
{
    const char *oldfilename = fi->filepath;
    const char *tempfilename = "tmp.txt";
    FileIndexEntry *fie = fi->entries;
    LineBuffer *b = buf_new(open(fi->filepath, O_RDONLY), "\n");
    int newfd = open(tempfilename, O_WRONLY | O_CREAT | O_TRUNC, 0640);
    int linemax = 1024;
    char *line = malloc(sizeof(char) * linemax);
    char *seperator = malloc(sizeof(char) * linemax);
    FileIndex *newFI;

    buf_readline(b, seperator, linemax);

    while (fie)
    {
        if(fie->del_flag){
            fie = fie->next;
            continue;
        }
        write(newfd, seperator, strlen(seperator));
        write(newfd, b->linesep, b->lineseplen);
        buf_seek(b,fie->seekpos);

        
        for(; fie->lines > 0; fie->lines--)
        {
            buf_readline(b,line,linemax);
            write(newfd,line,strlen(line));
            write(newfd, b->linesep, b->lineseplen);
        }

        write(newfd,b->linesep,b->lineseplen);
        fie = fie->next;
        
    }
    free(line);
    free(seperator);
    fi_dispose(fi);
    rename(tempfilename,oldfilename);
    newFI = fi_new(oldfilename,seperator);
    *fi = *newFI;
    return 0;
}

/**
int main(int argc, char const *argv[])
{
    FileIndex *newFI;
    FileIndexEntry *foundFIE;

    char *path = "first.txt";
    newFI = fi_new(path, "/SECTION");
    foundFIE = fi_find(newFI, 1);
    foundFIE->del_flag = 1;
    fi_compactify(newFI);
    fi_dispose(newFI);
    return 0;
}
**/


linebuffer.c

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "linebuffer.h"

LineBuffer *buf_new(int descriptor, const char *linesep)
{
    LineBuffer *buffer = calloc(1, sizeof(LineBuffer));
    buffer->descriptor = descriptor;
    buffer->linesep = linesep;
    buffer->lineseplen = strlen(linesep);

    return buffer;
}

void buf_dispose(LineBuffer *b)
{
    free(b);
}

void pump(LineBuffer *b)
{
    b->here = 0;
    b->end = read(b->descriptor, b->buffer, LINEBUFFERSIZE);
    b->bytesread += b->end;
}

int buf_where(LineBuffer *b)
{
    return b->bytesread - b->end + b->here;
}

int buf_readline(LineBuffer *b, char *line, int linemax)
{
    char *lineString = malloc(linemax);
    int linestart = buf_where(b);
    char *sep;
    int index = 0;

    lineString[0] = 0;
    lineString[linemax] = 0;

    while (index < linemax)
    {
        if (b->end == b->here)
        {
            pump(b);
            if (b->end == 0)
            {
                if (lineString[0] != 0)
                {
                    lineString[index] = 0;
                    strcpy(line, lineString);
                    free(lineString);
                    return linestart;
                }
                else
                {
                    free(lineString);
                    return -1;
                }
            }
        }

        lineString[index] = b->buffer[b->here++];
        sep = strstr(lineString, b->linesep);

        if (sep)
        {
            *sep = 0;
            break;
        }

        index++;
    }
    strcpy(line, lineString);
    free(lineString);
    return linestart;
}

int buf_seek(LineBuffer *b, int seekpos)
{
    int seek = lseek(b->descriptor, seekpos, SEEK_SET);

    if (seek >= 0)
    {
        b->bytesread = seekpos;
        b->here = 0;
        b->end = 0;
    }

    return seek;
}

/**
int main(int argc, char const *argv[])
{
    const char *sep = "\n";
    char *path = "first.txt";
    int linemax = 1024;
    char *line = malloc(sizeof(char) * linemax);
    int fd = open(path, O_RDONLY);
    LineBuffer *lb = buf_new(fd, sep);

    while (buf_readline(lb, line, 1024) >= 0)
    {
        printf("%s\n", line);
    }

    return 0;
}
 */
