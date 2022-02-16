#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include "t1.h"

void printArray(int* arr, int size){
  int i;
  for(i=0; i<size;i++){
    printf("%d ", arr[i]);
  }
  printf("\n");
}


//Fills the array with ascending, consecutive numbers, starting from 0.
void fill_ascending(int *array, int size)
{
    //TODO:
    for(int i = 0; i < size; i++)
    {
        array[i] = i+1;
    }
    
    
}
//Fills the array with descending numbers, starting from size-1
void fill_descending(int *array, int size)
{
    //TODO:
    for(int i = 0; i < size; i++)
    {
       array[i] = size-i+1;
    }
    
    
}

//Fills the array with uniform numbers.
void fill_uniform(int *array, int size)
{
    //TODO:
    int random = rand()%size;
    for(int i = 0; i < size; i++)
    {
        array[i] = random;
    }
    
    // printArray(array,7);
    
}

//Fills the array with random numbers within 0 and size-1. Duplicates are allowed.
void fill_with_duplicates(int *array, int size)
{
    //TODO:

    for(int i = 0; i < size; i++)
    {
        array[i] = rand()%size;
    }
    // printArray(array,7);
    
}


//Fills the array with unique numbers between 0 and size-1 in a shuffled order. Duplicates are not allowed.
void fill_without_duplicates(int *array, int size)
{
    //TODO
    int i = 0;
    while (i < size)
    {
        array[i] = i;
        i++;
    }

    if (size > 1)
    {
        i = 0;
        while (i < size - 1)
        {
            int j = i + rand() / (RAND_MAX / (size - i) + 1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
            i++;
        }
    }
}



// int main()
// {
//     int array[7] = {0,6,3,2,7,8,9};
//     fill_ascending(array, 7);
//     fill_with_duplicates(array,7);
//     fill_uniform(array,7);
//     fill_without_duplicates(array,7);
//     // printArray(array,7);

// }
