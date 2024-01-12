#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char* argv[]) {

    // Start initial post-open error handling ------------------------------------------------------------------------------------------------------

    // Handle case of > 1 argument
    if (argc != 2) {
        printf("Two parameters are accepted: './wordcount', and './<filename>'.\n");
        printf("Supplied, however, were %d arguments: \" ", argc);
        for (int i = 0; i < argc; i++) {
            printf("%s ", argv[i]);  
        }
        printf("\"\n");
        exit(-1);
    }

    // Begin variable creation
    char* filename = argv[1];
    int file = open(filename, O_RDONLY);
    char buffer = '\0';
    int status = 0;
    int lineCount = 0;
    int wordCount = 0;
    int charCount = 0;
    int inWord = 0;
    int lastChar = ' ';
    
    // Handle case of issue opening file
    if (file == -1) {
        printf("Error on opening file: %s\n", filename);
        printf("Error number %d: %s\n", errno, strerror(errno));
        close(file);
        exit(-1);
    }   

    // Handle case(s) of issue initially reading the file
    status = read(file, &buffer, 1);
    if (status == -1) {
        printf("Error on reading file: %s\n", filename);
        printf("Error numbers %d: %s\n", errno, strerror(errno));
        close(file);
        exit(-1);
    } else if (status == 0) {
        printf("Error on reading file: %s.\n", filename);
        printf("File  is empty.\n");
        close(file);
        exit(-1);
    }

    // End initial post-open error handling --------------------------------------------------------------------------------------------------------
    
    // Reset pointer to read from start of file
    lseek(file, 0, SEEK_SET);

    // Iterate file and count number of newlines, words, and characters
    do {
        status = read(file, &buffer, 1);

        switch(status) {

            case 0:
                if (inWord) {
                lineCount++;
                wordCount++;
                inWord = 0;
            }
            break;

            case -1:
                printf("Error on reading file: %s.\n", filename);
                printf("Error number: %d %s\n", errno, strerror(errno));
                close(file);
                exit(-1);
                break;

            default:
                charCount++;
                if (buffer == '\n') {
                    lineCount++;
                }
                
                if (isspace(buffer) || buffer == '\n') {
                    if (inWord) {
                        wordCount++;
                        inWord = 0;
                    }

                } else {
                    if (!inWord) {
                        inWord = 1;
                    }
                }
                lastChar = buffer;
                break;
        }
    } while (status > 0);

    // Check if the last character was non-whitespace character
    if (inWord) {
        wordCount++;
    }

    // Ending
    close(file);

    printf("%d %d %d %s\n", lineCount, wordCount, charCount, filename);

    return 0;
}
