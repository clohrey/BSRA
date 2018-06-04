#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include "dialog.h"

int validate_onoff(DialogRec *d){
	return !strcmp(d->param, "on") || !strcmp(d->param, "off");
}

int validate_noparam(DialogRec *d){
	if(strlen(d->param) == 0){
		return 1;
	}
	return 0;
}

DialogRec *findDialogRec(char *command, DialogRec dialogspec[]){
	DialogRec *dia_ptr = NULL;
	char *cp = command;
	int index = 0;
	int length = 0;
	
	while(isalpha(*cp)){
		length++;
		cp++;
	}
	
	while(strcmp(dialogspec[index].command, "") != 0){
		/*
		printf("%s\n", dialogspec[index].command);
		printf("%s\n", command);
		printf("%d\n", length);
		*/
		
		if(strncasecmp(command, dialogspec[index].command, length) == 0
		&& length == strlen(dialogspec[index].command)){
			
			dia_ptr = &dialogspec[index];
			return dia_ptr;
		}
		index++; 
	}
	return NULL;
}

ProlResult processLine(char line[LINEMAX], int state, DialogRec dialogrec[]){
	ProlResult res;
	DialogRec *dia_ptr = NULL;
	char *cp = line;
	int index = 0;
	
	dia_ptr = findDialogRec(line, dialogrec);
	
	if(dia_ptr == NULL){
		res.failed = 1;
		res.dialogrec = NULL;
		strcpy(res.info, "Eine aufrichtig bedauernde Nachricht an den Aufrufer");
	}
	
	if(dia_ptr != NULL && dia_ptr->state != state){
		res.failed = 1;
		res.dialogrec = dia_ptr;
		strcpy(res.info, "Falscher State aber DialogRec gefunden");
	}
	
	if(dia_ptr != NULL && dia_ptr->state == state){
		res.dialogrec = dia_ptr;
		
		if(dia_ptr->validator == NULL){
			dia_ptr->is_valid = 1;
		}else{
			dia_ptr->is_valid = dia_ptr->validator(dia_ptr);
		}
		
		while(isalpha(*cp)){
			cp++;
		}
		if(*(cp++) != 0){
			while(*cp == ' '){
				cp++;
			}
			while(*cp != '\n' && *cp != '\r'){
				dia_ptr->param[index] = *cp;
				index++;
				cp++;
			}
		}
		
		if(res.dialogrec != NULL && res.dialogrec->is_valid != 0){
			res.failed = 0;
		}
	}
	return res;
}


int main(void){
	/* DialogRec *ptr = NULL; */
	ProlResult res;
	
	DialogRec dialogspec[] = {
		/* command,			param, state, nextstate, validator */
		{ "BEGIN", 			"", 	0, 		1, 			validate_noparam },
		{ "kuehlschrank", 	"", 	1, 		1, 			validate_onoff },
		{ "fernseher",		"", 	1, 		1, 			validate_onoff },
		{ "toaster",		"", 	1, 		1 },
		{ "END",			"", 	1, 		2, 			validate_noparam },
		{ ""}
	};
	
	/*
	ptr = findDialogRec("toast forever", dialogspec);
	
	if(ptr != NULL){
		printf("%s\n", ptr->command);
	}else{
		printf("%s\n", "NULL");
	}
	*/
	
	res = processLine("toaster     blabla\r\n", 1, dialogspec);
	printf("%s\n", res.dialogrec->command);
	printf("%s\n", res.dialogrec->param);
	
	return 0;
}



