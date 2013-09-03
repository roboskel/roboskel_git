#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <stdlib.h>

using namespace std;


int main()
{
    char number[10], id[2], x[10], y[10], z[10];
	int counter = 0;
    char b[101] = "";
    char filename[] = "_skel2.data";
    FILE *file = fopen(filename, "r");
	FILE *write = fopen ("sticks2.data","w");
    if (file != NULL) {
        char line[5000];
        while (fgets(line, sizeof(line), file) != NULL) {
            if ( counter % 6 == 0 || counter % 6 == 1) {
				sscanf(line, "%s %s %s %s %s", &number, &id, &x, &y, &z);
				printf("number:%s \t id:%s -->x:%s \t y:%s \t z:%s \n", number, id, x, y, z);
				fprintf(write,"%s \t %s \t %s \t %s \t %s \n", number, id, x, y, z);
				
			}
			counter++;


        }
        //fclose(file);
		//fclose(write);
    } else {
        perror(filename);
    }
	
}
