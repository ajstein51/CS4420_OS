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
int getsize(int linepos);
int checkmatch(int line1pos, int line2pos);

// Global var
int debugvar = 0, fp;

int main(int argc, char *argv[])
{

    // Open and parse arguments
    //*************************************************************************
    if (argc == 3)
    { // open(const char* path, int flag, [, int mode])
        if (strcmp(argv[1], "-d") == 0)
        {
            printf("Debugging mode is on \n");
            debugvar = 1;
        }
        fp = open(argv[2], O_RDONLY);
    }
    else if (argc == 2)
        fp = open(argv[1], O_RDONLY);
    else
    {
        printf("Too many arguments");
        exit(1);
    }
    if (fp == -1)
    {                        // Check if the file can be opened or if its can't be seek'
        perror("Opening: "); // perror(const char *s), gives you a string b4 error msg
        exit(1);
    }
    else if (lseek(fp, 0, SEEK_CUR) == -1)
    { // check if file is able to seek, lseek(int fp, offset, whence)
        perror("Cant Seek: ");
        exit(1);
    }
    //*************************************************************************

    // var declarations
    int size_of_second_line = 0, size_of_first_line = 0, line1pos = 0, line2pos = 0, match = 0;
    while (1)
    {
        // its the first two lines in the file, so we move one line down
        if (match == 0)
        {
            // gets the size of the first line
            size_of_first_line = getsize(line1pos);
            //printf("\nThis is size of first: %d \n", size_of_first_line);

            line2pos = size_of_first_line + 1; // puts the 2nd lines pos on the start of the 2nd line
            //printf("\nThis is line2 %d \n", line2pos);

            size_of_second_line = getsize(line2pos); // get size will set the lseek to where ever the argument is

            //printf("\nThis is size of 2nd:  %d\n", size_of_second_line);

            // if the line sizes match then they are possibly the same, if they are then we check
            if (size_of_second_line == size_of_first_line)
            {
                match = checkmatch(line1pos, line2pos);
                lseek(fp, line1pos, SEEK_SET); // set pos at the line1pos
            }
            else if (size_of_first_line != size_of_second_line)
                match = 1;

        } // end of if
        if (match == -1)
        {                                  // the lines match, print out one of them, then move the lines down 1
            lseek(fp, line1pos, SEEK_SET); // go to 2nd line and print that out

            char tmp;
            int stopvar = 0, readcheck;
            while (stopvar == 0)
            {
                readcheck = read(fp, &tmp, 1);

                // error check
                if (readcheck == -1)
                {
                    perror("Readcheck1: ");
                    exit(1);
                }
                else if (readcheck == 0)
                {
                    if (debugvar == 1)
                        printf("\nEOF5\n");
                    exit(0);
                }
                if (tmp == '\n')
                {
                    stopvar = 1;
                }
                write(1, &tmp, 1);
            }
            // if we got here then the line is at the \n so we move lines down 1
            // swap first line info with 2nd line info
            size_of_first_line = size_of_second_line;
            line1pos = line2pos;

            // move line1 down another line
            line1pos += size_of_first_line + 1;
            size_of_first_line = getsize(line1pos);

            // move line2 info down 2 rows
            line2pos += size_of_second_line + 1;
            size_of_second_line = getsize(line2pos);

            line2pos += size_of_second_line + 1;
            size_of_second_line = getsize(line2pos);

            if (size_of_second_line == size_of_first_line)
            {
                match = checkmatch(line1pos, line2pos);
                lseek(fp, line1pos, SEEK_SET); // set pos at the line1pos
            }
            if (size_of_second_line != size_of_first_line)
                match = 1;
        }
        if (match == 1)
        { // the lines dont match print out first
            lseek(fp, line1pos, SEEK_SET);

            // print out first line
            char tmp;
            int stopvar = 0, readcheck;
            while (stopvar == 0)
            {
                readcheck = read(fp, &tmp, 1);

                // error check
                if (readcheck == -1)
                {
                    perror("Readcheck1: ");
                    exit(1);
                }
                else if (readcheck == 0)
                {
                    if (debugvar == 1)
                        printf("\nEOF6\n");
                    exit(0);
                }
                if (tmp == '\n')
                {
                    stopvar = 1;
                }
                write(1, &tmp, 1);
            }

            // move lines first to 3rd, 2nd to 4th
            size_of_first_line = size_of_second_line;
            line1pos = line2pos;

            // line1pos += size_of_first_line + 1;
            // size_of_first_line = getsize(line1pos);

            // other line info
            line2pos += size_of_second_line + 1;
            size_of_second_line = getsize(line2pos);

            // line2pos += size_of_second_line + 1;
            // size_of_second_line = getsize(line2pos);

            if (size_of_second_line == size_of_first_line)
            {
                match = checkmatch(line1pos, line2pos);
                lseek(fp, line1pos, SEEK_SET); // set pos at the line1pos
            }
            if (size_of_second_line != size_of_first_line)
                match = 1;
        }

    } // end of while
    // shouldnt get down here
    return 0;
} // end of main

// functions
int getsize(int linepos)
{
    char tmp;
    int count = 0, stopvar = 0;
    lseek(fp, linepos, SEEK_SET);
    while (stopvar == 0)
    {
        read(fp, &tmp, 1); // read through
        if (tmp == '\n')   // its a new line so we stop
            stopvar = 1;
        count++;
    }                             // end of loop
    lseek(fp, linepos, SEEK_SET); // set the position back to what is was
    return count - 1;
}

int checkmatch(int line1pos, int line2pos)
{
    char ch1, ch2;
    int stopvar = 0, readcheck1, readcheck2, lseekcheck1, lseekcheck2, linecounter = 0;

    while (stopvar == 0)
    {
        lseekcheck1 = lseek(fp, line1pos, SEEK_SET); // seek to first line

        // read the char from the first line
        readcheck1 = read(fp, &ch1, 1);

        // seek to 2nd line and read
        lseekcheck2 = lseek(fp, line2pos, SEEK_SET);
        readcheck2 = read(fp, &ch2, 1);

        if (debugvar == 1)
            printf("\nThis is Ch1 and Ch2  %c    %c \n", ch1, ch2);

        // error check
        if (readcheck1 == -1)
        {
            perror("Readcheck1: ");
            exit(1);
        }
        else if (readcheck1 == 0)
        {
            if (debugvar == 1)
                printf("\nEOF1\n");
        }
        if (readcheck2 == -1)
        {
            perror("Readcheck1: ");
            exit(1);
        }
        else if (readcheck2 == 0)
        {
            if (debugvar == 1)
                printf("\nEOF2\n");
        }
        if (lseekcheck1 == -1)
        {
            perror("Readcheck1: ");
            exit(1);
        }
        else if (lseekcheck1 == 0)
        {
            if (debugvar == 1)
                printf("\nEOF3\n");
        }
        if (lseekcheck2 == -1)
        {
            perror("Readcheck1: ");
            exit(1);
        }
        else if (lseekcheck2 == 0)
        {
            if (debugvar == 1)
                printf("\nEOF4\n");
        }

        if (ch1 == '\n')
        {
            return -1; // they are the same, print out 1 line only
        }
        else if (ch1 != ch2)
        {
            return 1; // they arent the same, print first line and move lines down
        }

        line1pos++, line2pos++; // increment
    }
    if (debugvar == 1)
        printf("\nWE GOT DOWN HERE\n");
}

/*
Rules
if they arent the same print the line out
if they are the same print one of the lines and move down
if they arent the same size its unique
if they are the same size check if they are the same
To Do:
- error checks for lseek, read, write
- Debug info
- if one of the lines is 0 then print the other to fix EOF issue?
*/