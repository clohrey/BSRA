#include <stdio.h>
#include "database_handler.c"
 
int main(int argc, char *argv[]){
    int i, open_fd = 0;
    const char *path = "data";
   
    if((open_fd = open("data", O_WRONLY | O_CREAT | O_TRUNC | O_APPEND, 0640)) < 0){
        fprintf(stderr, "\nFehler beim Oeffnen der Datei");
        exit(1);
    }
   
    DBRecord data1 = {"Peter", "Nemo", "A2" };
    DBRecord data2 = {"Hans", "Nemo", "A3" };
    DBRecord data3 = {"Franz", "Dori", "A1" };
    DBRecord data4 = {"Peter", "Dora", "A7" };
    DBRecord data5 = {"Peter", "Biffel", "A1" };
    DBRecord data6 = {"Hans", "Nemoo", "A1" };
   
    write(open_fd, &data1, sizeof(DBRecord));
    write(open_fd, &data2, sizeof(DBRecord));
    write(open_fd, &data3, sizeof(DBRecord));
    write(open_fd, &data4, sizeof(DBRecord));
    write(open_fd, &data5, sizeof(DBRecord));
    write(open_fd, &data6, sizeof(DBRecord));
    close(open_fd);
   
    if(strcmp(argv[i], "list")){
        if(argc > 1){
            list(path, open_fd);
        }else{
            list(path, open_fd, NULL, NULL);
        }
    }
    if(strcmp(argv[i], "search")){
        search();
    }
    if(strcmp(argv[i], "add")){
        add();
    }
    if(strcmp(argv[i], "update")){
        if(argc > 3){
           
        }else{
           
        }
        update();
    }
    if(strcmp(argv[i], "delete")){
        if(argc > 2){
           
        }else{
           
        }
        delete();
    }
    return 0;
}
