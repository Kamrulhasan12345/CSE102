/*

Offline Assignment
Author: Mohammad Kamrul Hasan
Student ID: 2405045
Course: CSE102
Topic: Loops and Conditionals

Variables and their purposes:
u = iterator for the main loop
t = total number of energy till now
n = number of rows
s = total number of energy in a row
i = iterator for the row scanning loop
k = number of items in a certain row
c = current integer of a row
a = sum of current row integers
m = first - second + third .... sum of the current row
p = temporary storage for the sum of current row integers
r = reversed number of current integer
l = number of things followed in a certain row
j = iterator for finding factors of the sum of all numbers in a row

*/

#include <stdio.h>

int main() {
  int u, t, n, s, i, k, c, a, m, p, r, l, j;
  for(scanf("%d",&n),u=t=0; (u < n); t+=s, u++) {
    for (scanf("%d",&k), i=1,a=s=m=r=p=l=0,c=-1; i<=k; scanf("%d",&c), a+=c, m+=c*(2*(i%2)-1), i++); /* i%2 = 1 or 0, so 2*(i%2) = 0, 2, so it will result in 1, or -1 */
    p = a;
    while (p) {
      r = r*10 + p%10;
      p/=10;
    }
    if (r == a) l++,s+=3; /* checks if palindrome */
    for (j=2; j*j < a; j++) if (a%j== 0) break;
    if (j*j >= a && a>1) l++, s+=5; /* checks if prime-active */
    if (m%7 == 0) l++, s+=4; /* checks if harmonic */
    if (m == 0) l++, s+=2; /* checks if balanced, odd - even should be zero */
    switch (l)
      {
      case 0: printf("Row %d: INACTIVE - Energy Level %d\n", u+1, s); break;
      case 1: printf("Row %d: LOW - Energy Level %d\n", u+1, s); break;
      case 2: printf("Row %d: MEDIUM - Energy Level %d\n", u+1, s); break;
      case 3: printf("Row %d: HIGH - Energy Level %d\n", u+1, s); break;
      case 4: printf("Row %d: ULTRA - Energy Level %d\n", u+1, s); break;
      }
  }
  printf("Total Energy: %d", t);
  return 0;
}
