//  Created by Vanaj Kamboj on 8/10/2021.
//  Copyright © 2021 Vanaj Kamboj. All rights reserved.


#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include <ctype.h>  //for isalnum

/*
By "size" of the hash table we mean how many slots or buckets it has
Choice of hash table size depends in part on choice of hash function, and collision resolution strategy
But a good general “rule of thumb” is:
The hash table should be an array with length about 1.3 times the maximum number of keys that will actually be in the table,
and Size of hash table array should be a prime number 

So, let M = the next prime larger than 1.3 times the number of keys you will want to store in the table,
and create the table as an array of length M

(If you underestimate the number of keys, you may have to create a larger table and rehash the entries when it gets too full;
if you overestimate the number of keys, you will be wasting some space)
*/
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

int hash(char *s)
{
    int hash = 0;
    while (*s)
    {
        hash = (hash + *s) % ARRAY_SIZE;
        s++;
    }
    return hash;
}

/*----------------------------------------------------------------
https://www.geeksforgeeks.org/string-hashing-using-polynomial-rolling-hash-function/

Where P and M are some positive numbers.  And s[0], s[1], s[2] … s[n-1] are the values assigned to each character in English alphabet (a->1, b->2, … z->26). 

Appropriate values for P and M  

P: The value of P can be any prime number roughly equal to the number of different characters used. 
For example: if the input string contains only lowercase letters of the English alphabet, then P = 31 is the appropriate value of P. 
If the input string contains both uppercase and lowercase letters, then P = 53 is an appropriate option.
M: the probability of two random strings colliding is inversely proportional to m, Hence m should be a large prime number. 
M = 10 ^9 + 9 is a good choice.

*/


Element* createNewElement(char* name){
    // TODO
    // you might want to use the function strcpy from the string package here!
    Element *newElement = (Element*) malloc(sizeof(Element));
    newElement->name = (char*) malloc(strlen(name) + 1);

    // Memory allocation completed, now add stuff to element
    strcpy(newElement->name, name);
    newElement->frequency = 1;
    newElement->key = hash(name);

    // printf("Key for %s is %d\n", name, newElement->key);

    return newElement;
}

// returns the index of element with name name or empty place where element can be stored, or -1 if hash table is full!
int search(char* name, bool addCollisions) {
    //TODO
    int pos = hash(name);
    int startPos = pos;

    while (hashTable[pos] != NULL) {
        if (strcmp(hashTable[pos]->name, name) == 0)
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
void insert(char* name) {
   // Check if element exists in table already
   int idx = search(name, true);

   if (idx == -1)
       printf("Hash Table full. Cannot Insert.\n");
   else if (hashTable[idx] == NULL) {
       // Search did not find name and returned empty bucket position
       hashTable[idx] = createNewElement(name);
    //    printf("Inserted %s\n", name);

       // Increment counter for metrics
       numElements++;
   } else {
       // Search has found element with name, so we increase frequency
       hashTable[idx]->frequency++;
    //    printf("Name %s already exists, increased frequency to %d\n", name, hashTable[idx]->frequency);
   }
}


// void printElementDetails(Element* element){
//     printf("Name: %s, Frequency: %d \n", element->name, element->frequency);
// }

//  Reads the contents of a file and adds them to the hash table - returns 1 if file was successfully read and 0 if not.
int load_file ( char *fname ) {
	FILE* file = fopen(fname,"r");
    if(!file) {
        printf("Can not open the File");
        return 0;
    }
    else {
        printf("File %s loaded\n", fname);
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

    char input[MAX_STRING_SIZE];
    printf("Enter term to get frequency or type \"quit\" to escape\n");
    while (true)
    {
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
            printf("%s %d\n", input, hashTable[index]->frequency);
        }
    }

    return 0;
}