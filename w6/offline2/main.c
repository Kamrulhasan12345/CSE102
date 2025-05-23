#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "utils.h"

#define MAX_SIZE 1000000
#define MAX_BINS 1000
#define MAX_STARS 50

double array[MAX_SIZE], min, max, stddev, mean, sum, median, sq_sum;
int n, f, i, j;

int cmp(const void *a, const void *b) {return *(double *)a-*(double *)b;}

void help(){
  printf("Commands: \n");
  printf("  set     - Set array size, seed, and distribution (uniform or gaussian\n");
  printf("  min     - Print minimum value\n");
  printf("  max     - Print maximum value\n");
  printf("  mean    - Print mean value\n");
  printf("  median  - Print median value\n");
  printf("  stddev  - Print standard value\n");
  printf("  hist    - Plot histogram\n");
  printf("  summary - Print min, max, mean, stddev\n");
  printf("  help    - Show this help message\n");
  printf("  exit    - Exit the program\n");
}

void pmin(){
  for(min=array[0],i=0;i<n;min<=array[i]?:(min=array[i]),i++);
  printf("Min       :%11.4f\n",min);
}
void pmax() {
  for(max=array[0],i=0;i<n;max>=array[i]?:(max=array[i]),i++);
  printf("Max       :%11.4f\n",max);
}

void pmean(){
  for(i=0;i<n;sum+=array[i++]); mean = sum/n;
  printf("Mean      :%11.4f\n",mean);
}

void pmedian(){
  qsort(array,n,sizeof(double),cmp);
  median = n&1?array[n/2]:((array[n/2]+array[n/2+1])/2);
  printf("Median    :%11.4f\n",median);
}

void pstddev(){
  for(i=0;i<n;sum+=array[i++]); mean = sum/n;
  for(i=0;i<n;sq_sum+=(array[i]-mean)*(array[i++]-mean)); stddev=sqrt(sq_sum/n);
  printf("Std Dev    :%11.4f\n",stddev);
}


int main() {
  // printf("Go");]
  char cmd[7],dist[9];
  for (f=0,help();;) {
    printf("\nEnter command: "); scanf("%s",&cmd);
    if (!strcmp(cmd,"set")) {
        unsigned int s;
        for(;printf("Enter number of elements (<= 100000): "),scanf("%d",&n);)if (n>0&&n<=MAX_SIZE) break; else printf("Invalid number of elements. Must be between 1 and %d\n",MAX_SIZE);
        printf("Enter seed: "); scanf("%d",&s);
        for(;printf("Distribution? (uniform/gaussian): "),scanf("%s",&dist);){
        if(!strcmp(dist,"uniform")) {
          double min, max;
          printf("Enter min and max: "); scanf("%lf%lf",&min,&max);
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
      if (!f) printf("Array not initialized. Use 'set' command first.\n");
      else {
        for(min=array[0],i=0;i<n;min<=array[i]?:(min=array[i]),i++);
        printf("Min       :%11.4f\n",min);
      }
    } else if(!strcmp(cmd,"max")) {
      if (!f) printf("Array not initialized. Use 'set' command first.\n");
      else pmin();
    } else if(!strcmp(cmd,"mean")) {
      if (!f) printf("Array not initialized. Use 'set' command first.\n");
      else pmean();
    } else if(!strcmp(cmd,"median")) {
      if (!f) printf("Array not initialized. Use 'set' command first.\n");
      else pmedian();
    } else if(!strcmp(cmd,"stddev")) {
      if (!f) printf("Array not initialized. Use 'set' command first.\n");
      else pstddev();
    } else if(!strcmp(cmd,"summary")) {
      if (!f) printf("Array not initialized. Use 'set' command first.\n");
      else pmin(),pmax(),pmean(),pmedian(),pstddev();
    } else if(!strcmp(cmd,"hist")) {
      if (!f) printf("Array not initialized. Use 'set' command first.\n");
      else {
        int bins;
        for(;printf("Enter number of bins: "),scanf("%d",&bins);) if (bins>0&&bins<=MAX_BINS) break; else printf("Invalid number of elements. Must be between 1 and %d\n",MAX_BINS);
        for(min=array[0],i=0;i<n;min<=array[i]?:(min=array[i]),i++);
        for(max=array[0],i=0;i<n;max>=array[i]?:(max=array[i]),i++);
        double s=min,bin_width=(max-min)/bins;
        int bin_counts[MAX_BINS]={}, bin_index, star_c, max_count;
        for(i=0;i<n;
          bin_index=(int)((array[i]-min)/bin_width),
          bin_counts[bin_index]++,i++);
        for(max_count=0,i=0;i<bins;max_count>=bin_counts[i]?:(max_count=bin_counts[i]),i++);
        for(i=0;i<bins;printf("\n"),s+=bin_width,i++) {
          printf("[%6.2f - %6.2f]: ",s,s+bin_width);
          star_c = (1.*bin_counts[i]/max_count)*MAX_STARS;
          for(j=0;j<star_c;printf("*"),j++);
        }
      }
    } else if(!strcmp(cmd,"help")) help();
    else if(!strcmp(cmd,"exit")) return 0;
    else printf("Unknown command. Type 'help' for list of commands.");
  }
}