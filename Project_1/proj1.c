/*
 * Sabína Gregušová, xgregu02, xgregu02@stud.fit.vutbr.cz
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>

void initInput(char input[]);
void initEnabled(char enabled[]);
void initTowns(char town[][100]);
char toUpper(char a);
bool startsWith(const char *input, const char *pTown);
void printEnabled(char enabled[]);
void checkLength(char * townRow);
int checkCharacter(char c);

/*
 * Valid characters in input and addresses are: letters, space and dash
 * Everything else is evaluated as invalid character
 */

int main(int argc, char*argv[]) {
    char towns[100][100];           // 2D array for all towns
    char* pTown;                    // pointer at town
    char input[100];                // input entered by the user
    char enabled[28];               // possible characters (alphabet plus underscore)
    unsigned int i = 0;             // iteration variables
    int j = 0;
    char c;                         // character for reading
    int iCount = 0;                 // checks how many compliances
    int iLastRow;                   // remembers the last row containing compliance

    // initializes input, address and enabled array
    initInput(input);
    initEnabled(enabled);
    initTowns(towns);

    if(argc > 2){                                       // too many arguments
        fprintf(stderr, "TOO MANY ARGUMENTS\n");
        exit(EXIT_FAILURE);
    } else if(argv[1] && strlen(argv[1]) > 99) {       // input is too long
        fprintf(stderr, "ARGUMENT IS TOO LONG\n");
        exit(EXIT_FAILURE);
    }

    // loads argument into input array
    if (argv[1]) {
        for (i = 0; i < strlen(argv[1]); i++) {
            input[i] = toUpper(argv[1][i]);         // immediately converts everything to uppercase

            if(checkCharacter(input[i]) == 1) {		// check whether argument is valid
                fprintf(stderr, "INPUT CONTAINS INVALID CHARACTER\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    while ((c = (char) getchar()) != EOF) {                  // reads addresses into 2D array
        checkLength(towns[j]);
        if (c != '\n') {
            if(checkCharacter(c) == 1){
                fprintf(stderr, "ADDRESSES CONTAIN INVALID CHARACTER\n");
                exit(EXIT_FAILURE);
            }
            towns[j][i] = toUpper(c);                      // saves argument in line and converts it to uppercase
            i++;
        } else {
            j++;                                             // moves to the next row
            i = 0;                                           // resets iteration of i
        }
    }

    for (j = 0; j < 100; j++) {
        pTown = towns[j];                      // pointer at row

        if (startsWith(input, pTown)) {       // compares input and the row
            iCount++;
            iLastRow = j;
            if (strlen(input) < strlen(pTown)) {     // input is shorter than length of the row
                if (pTown[strlen(input)] == ' ')
                    enabled[26] = '_';              // saves space as underscore
                if (pTown[strlen(input)] == '-')
                    enabled[27] = '-';
                else {
                    c = pTown[strlen(input)];       // saves character in alphabetical order
                    enabled[c - 'A'] = c;
                }
            }
        }
    }

    if (iCount == 0) {
        fprintf(stdout, "NOT FOUND\n");
    } else if (iCount == 1) {
        fprintf(stdout, "FOUND: %s\n", towns[iLastRow]);
    } else {
        printEnabled(enabled);
    }

    return 0;
}

// convert everything into uppercase
char toUpper(char a) {
    if(a >= 'a' && a <= 'z')
        a = a - ('a' - 'A');
    return a;
}

// checks whether the start of input is the same as address
bool startsWith(const char *input, const char *pTown) {
    return(strncmp(input, pTown, strlen(input)) == 0);	//returns true if the strings have equal prefix or are equal
}

// enables printing out all next possible characters
void printEnabled(char enabled[]) {
    fprintf(stdout, "ENABLED: ");
    for(int i = 0; i < 28; i++){
        if(enabled[i]){
            fprintf(stdout, "%c", enabled[i]);
        }
    }
    fprintf(stdout, "\n");
}

void initInput(char input[]){
    for(int i = 0; i < 100; i++){
        input[i] = '\0';
    }
}

void initEnabled(char enabled[]){
    for(int i = 0; i <= 28; i++){
        enabled[i] = '\0';
    }
}

void initTowns(char town[][100]){
    for(int j = 0; j < 100; j++){
        for(int i = 0; i < 100; i++)
            town[j][i] = '\0';
    }
}

void checkLength(char *townRow){
    if(strlen(townRow) > 99){
        fprintf(stdout, "ADDRESSES CONTAIN LINE WITH MORE THAN 100 CHARACTERS\n");
        exit(EXIT_FAILURE);
    }
}

// checks whether character is valid
int checkCharacter(char c) {
    char validCharacter[4] = {' ', '-', '\n', '\0'};
    if (!(isalpha(c) || strpbrk(&c, validCharacter))) {
        return 1;
    }
    else
        return 0;
}
