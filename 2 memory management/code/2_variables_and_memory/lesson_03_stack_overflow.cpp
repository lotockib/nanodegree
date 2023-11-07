#include <stdio.h>

void make_variable() {
  int x = 1;
  printf("iteration %p\n", &x);
  make_variable();
}

int main() {
  // Call function that recurses on itself so that varibales are made until stack overflow
  // More than 4 bytes are made per call because the int needs memory, but so do thing sfor the function
  make_variable();
}
