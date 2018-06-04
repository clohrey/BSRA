#include <stdio.h>
#include "linebuffer.h"

LineBuffer *buf_new(int descriptor, const char *linesep){
	LineBuffer lb_temp;
	LineBuffer *lbptr = &lb_temp;
	
	return lbptr;
}

void buf_dispose(LineBuffer *b){
	
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
