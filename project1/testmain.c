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
int getsize(int fp);
int debugvar = 0;
int firstpass = 0;
int main(int argc, char *argv[])
{
    // variable declarations
    int fp; // open(const char* path, int flag, [, int mode])

    if (argc == 3)
    {
        if (strcmp(argv[1], "-d") == 0)
        {
            printf("Debugging mode is on \n");
            debugvar = 1;
        }
        fp = open(argv[2], O_RDONLY);
    }
    else if (argc == 2)
    {
        fp = open(argv[1], O_RDONLY);
    }
    else
    {
        printf("Too many arguments");
        exit(1);
    }

    // Check if the file can be opened or if its can't be seek'
    if (fp == -1)
    {
        perror("Opening: "); // perror(const char *s), gives you a string b4 error msg
        exit(1);
    }
    else if (lseek(fp, 0, SEEK_CUR) == -1)
    { // check if file is able to seek, lseek(int fp, offset, whence)
        perror("Cant Seek: ");
        exit(1);
    }

    int current1 = 0, current2 = 0, linesize1 = 0, linesize2 = 0, tmp;
    char ch1, ch2;
    while(1){
        // getting stuff from 2nd line
        linesize1 = getsize(fp);
        if(firstpass == 0){
            lseek(fp, linesize1, SEEK_SET);
            firstpass = 1;
            current1 = linesize1;
        }
        else
            lseek(fp, current1, SEEK_SET);

        read(fp, &ch1, 1);
        current1++; // keeping position
        //printf("%c", ch1);

        // First line
         if(ch1 == '\n'){
            //printf("%d, %d \n", linesize1, linesize2);
            lseek(fp, current2, SEEK_SET);
            linesize2 = getsize(fp);
            if(linesize2 == linesize1)
                continue; // they are equal no point to check
        }
        // figure out how to keep the new lines matching each other, so they start on a fresh line when needed
        lseek(fp, current2, SEEK_SET);
            
        tmp = read(fp, &ch2, 1);
        //printf("%c", ch2);

        printf("%c, %c \n", ch1, ch2);       
        // if(ch1 == ch2)
        //     printf("%c", ch2);
        
        current2++;
        if(tmp == 0)
            exit(1);

    } // end of while
    return 0;
}
// when line1 gets to a newline, the 2nd one needs to drop and go to the new line as well. Stop it from lagging behind
int getsize(int fp){
    int count = 0;
    char addy;
    while(read(fp, &addy, 1) > 0){
        if(addy != '\n')
            count++;
        else if(addy == '\n'){
            count++;
            return count;
        }
    }
    
}

/*
char addy1, addy2;
    int len1 = 0, newline = 0, pos1 = 0, pos2 = 0, pos3 = 0;
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
            read(fp,&addy2, 1);                                             // dont need to check for -1 cause its reading whats already been read
            if(debugvar == 1){
                printf("This is addy1 %c , this is addy2 %c, this is pos1 %d, this is pos2 %d \n", addy1, addy2, pos1, pos2);

            }
            if(addy2 == addy1 && debugvar == 0){
                write(1, &addy1, 1);
            }
            pos2++;
            lseek(fp, pos1, SEEK_SET);
        } // end of if(newline)
        if(addy1 == '\n'){
            newline = 1;
        }
        */