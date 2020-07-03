/////////////////////////////////////////////////////////////////////////
// File Name       : Assignment2_1.c                                   //                           
// Date            : 2020/04/25                                        //                           
// OS              : Ubuntu 18.04.4 LTS                                //         
// Student Name    : Seung Hoon Jeong                                  //                 
// Student ID      : 2015707003                                        //                     
// ------------------------------------------------------------------- //
// Title : System Programming Assignment #2-1                          //
// Description : Argument handling implemetation using getopt function //             
/////////////////////////////////////////////////////////////////////////

#include <unistd.h>
#include <stdio.h>
#include <getopt.h>
int main(int argc, char **argv)
{
    int aflag = 0, bflag = 0;
    char *cvalue = NULL;
    int index, c;
    opterr = 0;
///////////// arg insert and Analysis ///////////////////////////////////////////////////////
    while((c = getopt(argc, argv, "abc:")) != -1)
    {
	    switch(c)
	    {
		    /* write code */
            case 'a':
                aflag=1; // when '-a' option input, change integer of aflag
                break;
            case 'b':
                bflag=1; // when '-b' option input, change integer of bflag
                break;
            case 'c': // when '-c' option input
                cvalue = optarg; //after option '-c', Save input argument content to cvalue array
                break;
        }
    }
    printf("aflag = %d, bflag = %d, cvalue = %s\n", aflag, bflag, cvalue);
    for(index = optind; index < argc; index++)
	    printf("Non-option argument %s\n", argv[index]); // print argument content except option -a,-b,-c
/////////////////// End of argument Analysis//////////////////////////////////////////////////
    return 0;
}
