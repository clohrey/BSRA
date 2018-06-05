#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linebuffer.h"

LineBuffer *buf_new(int descriptor, const char *linesep)
{
    LineBuffer *bptr = calloc(1, sizeof(LineBuffer));
    bptr->descriptor = descriptor;
    bptr->linesep = linesep;
    bptr->lineseplen = strlen(linesep);
    return bptr;
}

void buf_dispose(LineBuffer *b){
	free(b);
}

int buf_readline(LineBuffer *b, char *line, int linemax){
	
	return 0;
}

int buf_where(LineBuffer *b){
	
	return 0;
}

int buf_seek(LineBuffer *b, int seekpos){
	
	return 0;
}
