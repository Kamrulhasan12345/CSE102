/*

Offline Assignment
Author: Mohammad Kamrul Hasan
Student ID: 2405045
Course: CSE102
Topic: Array

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "utils.c"

#define MAX_SIZE 1000000
#define MAX_BINS 1000
#define MAX_STARS 50

#define UNK_CMD printf("Array not initialized. Use 'set' command first.\n")

double array[MAX_SIZE], min, max, stddev, mean, sum, median, sq_sum;
int n, f, i, j;

int cmp(const void *a, const void *b) {return *(double *)a-*(double *)b;}

void help(void){
  printf("Commands:\n");
  printf("  set     - Set array size, seed, and distribution (uniform or gaussian)\n");
  printf("  min     - Print minimum value\n");
  printf("  max     - Print maximum value\n");
  printf("  mean    - Print mean value\n");
  printf("  median  - Print median value\n");
  printf("  stddev  - Print standard deviation\n");
  printf("  hist    - Plot histogram\n");
  printf("  summary - Print min, max, mean, stddev\n");
  printf("  help    - Show this help message\n");
  printf("  exit    - Exit the program\n");
}

void pmin(void){
  for(min=array[0],i=0;i<n;min<=array[i]?:(min=array[i]),i++);
  printf("Min       :%11.4f\n",min);
}
void pmax(void) {
  for(max=array[0],i=0;i<n;max>=array[i]?:(max=array[i]),i++);
  printf("Max       :%11.4f\n",max);
}

void pmean(void){
  for(i=sum=0;i<n;sum+=array[i++]); mean = sum/n;
  printf("Mean      :%11.4f\n",mean);
}

void pmedian(void){
  qsort(array,n,sizeof(double),cmp);
  median = n&1?array[n/2]:((array[n/2-1]+array[n/2])/2);
  printf("Median    :%11.4f\n",median);
}

void pstddev(void){
  for(i=0,sum=0;i<n;sum+=array[i++]); mean = sum/n;
  for(sq_sum=0,i=0;i<n;sq_sum+=(array[i]-mean)*(array[i++]-mean)); stddev=sqrt(sq_sum/n);
  printf("Std Dev   :%11.4f\n",stddev);
}


int main() {
  char cmd[8],dist[9];
  for (f=0,help();;) {
    printf("\nEnter command: "); scanf("%s",&cmd);
    if (!strcmp(cmd,"set")) {
        unsigned int s;
        for(;printf("Enter number of elements (<= 100000): "),scanf("%d",&n);)
          if (n>0&&n<=MAX_SIZE) break; 
          else printf("Invalid number of elements. Must be between 1 and %d\n",MAX_SIZE);
        printf("Enter seed: "); scanf("%d",&s);
        for(;printf("Distribution? (uniform/gaussian): "),scanf("%s",&dist);){
        if(!strcmp(dist,"uniform")) {
          double min, max;
          for(;printf("Enter min and max: "),scanf("%lf%lf",&min,&max);)
            if (max>min) break;
            else printf("Invalid min-max value. The value of 'max' must be greater than 'min'.\n");
          populate_array_uniform(array,n,min,max,s);
          printf("Array initialized with uniform distribution.\n");
          f=1; break;
        } else if(!strcmp(dist,"gaussian")) {
          double mu,sigma;
          printf("Enter mean and stddev: "); scanf("%lf%lf",&mu,&sigma);
          populate_array_gaussian(array,n,mu,sigma,s);
          printf("Array initialized with gaussian distribution.\n");
          f=1; break;
        } else printf("Invalid distribution\n");
      }
    } else if(!strcmp(cmd,"min")) {
      if (!f) UNK_CMD;
      else pmin();
    } else if(!strcmp(cmd,"max")) {
      if (!f) UNK_CMD;
      else pmax();
    } else if(!strcmp(cmd,"mean")) {
      if (!f) UNK_CMD;
      else pmean();
    } else if(!strcmp(cmd,"median")) {
      if (!f) UNK_CMD;
      else pmedian();
    } else if(!strcmp(cmd,"stddev")) {
      if (!f) UNK_CMD;
      else pstddev();
    } else if(!strcmp(cmd,"summary")) {
      if (!f) UNK_CMD;
      else pmin(),pmax(),pmean(),pmedian(),pstddev();
    } else if(!strcmp(cmd,"hist")) {
      if (!f) UNK_CMD;
      else {
        int bins;
        for(;printf("Enter number of bins: "),scanf("%d",&bins);) 
          if (bins>0&&bins<=MAX_BINS) break; 
          else printf("Invalid number of elements. Must be between 1 and %d\n",MAX_BINS);
        for(min=array[0],i=0;i<n;min<=array[i]?:(min=array[i]),i++);
        for(max=array[0],i=0;i<n;max>=array[i]?:(max=array[i]),i++);
        double s=min,bin_width=(max-min)/bins;
        int bin_counts[MAX_BINS]={}, bin_index, star_count, max_count;
        for(i=0;i<n;
          bin_index=(int)((array[i]-min)/bin_width),
          bin_index<bins?:(bin_index=bins-1),
          bin_counts[bin_index]++,i++);
        for(max_count=0,i=0;i<bins;max_count>=bin_counts[i]?:(max_count=bin_counts[i]),i++);
        for(i=0;i<bins;printf("\n"),s+=bin_width,i++) {
          printf("[%6.2f - %6.2f]: ",s,s+bin_width);
          star_count = (MAX_STARS*bin_counts[i]/max_count);
          for(j=0;j<star_count;printf("*"),j++);
        }
      }
    } else if(!strcmp(cmd,"help")) help();
    else if(!strcmp(cmd,"exit")) break;
    else printf("Unknown command. Type 'help' for list of commands.");
  }
  return 0;
}