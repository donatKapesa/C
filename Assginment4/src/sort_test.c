#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <math.h>
#include <errno.h>


void sort_words( );
char text_array[5][200] = {"Don", "Thi", "Thi", "Ama", "Bat"};

int main()
{
	printf("%d\n",strcmp("Ha", "Hb"));
	printf("%s\n", "Before sorting we have: ");
	for(int i=0; i < 5; i++) {
		puts(text_array[i]);
	}

	sort_words();

	printf("\n\n\n%s\n\n\n", "After sorting we have: ");
	for(int i=0; i < 5; i++) {
		puts(text_array[i]);
	}


    return 0;
}

// YOU COMPLETE THIS ENTIRE FUNCTION FOR Q1.
void sort_words(){
    // will use insertion sort
    //printf("%s\n", "\n\nSORTING\n\n");
    int i, j;
    char key[200];

    for(i=1; i < 5; i++) { // here 5 = number of words/lines
        strcpy(key, text_array[i]);
        j = i-1;
         printf("%s\n", "Before while loop");
    	printf("%d\n", j);
    	printf("%d\n", i);
    	while(j >= 0 && strcmp(text_array[j], key) > 0) {

    	printf("%s %s\n", text_array[j + 1], text_array[j]);

        strcpy(text_array[j + 1], text_array[j]);
        j = j-1;
    }
    strcpy(text_array[j + 1], key);
    }
}