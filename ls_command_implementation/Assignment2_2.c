//////////////////////////////////////////////////////////////////////////
// File Name       : Assignment2_2.c                                    //                           
// Date            : 2020/04/26                                         //                           
// OS              : Ubuntu 18.04.4 LTS                                 //         
// Student Name    : Seung Hoon Jeong                                   //                 
// Student ID      : 2015707003                                         //                     
// -------------------------------------------------------------------- //
// Title : System Programming Assignment #2-2                           //
// Description : ls command implementation                              //             
//////////////////////////////////////////////////////////////////////////

 

 

#include <sys/types.h>
#include <dirent.h> // defines a file system independent directory entry
#include <stdio.h>
#include <sys/stat.h> // for use lstat function to save file information
#include <string.h>

int main(int argc, char **argv)
{
	DIR *dp; // for open directory access, we need this structure
	struct dirent *dirp;
	struct stat buf;
//////////////////////// arg input //////////////////////
	if (argc != 2) { // must excute two argument 
		printf("Usage : %s [directory]\n", argv[0]);
		return -1;
	}

	if ((dp = opendir(argv[1])) == 0) { // opens a directory stream corresponding to the directory name
		printf("Can't open %s\n", argv[1]);
		return -1;
	}

	while ((dirp = readdir(dp)) != NULL) {
		if (dirp->d_ino != 0) {
			lstat(dirp->d_name, &buf); // get file information
			if (S_ISDIR(buf.st_mode)) { // directory file
				if (!strcmp(dirp->d_name, ".") || !strcmp(dirp->d_name, ".."))
					printf("%s\n", dirp->d_name); // current, parent directory print
				else continue; // except ignoring
			}

			else if (S_ISREG(buf.st_mode)) // regular file 
				printf("%s\n", dirp->d_name);
			else if (S_ISFIFO(buf.st_mode)) // directory file
				printf("%s\n", dirp->d_name);
			else if (S_ISLNK(buf.st_mode)) //  symbolic link
				printf("%s\n", dirp->d_name);
			else if (S_ISCHR(buf.st_mode)) // character special file (Keyboard, mouse, …)
				printf("%s\n", dirp->d_name);
			else if (S_ISBLK(buf.st_mode)) //  block special file (USB storage device)
				printf("%s\n", dirp->d_name);
			else if (S_ISSOCK(buf.st_mode)) // socket
				printf("%s\n", dirp->d_name);
		}
	}
//////////////////////// print content //////////////////////
	closedir(dp); // closes the directory stream associated with dirp
	return 0;
}
