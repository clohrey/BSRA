#include <stdio.h>
#include "linebuffer.h"

LineBuffer *buf_new(int descriptor, const char *linesep)
{
    LineBuffer *buffer = calloc(1, sizeof(LineBuffer));
    buffer->descriptor = descriptor;
    buffer->linesep = linesep;
    buffer->lineseplen = strlen(linesep);
    return buffer;
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
