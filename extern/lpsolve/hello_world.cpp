#include <stdio.h>
#include "lp_lib.h"

using namespace std;

int main()
{
  lprec *lp;
  lp = make_lp(0, 4);
  delete_lp(lp);
  printf("Hello world!");
  return 0;
}
