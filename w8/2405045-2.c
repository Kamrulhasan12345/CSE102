/*

Offline Assignment
Author: Mohammad Kamrul Hasan
Student ID: 2405045
Course: CSE102
Topic: Pointers

Problem 2: Lexicographical Sorting Using Pointer Arithmetic

*/

#include <stdio.h>
#include <stdlib.h>

int main() {
  int n,i,j,k;
  scanf("%d",&n);
  char **words=(char**)malloc(n*sizeof(char*));
  if (words==NULL) {
    printf("Couldn't allocate %d bytes",n*sizeof(char*));
    exit(1);
  }
  for(i=0;i<n;i++) {
    /* calloc required for the first approach to swapping, 
    not required for the second approach */
    *(words+i)=(char*)calloc(26,sizeof(char));
    if (*(words+i)==NULL) {
        printf("Couldn't allocate 26 bytes for %dth word",i+1);
        exit(1);
    }
    scanf("%s",*(words+i));
  }
  for(i=0;i<n-1;i++) {
    for(j=0;j<n-i-1;j++) {
      for(k=0;*(*(words+j)+k)==*(*(words+j+1)+k);k++); 
      if (*(*(words+j)+k)>*(*(words+j+1)+k)){
        /* start swapping from the unmatched char to increase efficiency */
        // for(;*(*(words+j)+k)!=0||*(*(words+j+1)+k)!=0;k++)
        // *(*(words+j)+k)=*(*(words+j)+k)^*(*(words+j+1)+k),
        // *(*(words+j+1)+k)=*(*(words+j)+k)^*(*(words+j+1)+k),
        // *(*(words+j)+k)=*(*(words+j)+k)^*(*(words+j+1)+k);
        /* or just swap the pointers */
        char *tmp = *(words+j);
        *(words+j)=*(words+j+1);
        *(words+j+1)=tmp;
      }
    }
  }
  for(i=0;i<n;i++) printf("%s\n",*(words+i)),free(*(words+i));
  free(words);
}
