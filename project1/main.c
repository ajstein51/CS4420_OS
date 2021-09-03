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

// functions:
int getsize(int fp);
int checkmatch(int fp);

// Global var
int debugvar = 0;
int line1pos = 0, line2pos = 0;

int main(int argc, char *argv[]){
    // variable declarations
    int fp;
    
    // Open and parse arguments
//*************************************************************************
    if(argc == 3){                                                              // open(const char* path, int flag, [, int mode])
        if (strcmp(argv[1], "-d") == 0)
        {
            printf("Debugging mode is on \n");
            debugvar = 1;
        }
        fp = open(argv[2], O_RDONLY);
    }
    else if (argc == 2)
       fp = open(argv[1], O_RDONLY);
    else{
       printf("Too many arguments");
       exit(1);
    }

    if (fp == -1){                                                              // Check if the file can be opened or if its can't be seek'
        perror("Opening: ");                                                    // perror(const char *s), gives you a string b4 error msg
        exit(1);
    }
    else if (lseek(fp, 0, SEEK_CUR) == -1){                                     // check if file is able to seek, lseek(int fp, offset, whence)
        perror("Cant Seek: ");
        exit(1);
    }
//*************************************************************************

    // var declarations
    int firstpass = 0, size_of_second_line = 0, size_of_first_line = 0, match = 0;
    while(1){
//************************************************************************* First pass through loop
        if(firstpass == 0){                                                     // first pass though we set a line to the 2nd line to get started

            int check1, check2;
            size_of_first_line = getsize(fp);                                   // get the size of the first line
            check1 = lseek(fp, size_of_first_line + 1, SEEK_SET);               // go to the next line (2nd line), we do +1 cause we return at \n
            
            if(check1 == -1){                                                   // error check
                perror("Error on seek 11: ");
                exit(1);
            }

            size_of_second_line = getsize(fp);                                  // now we get the size of 2nd line, if they arent same size we can move on

            line2pos = size_of_first_line;                                      // make the line2pos = to the size + 1 of the first line

            if(debugvar == 1)
                printf("\n%d,  %d, %d  \n", size_of_first_line, size_of_second_line, line2pos);

            check2 = lseek(fp, 0, SEEK_SET);                                    // go to the start

            if (check2 == -1)                                                   // error check2
            {
                perror("Error on seek 11: ");
                exit(1);
            }

            if (size_of_first_line == size_of_second_line)                     // size are the same, so check bytes now
                match = checkmatch(fp);
            
            firstpass = 1;
        } // end of if firstpass
//*************************************************************************
//************************************************************************* Check match, print if so
        if (match == 1)
        {
            int stop = 0, checkread;
            char tmp;
            while(stop == 0){
                checkread = read(fp, &tmp, 1);
                printf("%c", tmp);
                
                if(tmp == '\n')                                                 // new line stop printing out
                    stop = 1;
                else if (checkread == -1)                                       // error check
                { 
                    perror("match perror: ");
                    exit(1);
                }
                else if (checkread == 0)                                        // error check
                { 
                    //printf("\nEOF\n");
                    exit(0);
                }
            }                                                                   // end of while
            match = 0;                                                          // reset match
        }                                                                       // end of if match == 1
//************************************************************************* dont match, move forward on line1 and line2
        else{ 
            // int check1, check2;
            // check1 = lseek(fp, size_of_first_line + line1pos, SEEK_SET);        // go to 2nd

            // if (check1 == -1)                                                   // error check
            // { 
            //     perror("Else Problem: ");
            //     exit(1);
            // }
            // size_of_first_line = getsize(fp) + 1;

            // check2 = lseek(fp, size_of_second_line + line2pos, SEEK_SET);       // get 2nd to 3rd
            // line2pos += size_of_second_line + 1;                                // should go pos of 3rd line
            // size_of_second_line = getsize(fp);

            // if (check2 == -1)                                                   // error check
            // {
            //     perror("Else Problem: ");
            //     exit(1);
            // }

            // if(debugvar == 1)
            //     printf("\n%d,  %d  \n", size_of_first_line, size_of_second_line);

            // if (size_of_first_line == size_of_second_line)
            //     match = checkmatch(fp);

            int check1, check2;
            check1 = lseek(fp, size_of_first_line, SEEK_SET);                   // go to 2nd

            if (check1 == -1)                                                   // error check
            {
                perror("Else Problem: ");
                exit(1);
            }
            line1pos += size_of_first_line - 1;

            size_of_first_line = getsize(fp);

            check2 = lseek(fp, size_of_second_line + line2pos, SEEK_SET);       // get 2nd to 3rd
            line2pos += size_of_second_line;                                    // should go pos of 3rd line
            size_of_second_line = getsize(fp);

            if (check2 == -1)                                                   // error check
            {
                perror("Else Problem: ");
                exit(1);
            }

            //if(debugvar == 1)
                //printf("\n%d,  %d  \n", size_of_first_line, size_of_second_line);

            if (size_of_first_line == size_of_second_line)
                match = checkmatch(fp);

        } // end of else
//************************************************************************* 
    } // end of while
    // shouldnt get down here
    return 0;
} // To Do: output with write() // end of main

// functions:
//************************************************************************* Get the size of the line
int getsize(int fp){
    int count = 0, readcheck;
    char ch;

    while(1){
        readcheck = read(fp, &ch, 1);
    
        if (readcheck == -1)                                                    // error check
        { 
            perror("getsize problem: ");
            exit(1);
        }
        else if (readcheck == 0){
            if(debugvar == 1)
                //printf("EOF");
            exit(0);
        }

        else if (ch == '\n'){                                                   // if we got to the new line, send the count back. Remember itll be +1 to the first line of next row
            if(debugvar == 1)
                printf("This is in getsize: %d\n", count);
            return count;
        }
        count++;
    }
}
//************************************************************************* 
//************************************************************************* Check if the lines match
int checkmatch(int fp){
    int readcheck1, readcheck2, ret = 0, lseekcheck1, lseekcheck2;
    char ch1, ch2;
    //if(debugvar == 1)
        printf("%d, %d  \n", line1pos, line2pos);
    while(ret == 0){
        // seek to first line, this is new
        lseek(fp, line1pos, SEEK_SET);
        readcheck1 = read(fp, &ch1, 1);                                     // read from first line

        // move to the 2nd line
        lseekcheck1 = lseek(fp, line2pos, SEEK_SET);

        if (lseekcheck1 == -1)                                               // error check
        {
            perror("Else Problem: ");
            exit(1);
        }

        readcheck2 = read(fp, &ch2, 1);                                     // read from 2nd line

        if (ch2 != ch1)                                                     // they dont match, return -1
            ret = 1;

        if (ch1 == '\n')                                                    // they went through the whole sentence and match, return 1
            ret = -1;
        
        if (readcheck1 == -1 || readcheck2 == -1){                          // Error check 
            perror("Readcheck1: ");
            exit(1);
        }
        // increment pos
        line1pos++, line2pos++; // new
    } // end of while
    return ret;
} // end of function
//*************************************************************************
