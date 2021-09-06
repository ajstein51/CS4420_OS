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
int debugvar = 0, fp, stopvar = 0;

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
    int size_of_second_line = 0, size_of_first_line = 0, line1pos = 0, line2pos = 0, match = -1;
    while (1)
    {
        // its the first two lines in the file, so we move one line down
        if (match == -1)
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
                match = checkmatch(line1pos, line2pos);
            else
                match = 0; // the sizes are not the same, print the first line
        } // end of if
        if(match == 0){ // the lines are different sizes, print first line, move down 1
            int seekcheck1;
            seekcheck1 = lseek(fp, line1pos, SEEK_SET);
            // error check
            if (seekcheck1 == -1)
            {
                perror("seekcheck1: ");
                exit(1);
            }
            else if (seekcheck1 == 0)
                if (debugvar == 1)
                    printf("\nseekcheck1 == eof");

            // print out first line
            char tmp;
            int can = 0, rdcheck1, writecheck;
            while (can == 0)
            {
                rdcheck1 = read(fp, &tmp, 1);
                // error check
                if (rdcheck1 == -1)
                {
                    perror("Rdcheck1(match == 1): ");
                    exit(1);
                }
                else if (rdcheck1 == 0)
                {
                    if (debugvar == 1)
                        printf("\nrdcheck1(match == 1) == eof");
                    exit(0);
                }

                if (tmp == '\n')
                    can = 1;
                
                writecheck = write(1, &tmp, 1);
                // error check
                if (writecheck == -1)
                {
                    perror("writecheck(match == 1): ");
                    exit(1);
                }
                else if (writecheck == 0)
                {
                    if (debugvar == 1)
                        printf("\nwritecheck(match == 1) == eof");
                    exit(0);
                }
            }

            // move lines 1st to 2nd, 2nd to 3rd
            size_of_first_line = size_of_second_line;
            line1pos = line2pos;

            // other line info
            line2pos += size_of_second_line + 1;
            size_of_second_line = getsize(line2pos);

            if (size_of_second_line == size_of_first_line)
                match = checkmatch(line1pos, line2pos);
            if (size_of_second_line != size_of_first_line)
                match = 0;
        }
        if(match == 1){ // first and 2nd line are the same but no more after, move down 2 lines
            int seekcheck1;
            seekcheck1 = lseek(fp, line1pos, SEEK_SET); // go to line one
            // error check
            if (seekcheck1 == -1)
            {
                perror("seekcheck1: ");
                exit(1);
            }
            else if (seekcheck1 == 0)
                if (debugvar == 1)
                    printf("\nseekcheck1 == eof"); 

            char tmp;
            int whilestop = 0, rdcheck1, writecheck;
            while(whilestop == 0){
                rdcheck1 = read(fp, &tmp, 1);
                
                // error check
                if (rdcheck1 == -1)
                {
                    perror("Rdcheck1(match == 1): ");
                    exit(1);
                }
                else if (rdcheck1 == 0)
                {
                    if (debugvar == 1)
                        printf("\nrdcheck1(match == 1) == eof");
                    exit(0);
                }

                if(tmp == '\n')
                    whilestop = 1;

                writecheck = write(1, &tmp, 1);
                // error check
                if (writecheck == -1)
                {
                    perror("writecheck(match == 1): ");
                    exit(1);
                }
                else if (writecheck == 0)
                {
                    if (debugvar == 1)
                        printf("\nwritecheck(match == 1) == eof");
                    exit(0);
                }
            } 

            // move lines down 2
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

            // check match
            if (size_of_second_line == size_of_first_line)
                match = checkmatch(line1pos, line2pos);
            else
                match = 0; // the sizes are not the same, print the first line
        } // end of if
        else if(match >= 2){ // more than 2 lines are the same, prints first and go down match size
            int seekcheck1;
            seekcheck1 = lseek(fp, line1pos, SEEK_SET); // go to line one
            // error check
            if (seekcheck1 == -1)
            {
                perror("seekcheck1: ");
                exit(1);
            }
            else if (seekcheck1 == 0)
                if (debugvar == 1)
                    printf("\nseekcheck1 == eof");

            char tmp;
            int whilestop = 0, rdcheck1, writecheck;
            while (whilestop == 0)
            {
                rdcheck1 = read(fp, &tmp, 1);

                // error check
                if (rdcheck1 == -1)
                {
                    perror("Rdcheck1(match >= 2): ");
                    exit(1);
                }
                else if (rdcheck1 == 0)
                {
                    if (debugvar == 1)
                        printf("\nrdcheck1(match >= 2) == eof");
                    exit(0);
                }


                if (tmp == '\n')
                    whilestop = 1;

                writecheck = write(1, &tmp, 1);
                // error check
                if (writecheck == -1)
                {
                    perror("writecheck(match == 1): ");
                    exit(1);
                }
                else if (writecheck == 0)
                {
                    if (debugvar == 1)
                        printf("\nwritecheck(match == 1) == eof");
                    exit(0);
                }
            }

            // move first line to match size
            int tmpcount = 0;
            while(tmpcount <= match){
                line1pos += size_of_first_line + 1;
                size_of_first_line = getsize(line1pos);
                tmpcount++;
            } // should put us at the position of match on the line

            // move 2nd line to match + 1 size
            int tmpcount1 = 0;
            while (tmpcount1 <= match)
            {
                line2pos += size_of_second_line + 1;
                size_of_second_line = getsize(line2pos);
                tmpcount1++;
            }
            if(debugvar == 1)
                printf("\nline1  %d    line2 %d  size1  %d   size2  %d \n", line1pos, line2pos, size_of_first_line, size_of_second_line);
            

            // check match
            if (size_of_second_line == size_of_first_line)
                match = checkmatch(line1pos, line2pos);
            else
                match = 0; // the sizes are not the same, print the first line
            if (debugvar == 1)
                printf("\nTHIS IS MATCH %d \n", match);
        } // end of else
    } // end of while

    // shouldnt get down here
    return 0;
} // end of main

// functions
int getsize(int linepos)
{
    char tmp;
    int count = 0, can = 0, rdcheck1, seekcheck1;
    seekcheck1 = lseek(fp, linepos, SEEK_SET);

    // error check
    if (seekcheck1 == -1)
    {
        perror("seekcheck1: ");
        exit(1);
    }
    else if (seekcheck1 == 0)
        if (debugvar == 1)
            printf("\nseekcheck1 == eof");

    while (can == 0)
    {
        rdcheck1 = read(fp, &tmp, 1); // read through

        // error check
        if (rdcheck1 == -1)
        {
            perror("Rdcheck1: ");
            exit(1);
        }
        else if(rdcheck1 == 0)
            if(debugvar == 1)
                printf("\ngetsize == eof");

        if (tmp == '\n')   // its a new line so we stop
            can = 1;
        count++;
    }// end of loop
    seekcheck1 = lseek(fp, linepos, SEEK_SET); // set the position back to what is was
    // error check
    if (seekcheck1 == -1)
    {
        perror("seekcheck1: ");
        exit(1);
    }
    else if (seekcheck1 == 0)
        if (debugvar == 1)
            printf("\nseekcheck1 == eof");
    
    return count - 1;
}

int checkmatch(int line1pos, int line2pos)
{
    char ch1, ch2;
    int counter = 0, can = 0, rdcheck1, rdcheck2, seekcheck1, seekcheck2;

    while (can == 0)
    {
        // seek to the first line
        seekcheck1 = lseek(fp, line1pos, SEEK_SET);

        // error check
        if (seekcheck1 == -1)
        {
            perror("seekcheck1: ");
            exit(1);
        }
        else if (seekcheck1 == 0)
            if (debugvar == 1)
                printf("\nseekcheck1 == eof");

        // read the char
        rdcheck1 = read(fp, &ch1, 1);

        // error check
        if(rdcheck1 == -1){
            perror("Rdcheck1: ");
            exit(1);
        }
        else if(rdcheck1 == 0){
            if(debugvar == 1)
                printf("\nrdcheck1 == eof");
            exit(0);
        }

        // seek to the 2nd line
        seekcheck2 = lseek(fp, line2pos, SEEK_SET);

        // error check
        if (seekcheck2 == -1)
        {
            perror("seekcheck21: ");
            exit(1);
        }
        else if (seekcheck2 == 0)
            if (debugvar == 1)
                printf("\nseekcheck2 == eof");

        // read the char
        rdcheck2 = read(fp, &ch2, 1);

        // error check
        if (rdcheck2 == -1)
        {
            perror("Rdcheck2: ");
            exit(1);
        }
        else if (rdcheck2 == 0)
        {
            if (debugvar == 1)
                printf("\nrdcheck2 == eof");
            exit(0);
        }

        // we check if they are different, if they are different we do return counter - 1
        if(ch1 != ch2){
            if(counter == 1)
                return counter;
            else if(counter >= 2)
                return counter; // we do minus 1 cause the new line it got to isnt the same
        }
        // if they are the same and get to a new line, counter++
        if(ch1 == ch2 && ch1 == '\n')
            counter++;

        // move forward in the line, increment
        line2pos++, line1pos++;

    } // end of while
    // should never get here
    if(debugvar == 1)
        printf("\nShouldn't get here\n");
    return 0;
}

/*
Rules
if they arent the same print the line out
if they are the same print one of the lines and move down
if they arent the same size its unique
if they are the same size check if they are the same
*/