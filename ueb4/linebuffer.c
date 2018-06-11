#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "linebuffer.h"

LineBuffer *buf_new(int descriptor, const char *linesep)
{
    LineBuffer *bptr = malloc(sizeof(LineBuffer));
    bptr->descriptor = descriptor;
    bptr->linesep = linesep;
    bptr->lineseplen = strlen(linesep);
    return bptr;
}

void buf_dispose(LineBuffer *b){
	free(b);
}

int buf_readline(LineBuffer *b, char *line, int linemax){
	int index = 0;
	char *seperator;
	/* calloc wegen Nullen */
	char *string = calloc(1, linemax);
	
	while(index < linemax){
		if(b->end == b->here){
			b->here = 0;
			b->end = read(b->descriptor, b->buffer, sizeof(b->buffer));
			b->bytesread += b->end;
			if(b->end == 0){
				if(string[0] != 0){
					string[index] = 0;
				}
			}
		}
		string[index] = b->buffer[b->here];
		b->here++;
        seperator = strstr(string, b->linesep);
        index++;
	}
	return 0;
}

int buf_where(LineBuffer *b){
	return b->bytesread - b->end + b->here;
}

int buf_seek(LineBuffer *b, int seekpos){
	
	return 0;
}
