
#include <stdio.h>
#include <string.h>
#include<curses.h>  //conio doesn't work for macOS

int main(int argc, char** argv)
{
    if (argc < 2) {
        printf("Filename not provided. Exiting...");
        return 1;
    }

    char* fileName = argv[1];
    FILE* file = fopen(fileName,"r");
    if(!file)
        printf("Can not open the File");
    else {
        char buffer[1024];
        bool isFirst = true;
        bool foundQuote = false;
        char element[512]; 

        while(fgets(buffer, 1024, file)) {
            if (isFirst) {
                isFirst = false;
                continue;
            }
            // At start of newline, set our element array to 0
            memset(element, 0, 512);

            // Iterate over the buffer and print column elements
            for (int i = 0; i < strlen(buffer); i++) {
                switch (buffer[i])
                {
                    case ',':
                        if (!foundQuote) {
                            printf("%s \n", element);
                            memset(element, 0, 512);
                        }
                        break;
                    case '\"':
                        foundQuote = !foundQuote;
                        // strcat(element, (char[2]) { buffer[i], '\0' });
                        continue;
                    default:
                        strcat(element, (char[2]) { buffer[i], '\0' });
                        break;
                }
            }

            // Print last element remaining inside the variable
            printf("%s \n", element);
        }
    }

    fclose(file);

    return 0;
}