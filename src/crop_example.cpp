#include <stdio.h>
#include "lp_lib.h"

using namespace std;

int demo() {
  lprec *lp;
  int n_cols, *col_no = NULL, j, ret = 0;
  REAL *row = NULL;

  /* we will build the model row by row
   * so we start with creating a model with 0 rows and 2 columns */
  n_cols = 2; // there are two variables in the model
  lp = make_lp(0, n_cols);
  if (lp == NULL) 
    ret = 1; // couldn't construct a new model

  if (ret == 0) {
    // printf("Setting variable names and allocating memory for building the model...\n");
    // let us name our variables. Not required, but can be useful for debuggin
    char* wheat = (char *) "wheat";
    char* barley = (char *) "barley";
    set_col_name(lp, 1, wheat);
    set_col_name(lp, 2, barley);

    // create space large enough for one row 
    col_no = (int*) malloc(n_cols * sizeof(*col_no));
    row = (REAL *) malloc(n_cols * sizeof(*row));
    if ((col_no == NULL) || (row == NULL))
      ret = 2;
  }

  if (ret == 0) {
    // printf("Adding expense constraint: 120x + 210y <= 15000\n");
    set_add_rowmode(lp, TRUE); // makes building the model faster if it is done rows by row

    // construct first row (120x + 210y <= 15000)
    j = 0;

    col_no[j] = 1; // first column
    row[j++] = 120;

    col_no[j] = 2; // second column
    row[j++] = 210;

    // add the row to lpsolve
    if (!add_constraintex(lp, j, row, col_no, LE, 15000))
      ret = 3;
  }
  
  if (ret == 0) {
    // printf("Adding storage constraint: 110x + 30y <= 4000\n");
    // construct second row (110x + 30y <= 4000)
    j = 0;

    col_no[j] = 1; // first column
    row[j++] = 110;

    col_no[j] = 2; // second column
    row[j++] = 30;

    // add thw row to lpsolve
    if (!add_constraintex(lp, j, row, col_no, LE, 75))
      ret = 3;
  }

  if (ret == 0) {
    // printf("Adding acreage constraint: x + y <= 75\n");
    // construct third row (x + y <= 75)
    j = 0;

    col_no[j] = 1; // first column
    row[j++] = 1;

    col_no[j] = 2; // second column
    row[j++] = 1;

    // add the row to lpsolve
    if (!add_constraintex(lp, j, row, col_no, LE, 75))
      ret = 3;
  }

  if (ret == 0) {
    // printf("Adding objective function: 143x + 60y\n");
    set_add_rowmode(lp, FALSE); // rowmode should be turned off again when done building the model

    // set the objective function (143x + 60y)
    j = 0;

    col_no[j] = 1; // first column
    row[j++] = 143;

    col_no[j] = 2; // second column
    row[j++] = 60;

    // set the objective in lpsolve
    if (!set_obj_fnex(lp, j, row, col_no))
      ret = 4;
  }

  if (ret == 0) {
    // printf("Solving LP problem...\n");
    // set the object direction to maximize
    set_maxim(lp);

    // just out of curiosity, now show the model in lp forst on screen
    // this only works if this is a console application. If not, use write_lp
    // and a filename
    write_LP(lp, stdout);
    // write_lp(lp, "model.lp");

    // I only want to see important messages on screen while solving
    set_verbose(lp, IMPORTANT);

    // now let lpsolve calculate a solution 
    ret = solve(lp);
    if (ret == OPTIMAL)
      ret = 0;
    else
      ret = 5;
  }

  if (ret == 0) {
    // a solution is calculated, now lets get some results

     // objective value
    printf("Objective value: %f\n", get_objective(lp));

    // variable values
    get_variables(lp, row);
    for (j = 0; j < n_cols; j++)
      printf("%s: %.3f\n", get_col_name(lp, j + 1), row[j]);

    // we are done now
  }

  // free allocated memory
  if (row != NULL)
    free(row);
  if (col_no != NULL)
    free(col_no);

  if (lp != NULL) {
    // clean up such that all used memory by lpsolve is freed
    delete_lp(lp);
  }

  return ret;
}


int main()
{
  printf("Solve crop problem...\n");
  int ret = demo();
  return 0;
}
