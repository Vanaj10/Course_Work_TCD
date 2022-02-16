
#include <stdio.h>
#include <string.h>
#include <curses.h> //conio doesn't work for macOS
#include <stdlib.h>

#define MAX_BUFFER 256
#define MAX_ROWS 18626

const int elementSize = 256;

// title,platform,Score,release_year

// Struct for storing a Pokemon's details
typedef struct Game
{
    char *title;
    char *platform;
    int score;
    int year;
} Game;

Game *getGame(char buffer[])
{
    // printf("\n%s\n", buffer);
    int column = 0;
    int foundQuote = 0;
    char element[elementSize];
    memset(element, 0, elementSize); // Clear variable to build next field

    // Result Game struct
    Game *game = malloc(sizeof(Game));

    // Iterate over the buffer and print column elements
    for (int i = 0; i < strlen(buffer); i++)
    {
        switch (buffer[i])
        {
        case ',':
            if (!foundQuote)
            {

                // Column data present in element
                // printf("Column data -> %s, Found Quote %d\n", element, foundQuote);

                // Store this element in struct!
                // TODO: This part can be abstracted into a neat function...
                switch (column)
                {
                case 0:
                    game->title = strdup(element);
                    break;
                case 1:
                    game->platform = strdup(element);
                    break;
                case 2:
                    game->score = atoi(element);
                    break;
                case 3:
                    game->year = atoi(element);
                    break;
                default:
                    break;
                }
                column++;
                // Store this in our struct.
                memset(element, 0, elementSize); // Clear variable to build next field
            }
            break;
        case '\"':
            foundQuote = !foundQuote;
            continue;
        default:
            strcat(element, (char[2]){buffer[i], '\0'});
            break;
        }
    }

    game->year = atoi(element);
    return game;
}

void insertionSort(Game *arr[], int size)
{
    int i, j;
    for (i = 1; i < size; i++)
    {
        Game *temp = arr[i];

        for (j = i - 1; j >= 0 && arr[j]->score < temp->score; j--)
        {
            arr[j + 1] = arr[j];
        }
        arr[j + 1] = temp;
    }
}

void print_game(Game *p)
{
    printf("%-60s %-25s %-4d %-4d\n", p->title, p->platform, p->score, p->year);
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Filename not provided. Exiting...\n");
        return 1;
    }
    char *fileName = argv[1];
    FILE *file = fopen(fileName, "r");
    Game *gameArray[MAX_ROWS - 1];
    int numGames = 0;

    if (!file)
        printf("Can not open the File\n");
    else
    {
        // printf("File found! Parsing file...\n");

        char buffer[1024];
        int isFirst = 1;

        while (fgets(buffer, 1024, file))
        {
            if (isFirst)
            {
                isFirst = 0;
                continue;
            }

            gameArray[numGames] = getGame(buffer);
            numGames++;
            insertionSort(gameArray, numGames); // Sort the list
        }
    }

    for (int i = 0; i < 10; i++)
    {
        print_game(gameArray[i]);
    }

    fclose(file);

    return 0;
}
