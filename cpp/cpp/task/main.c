#include <stdio.h>
int func(int a) {
  printf("yes");
  return 1;
}
int main() { int b = 1 ? func(0) : func(1); }