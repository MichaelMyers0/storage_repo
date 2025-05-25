#if 1
#include "file_handler.h"
int main()
{
	file_handler fh;
	fh.modify_file();
	exit(0);
}
#else
#include <iostream>
#include <stdio.h>
#include <ctime>
int main() {
    time_t start_time = time(NULL);
    int duration = 5;

    printf("Timer started for %d seconds...\n", duration);

    while (difftime(time(NULL), start_time) < duration) {
        // Code to execute during timer
    }

    printf("Timer is up!\n");
    return 0;
}
#endif
