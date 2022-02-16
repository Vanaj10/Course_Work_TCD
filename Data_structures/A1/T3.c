//  Created by Vanaj Kamboj on 10/10/2021.
//  Copyright © 2021 Vanaj Kamboj. All rights reserved.

#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include <ctype.h>  //for isalnum

#define ARRAY_SIZE 59
#define MAX_STRING_SIZE 20

int numElements = 0;
int collisions = 0;

typedef struct Element Element;

struct Element {
    unsigned int key;
    char* name;
    unsigned short int frequency;
};

// Creating a global hash table for ease of use
Element* hashTable[ARRAY_SIZE];

int hash1(char *s)
{
    int hash = 0;
    while (*s)
    {
        hash = (hash + *s) % ARRAY_SIZE;
        s++;
    }
    return hash;
}
int hash2(char *s)
{
    int hash = 0;
    while (*s)
    {
        hash = (31*hash + *s) % ARRAY_SIZE;
        s++;
    }
    return hash;
}
int hash(char *s, int i){
    int hash = 0;
    int f, g, k;
    while(*s){
        k = hash + *s;
        f = k % ARRAY_SIZE;
        g = 1 + (k % (ARRAY_SIZE-1));
        hash = (f + i * g) % ARRAY_SIZE;
        s++;
    }
    return hash;
    printf("\n%d", hash);
}


Element* createNewElement(char* name, int i){
    // TODO
    // you might want to use the function strcpy from the string package here!
    Element *newElement = (Element*) malloc(sizeof(Element));
    newElement->name = (char*) malloc(strlen(name) + 1);

    // Memory allocation completed, now add stuff to element
    strcpy(newElement->name, name);
    newElement->frequency = 1;
    newElement->key = hash(name, i);

    // printf("Key for %s is %d\n", name, newElement->key);

    return newElement;
}

// returns the index of element with name name or empty place where element can be stored, or -1 if hash table is full!
int search(char* name, bool addCollisions) {
    //TODO
    int i = 1;
    int pos = hash(name, i);
    
    // int startPos = pos;

    while (hashTable[pos] != NULL) {
        i++;
        if (strcmp(hashTable[pos]->name, name) == 0)
            return pos;
        
        pos = hash(name, i);
        // Increment counter for collision metrics
        if (addCollisions)
            collisions++;
        // pos = pos % ARRAY_SIZE;
    //     if (pos == startPos) {
    //         return -1;
    //     }
    }
    
    return pos;
}

// assuming that no element of key name is in the list (use search first!), add element at the correct place in the list
// NB: it would be more efficient for search to return the index where it should be stored directly, but aiming for simplicity here!
void insert(char* name) {
   // Check if element exists in table already
   int idx = search(name, true);

   if (idx == -1)
       printf("Hash Table full. Cannot Insert.\n");
   else if (hashTable[idx] == NULL ) {
       // Search did not find name and returned empty bucket position
       hashTable[idx] = createNewElement(name, idx);
    //    printf("Inserted %s\n", name);
    
       // Increment counter for metrics
       numElements++;
   } else {
       // Search has found element with name, so we increase frequency
       hashTable[idx]->frequency++;
    //    printf("Name %s already exists, increased frequency to %d\n", name, hashTable[idx]->frequency);
   }
}


void printElementDetails(Element* element){
    printf("Name: %s, Frequency: %d \n", element->name, element->frequency);
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
            insert(buffer);

            // Print last element remaining inside the variable
            printf("%s", element);
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
        // scanf("%s",&input);
        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "quit") == 0)
            break;
            
        int index = search(input, false);
        if (index == -1) {
            printf("Name not found and hash table is full.\n");
        } else if (hashTable[index] == NULL) {
            printf("%s not in table.\n", input);
        } else {
            printf("- %d\n", hashTable[index]->frequency);
        }
    }

    return 0;
}