//  Created by Vanaj Kamboj on 13/10/2021.
//  Copyright © 2021 Vanaj Kamboj. All rights reserved.

#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include <ctype.h>  //for isalnum

#define ARRAY_SIZE 99991
#define MAX_STRING_SIZE 100

int numElements = 0;
int collisions = 0;

typedef struct Person Person;
struct Person{
    int id;
    char* depositionId;
    char* surname;
    char* forename;
    char* personType;
    char* gender;
    char* nationality;
    char* religion;
    char* occupation;
    int age;
    Person* next;
};

typedef struct Element Element;
struct Element{
    int key;
    Person* person;
};

// Creating a global hash table for ease of use
Element* hashTable[ARRAY_SIZE];

int hash(char *s)
{
    int hash = 0;
    while (*s)
    {
        hash = (31*hash + *s) % ARRAY_SIZE;
        s++;
    }
    return hash;
}

Person *createNewPerson(char buffer[])
{
    int column = 0;
    bool foundQuote = false;
    char element[MAX_STRING_SIZE];
    memset(element, 0, MAX_STRING_SIZE);    // Clear variable to build next field

    // Result Person struct
    Person *person = malloc(sizeof(Person));

    // Iterate over the buffer and print column elements
    for (int i = 0; i < strlen(buffer); i++) {
        switch (buffer[i])
        {
            case ',':
                if (!foundQuote) {
                    switch (column)
                    {
                    case 0:
                        person->id = atoi(element);
                        break;
                    case 1:
                        person->depositionId = strdup(element);
                        break;
                    case 2:
                        person->surname = strdup(element);
                        break;
                    case 3:
                        person->forename = strdup(element);
                        break;
                    case 4:
                        person->age = atoi(element);
                        break;
                    case 5:
                        person->personType = strdup(element);
                        break;
                    case 6:
                        person->gender = strdup(element);
                        break;
                    case 7:
                        person->nationality = strdup(element);
                        break;
                    case 8:
                        person->religion = strdup(element);
                        break;
                    // case 9:
                    //     person->occupation = strdup(element);
                    //     break;
                    default:
                        break;
                    }

                    // Store this in our struct.
                    memset(element, 0, MAX_STRING_SIZE);    // Clear variable to build next field
                    column++;
                }
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
    person->occupation = strdup(element); 

    return person;
}

Element *createNewElement(Person *person)
{
    Element *newElement = (Element*) malloc(sizeof(Element));

    // Use hash function and get key
    newElement->key = hash(person->surname);
    newElement->person = person;

    // printf("Key for %s is %d\n", name, newElement->key);
    return newElement;
}

// returns the index of element with name name or empty place where element can be stored, or -1 if hash table is full!
int search(char* surname, bool addCollisions) {
    //TODO
    int pos = hash(surname);
    int startPos = pos;

    while (hashTable[pos] != NULL) {
        if (strcmp(hashTable[pos]->person->surname, surname) == 0)
            return pos;
        pos++;
        // Increment counter for collision metrics
        if (addCollisions)
            collisions++;
        pos = pos % ARRAY_SIZE;
        if (pos == startPos) {
            return -1;
        }
    }
    
    return pos;
}

// assuming that no element of key name is in the list (use search first!), add element at the correct place in the list
// NB: it would be more efficient for search to return the index where it should be stored directly, but aiming for simplicity here!
void insert(Person* person)
{
    // Check if element exists in table already
    int idx = search(person->surname, true);

    if (idx == -1)
        printf("Hash Table full. Cannot Insert.\n");
    else if (hashTable[idx] == NULL)
    {
        // Search did not find name and returned empty bucket position
        hashTable[idx] = createNewElement(person);

        // Increment counter for metrics
        numElements++;
    }
    else
    {
        // Search has found element with name, so we increase frequency
        // Traverse our linked list to the last node
        Person *current = hashTable[idx]->person;
        while (current->next != NULL) {
            current = current->next;
        }

        // Insert person to end of LL
        current->next = person;
    }
}

//  Reads the contents of a file and adds them to the hash table - returns 1 if file was successfully read and 0 if not.
int load_file ( char *fname ) {
	FILE* file = fopen(fname,"r");
    if(!file) {
        printf("Can not open the File");
        return 0;
    }
    else {
        printf("File %s loaded!\n", fname);
        char buffer[1024];
        // bool isFirst = true;
        // bool foundQuote = false;
        char element[MAX_STRING_SIZE]; 
        while(fgets(buffer, 1024, file)) {
            memset(element, 0, MAX_STRING_SIZE);

            // Iterate over the buffer and print column elements
            buffer[strcspn(buffer, "\n")] = '\0';
            Person* person = createNewPerson(buffer);
            // printf("Person for insert -> %d -> %s %s\n", person->id, person->forename, person->surname);
            insert(person);
        }
    }

    fclose(file);
    return 1;
}

void printMetrics() {
    printf("\tCapacity: %d\n", ARRAY_SIZE);
    printf("\tNum Terms: %d\n", numElements);
    printf("\tCollisions: %d\n", collisions);
    printf("\tLoad: %f\n", (double)numElements/ARRAY_SIZE);
}

void printPersonList(Person *head) {
    printf("Person ID\tDeposition ID\tSurname\tForename\tAge\tPerson Type\tGender\tNationality\tReligion\tOccupation\n");
    Person *current = head;
    do {
        printf("%d\t%s\t%s\t%s\t%d\t%s\t%s\t%s\t%s\t%s\n", current->id, current->depositionId, current->surname, current->forename, current->age, current->personType, current->gender, current->nationality, current->religion, current->occupation);
        current = current->next;
    } while (current != NULL);
}


int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Filename not provided. Exiting...");
        return 1;
    }
    char* fileName = argv[1];

    load_file(fileName);
    printMetrics();
    printf("Enter term to get frequency or type \"quit\" to escape\n");

    char input[MAX_STRING_SIZE];
    while (true)
    {
        printf(">>> ");
        fgets(input, MAX_STRING_SIZE, stdin);
        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "quit") == 0)
            break;
            
        int index = search(input, false);
        if (index == -1) {
            printf("Name not found and hash table is full.\n");
        } else if (hashTable[index] == NULL) {
            printf("%s not in table.\n", input);
        } else {
            printPersonList(hashTable[index]->person);
        }
    }

    return 0;
}