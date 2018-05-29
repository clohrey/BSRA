#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include "database.h"

int filter_by_name(DBRecord *rec, const void *data){
	if(strcmp(rec->key, data) == 0){
		return 1;
	}
	return 0;
}

int filter_by_substring(DBRecord *rec, const void *data){
	if(strstr(rec->key, data) != NULL || strstr(rec->cat, data) != NULL){
		return 1;
	}
	return 0;
}

int main(int argc, char *argv[]){
    int start=0, index=0, open_fd, out_fd=0;
    int (*function_ptr)(DBRecord *rec, const void *data);
    const char *path = "data";
    DBRecord temp_record;

    DBRecord data1 = {"Peter", "Nemo", "A2"};
    DBRecord data2 = {"Hans", "Nemo", "A3"};
    DBRecord data3 = {"Franz", "Dori", "A1"};
    DBRecord data4 = {"Peter", "Dora", "A7"};
    DBRecord data5 = {"Peter", "Biffel", "A1"};
    DBRecord data6 = {"Hans", "Nemoo", "A1"};

    if((open_fd = open("data", O_WRONLY | O_CREAT | O_TRUNC | O_APPEND, 0640)) < 0){
        fprintf(stderr, "\nFehler beim Oeffnen der Datei");
        exit(1);
    }

    write(open_fd, &data1, sizeof(DBRecord));
    write(open_fd, &data2, sizeof(DBRecord));
    write(open_fd, &data3, sizeof(DBRecord));
    write(open_fd, &data4, sizeof(DBRecord));
    write(open_fd, &data5, sizeof(DBRecord));
    write(open_fd, &data6, sizeof(DBRecord));
	close(open_fd);

    if(strcmp(argv[1], "list") == 0){
		function_ptr = &filter_by_name;
		if(argc < 3){
			db_list(path, out_fd, NULL, NULL);
		}else{
			db_list(path, out_fd, function_ptr, argv[2]);
		}
		return 0;
    }

    if(strcmp(argv[1], "search") == 0){
		function_ptr = &filter_by_substring;
		db_list(path, out_fd, function_ptr, argv[2]);
    }

    if(strcmp(argv[1], "add") == 0){
		strcpy(temp_record.key, argv[2]);
		strcpy(temp_record.cat, argv[3]);
		if(argc < 5){
			strcpy(temp_record.value, "Gruppenaquarium");
			db_update(path, &temp_record);
			db_list(path, out_fd, NULL, NULL);
			return 0;
		}else{
			strcpy(temp_record.value, argv[4]);
		}
		db_update(path, &temp_record);
		db_list(path, out_fd, NULL, NULL);
		return 0;
    }

    if(strcmp(argv[1], "update") == 0){
        strcpy(temp_record.key, argv[2]);
        index = db_search(path, index, &temp_record);
        
        while(index != -1){
			index = db_search(path, start, &temp_record);
			strncpy(temp_record.value, "Loftiquarium", sizeof("Loftiquarium"));
			db_update(path, &temp_record);
			start++;
		}
		db_list(path, out_fd, NULL, NULL);
		return 0;
    }

    /* Baustelle */
    if(strcmp(argv[1], "delete") == 0){
		index = db_search(path, start, &temp_record);
        if(argc > 2){
			while(index != -1){
				index = db_del(path, index);
			}
        }else{
			while(){
				
			}
        }
        db_list(path, out_fd, NULL, NULL);
    }
    
    return 0;
}
