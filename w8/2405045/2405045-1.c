/*

Offline Assignment
Author: Mohammad Kamrul Hasan
Student ID: 2405045
Course: CSE102
Topic: Pointers

Problem 1: Remove Negative Numbers Using Pointers

*/

#include <stdio.h>
#include <stdlib.h>

void removeNegatives(int* input, int n, int** output, int* newSize) {
  *output = (int*)malloc(n*sizeof(*input));
  if (*output==NULL) {
    printf("Couldn't allocate memory for output array");
    exit(1);
  }
  int i,k;
  *newSize=0;
  for(i=k=0;i<n;i++) (*(input+i)<0)?0:((*newSize)++,*(*output+k++)=*(input+i));
  *output = (int*)realloc(*output,*newSize*sizeof(*input));
  printf("%d\n",*newSize);
  for(i=0;i<*newSize;printf("%d ",*(*output+i++)));
  printf("\n");
}

int main() {
  int n,i,newSize;
  scanf("%d",&n);
  int *input = (int*)malloc(n*sizeof(int)),*output;
  if (input==NULL) {
    printf("Couldn't allocate memory for input array");
    return 1;
  }
  for(i=0;i<n;scanf("%d",input+i++));
  removeNegatives(input,n,&output,&newSize);
  free(input);
  free(output);
  return 0;
}
