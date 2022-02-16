
#include <stdio.h>
#include <string.h>
#include <curses.h>  //conio doesn't work for macOS
#include <stdlib.h>

const int elementSize = 256;

// Struct for storing a Pokemon's details
typedef struct {
    int id;
    char *name;
    char *type1;
    char *type2;
    int total;
    int hp;
    int attack;
    int defense;
    int splAttack;
    int splDefense;
    int speed;
    int generation;
    bool legendary;
    char *pokedex;
} Pokemon;

Pokemon *getPokemon(char buffer[]) {
    int column = 0;
    bool foundQuote = false;
    char element[elementSize];
    memset(element, 0, elementSize);    // Clear variable to build next field

    // Result Pokemon struct
    Pokemon *pokemon = malloc(sizeof(Pokemon));

    // Iterate over the buffer and print column elements
    for (int i = 0; i < strlen(buffer); i++) {
        switch (buffer[i])
        {
            case ',':
                if (!foundQuote) {
                    // Column data present in element
                    // printf("%s\n", element);

                    // Store this element in struct!
                    // TODO: This part can be abstracted into a neat function...
                    switch (column)
                    {
                    case 0:
                        pokemon->id = atoi(element);
                        break;
                    case 1:
                        pokemon->name = strdup(element);
                        break;
                    case 2:
                        pokemon->type1 = strdup(element);
                        break;
                    case 3:
                        pokemon->type2 = strdup(element);
                        break;
                    case 4:
                        pokemon->total = atoi(element);
                        break;
                    case 5:
                        pokemon->hp = atoi(element);
                        break;
                    case 6:
                        pokemon->attack = atoi(element);
                        break;
                    case 7:
                        pokemon->defense = atoi(element);
                        break;
                    case 8:
                        pokemon->splAttack = atoi(element);
                        break;
                    case 9:
                        pokemon->splDefense = atoi(element);
                        break;
                    case 10:
                        pokemon->speed = atoi(element);
                        break;
                    case 11:
                        pokemon->generation = atoi(element);
                        break;
                    case 12:
                        pokemon->legendary = strcmp(element, "True");
                        break;
                    case 13:
                        pokemon->pokedex = strdup(element);
                        break;
                    default:
                        break;
                    }

                    // Store this in our struct.
                    memset(element, 0, elementSize);    // Clear variable to build next field
                }
                column++;
                break;
            case '\"':
                foundQuote = !foundQuote;
                continue;
            default:
                strcat(element, (char[2]) { buffer[i], '\0' });
                break;
        }
    }

    // Add last element remaining inside the variable
    // We know last column is pokedex
    pokemon->pokedex = strdup(element); 

    return pokemon;
}

int main(int argc, char** argv) {
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
        int totalAttack = 0;
        int totalCharacters = 0;

        while(fgets(buffer, 1024, file)) {
            if (isFirst) {
                isFirst = false;
                continue;
            }
            Pokemon* pokemon = getPokemon(buffer);
            totalAttack += pokemon->attack;
            totalCharacters++;
        }

        float avgAttack = ((float) totalAttack / (float) totalCharacters);
        printf("%f", avgAttack);
    }
    fclose(file);

    return 0;
}


