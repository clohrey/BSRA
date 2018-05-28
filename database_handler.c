#define _POSIX_C_SOURCE 200809L
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include "database.h"
 
int db_list(
    const char *path,
    int out_fd,
    int (*filter)(DBRecord *rec, const void *data),
    const void *data
    ){
    int open_fd;
    int read_status;
    int db_listing_amount = 0;
    DBRecord temp_record;
 
    if((open_fd = open(path, O_RDONLY)) < 0){
        perror("Fehler beim Oeffnen der Datenbank");
        exit(1);
    }
   
    for(;;){
        if((read_status = read(open_fd, &(temp_record), sizeof(DBRecord))) > 0){
            if(filter != NULL){
                if(filter(&temp_record, data)){
                    dprintf(out_fd, "%-15s|", temp_record.key);
                    dprintf(out_fd, "%-15s|", temp_record.cat);
                    dprintf(out_fd, "%s", temp_record.value);
                    dprintf(out_fd, "\n");
                }
            }else{
                dprintf(out_fd, "%-15s|", temp_record.key);
                dprintf(out_fd, "%-15s|", temp_record.cat);
                dprintf(out_fd, "%s", temp_record.value);
                dprintf(out_fd, "\n");
            }
            db_listing_amount++;
        }
       
        if(read_status == 0){
            printf("\nDBListing: %2d\n\n", db_listing_amount);
            close(open_fd);
            return db_listing_amount;
        }
       
        if(read_status < 0){
            perror("Fehler beim Lesen der Datenbank");
            close(open_fd);
            return read_status;
        }
    }
    close(open_fd);
    return db_listing_amount;
}
 
int db_search(const char *filepath,  int start, DBRecord *record){
    DBRecord temp_record;
    int open_fd, read_status, list_index=start;
   
    if((open_fd = open(filepath, O_RDONLY)) < 0){
        perror("Fehler beim Oeffnen der Datenbank");
        close(open_fd);
        return -42;
    }
    lseek(open_fd, sizeof(DBRecord) * start, SEEK_SET);
   
    for(;;){
        if((read_status = read(open_fd, &temp_record, sizeof(DBRecord))) > 0){
            if((strcmp(temp_record.key, record->key)
            && strcmp(temp_record.cat, record->cat))
            || (strcmp(temp_record.key, "")
            && strcmp(temp_record.cat, record->cat))
            || (strcmp(temp_record.key, record->key)
            && strcmp(temp_record.cat, ""))
            || (strcmp(temp_record.key, "")
            && strcmp(temp_record.cat, ""))){
                strcpy(record->value, temp_record.value);
                return list_index;
            }
            list_index++;
        }
       
        if(read_status == 0){
            perror("Keinen entsprechenden Datenbankeintrag gefunden");
            close(open_fd);
            return -1;
        }
       
        if(read_status < 0){
            perror("Fehler beim Lesen der Datenbank");
            close(open_fd);
            return -42;
        }
    }
    return -42;
}
 
int db_get(const char *filepath, int index, DBRecord *result){
    DBRecord temp_record;
    int open_fd, read_status, seek_status;
   
    if((open_fd = open(filepath, O_RDONLY)) < 0){
        perror("Fehler beim Oeffnen der Datenbank");
        return -1;
    }
   
    if((read_status = read(open_fd, &(temp_record), sizeof(DBRecord))) > 0){
        seek_status = lseek(open_fd, sizeof(DBRecord) * index, SEEK_SET);
        if(seek_status > 0){
            *result = temp_record;
            close(open_fd);
            return 0;
        }
    }
    return -1;
}
 
int db_put(const char *filepath, int index, const DBRecord *record){
    int open_fd, write_fd, end_fd, file_size, seek_status;
    DBRecord temp_record;
   
    if((open_fd = open(filepath, O_RDWR)) < 0){
        perror("Fehler beim Oeffnen der Datenbank");
        return -1;
    }
    end_fd = open_fd;
    file_size = lseek(end_fd, 0, SEEK_END) / sizeof(DBRecord);
    seek_status = lseek(open_fd, sizeof(DBRecord) * index, SEEK_SET);
   
    if(open_fd > 0 && seek_status != -1){
        if(0 <= index && file_size < end_fd){
            read(open_fd, &temp_record, sizeof(DBRecord));
            strcpy(temp_record.key, record->key);
            strcpy(temp_record.cat, record->cat);
            strcpy(temp_record.value, record->value);
            write(open_fd, &temp_record, sizeof(DBRecord));
            close(open_fd);
        }else{
            close(open_fd);
            write_fd = open(filepath, O_WRONLY | O_APPEND);
            write(write_fd, record, sizeof(DBRecord));
            close(write_fd);
        }
    }else{
        return -1;
    }
    return 0;
}
 
int db_update(const char *filepath, const DBRecord *new){
    int open_fd, read_status, index=0;
    DBRecord temp_record;
   
    if((open_fd = open(filepath, O_RDWR)) < 0){
        perror("Fehler beim Oeffnen der Datenbank");
        return -1;
    }
   
    for(;;){
        if((read_status = read(open_fd, &temp_record, sizeof(DBRecord))) == 0){
            write(open_fd, new, sizeof(DBRecord));
            return index;
        }else{
            if(strcmp(temp_record.key, new->key)
            && strcmp(temp_record.cat, new->cat)){
                lseek(open_fd, sizeof(DBRecord) * index, SEEK_SET);
                write(open_fd, new, sizeof(DBRecord));
                return index;
            }
        }
        index++;
    }
    return 0;
}
 
int db_del(const char *filepath, int index){
    DBRecord temp_record;
    int open_fd, temp_fd, read_status, i=0;
    int temp_data = open("temp_data", O_WRONLY | O_CREAT, 0777);
   
    if((open_fd = open(filepath, O_RDWR)) < 0){
        perror("Fehler beim Oeffnen der Datenbank");
        return -1;
    }
   
    temp_fd = open_fd;
   
    if(index >= 0 && index < (lseek(temp_fd, 0, SEEK_END) / sizeof(DBRecord))){
        return -1;
    }
   
    while((read_status = read(temp_fd, &temp_record, sizeof(DBRecord))) != 0){
        if(index == i){
            read(temp_fd, &temp_record + sizeof(DBRecord), sizeof(DBRecord));
            write(temp_data, &temp_record, sizeof(DBRecord));
        }else{
            write(temp_data, &temp_record, sizeof(DBRecord));
            i++;
        }
    }
    rename("temp_data", "data");
    return 0;
}
