#include <stdio.h> 
#include "t2.h"


int number_comparisons = 0;
int number_swaps = 0;


void selectionSort(int arr[], int size) 
{ 
    int i,j;
    for(i=0;i<size-1;i++){
        int min=i;
        // number_comparisons++;
        for(j=i+1;j<size;j++){
            if(arr[j]<arr[min]){
            number_comparisons++;
            min=j;
            }
        


        }
        if(min!=i){
            // swap(&arr[i],&arr[min]);
            int temp=arr[i];
            arr[i]=arr[min];
            arr[min]=temp;

            number_swaps++;
        }
    }
}    
 
void insertionSort(int arr[], int size) 
{ 
    int i,j;
  for(i=1;i<size;i++){
      int temp=arr[i];
      number_comparisons++;
      for(j=i-1;j>=0&&arr[j]>temp;j--){
          arr[j+1]=arr[j];
          number_swaps++;
      }
      arr[j+1]=temp;
  }
  
}

// The implementation idea for quickSort function was taken from 
// https://stackoverflow.com/questions/50858823/c-quicksort-in-one-function-with-2-parameters-int-length

void quickSort(int* array, int size)
{
        int Pivot = array[size-1];
        int idxLargestElement = 0;
        for (int i = 0; i < size-1; i++)
        {number_comparisons++;
                if (array[i] < Pivot)
                {
                        // Swap largest element with this
                        number_swaps++;
                        int temp = array[i];
                        array[i] = array[idxLargestElement];
                        array[idxLargestElement]  = temp;
                        idxLargestElement++;
                }
        }
        // swap pivot with array[idxLargestElement]
        number_swaps++;
        int temp = array[idxLargestElement];
        array[idxLargestElement] = array[size-1];
        array[size-1] = temp;
        if (idxLargestElement > 1)
                quickSort(array, idxLargestElement);
        if (size-idxLargestElement-1 > 1)
                quickSort(array+idxLargestElement+1, size-idxLargestElement-1);
}


