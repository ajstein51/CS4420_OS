/**
 * Name: AJ Stein
 * Class: CS4420 - Operating Systems
 * Project1
 * Description: In this project we will essentially be redoing the system cmd of unique in c while using system calls only.
 */ 
// includes
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int debugvar = 0;

int main(int argc, char *argv[]){
    // variable declarations
    int fp;                                                               // open(const char* path, int flag, [, int mode])                                
   
   if(argc == 3){
       if (strcmp(argv[1], "-d") == 0){
           printf("Debugging mode is on \n");
           debugvar = 1;
       }
       fp = open(argv[2], O_RDONLY);
   }
   else if (argc == 2){
       fp = open(argv[1], O_RDONLY);
   }
   else{
       printf("Too many arguments");
       exit(1);
   }

    // Check if the file can be opened or if its can't be seek'
    if (fp == -1){
        perror("Opening: ");                                            // perror(const char *s), gives you a string b4 error msg
        exit(1);
    }
    else if (lseek(fp, 0, SEEK_CUR) == -1){                              // check if file is able to seek, lseek(int fp, offset, whence)
        perror("Cant Seek: ");
        exit(1);
    }

    char addy1, addy2;
    int len1 = 0, newline = 0, pos1 = 0, pos2 = 0;
    while(1){
        len1 = read(fp, &addy1, 1);                                          // read(int fd, *buff, count)
        if(len1 == -1){                                                      // error in reading
            perror("Reading Error: ");
            exit(1);
        }
        if(len1 == 0){                                                       // leaving the while loop
            if (close(fp) == -1){                                           // int close(int fd), fd = file descriptor
                perror("Closing: ");                                        
                exit(1);
            }
            printf("\n");
            exit(0);                                                        // EOF, empty happy 
        } 

        pos1++;
        if (newline == 1){

            lseek(fp, pos2, SEEK_SET);
            int len2 = read(fp,&addy2, 1);
            if(debugvar == 1)
                printf("This is addy1 %c , this is addy2 %c \n", addy1, addy2);
            if(len2 != -1 && addy2 == addy1){
                pos2++;
                write(1, &addy1, 1);
            }
            else{
                newline = 0;
            }
            lseek(fp, pos1, SEEK_SET);
        } // end of if(newline)
        if(addy1 == '\n'){
            newline = 1;
        }
        
        
        //write(1, &addy1, 1);
    
    
    } // end of while


    // shouldnt get down here
    return 0;
}
/* example of printing out the file twice
lseek(fp, 0, SEEK_CUR);
        write(1, &addy1, 1);
        if(len1 == 0){
            lseek(fp, 0, SEEK_SET);
            pos1++;
        }
        if(pos1 == 2){
            exit(0);
        }
*/