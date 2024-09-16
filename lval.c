#include <stdio.h>
#include "lval.h"

/*Create a new number lval type*/
lval lval_num(float x)
{
	lval v;
	v.type = LVAL_NUM;
	v.num = x;
	return v;
}

/*Create a new error type lval*/

lval lval_err(int x)
{
	lval v;
	v.type = LVAL_ERR;
	v.err = x;
	return v;
}

/*Print lvals*/
void lval_print(lval v)
{
	switch(v.type)
	{
	case LVAL_NUM: printf("%f",v.num); break;
	case LVAL_ERR:
		       if (v.err == LERR_DIV_ZERO)
		       {
			       printf("Error: Divsion By Zero!");
		       }
		       if(v.err == LERR_BAD_OP)
		       {
			       printf("Error: Invalid Operator!");
		       }

		      if(v.err == LERR_BAD_NUM)
		      {
			      printf("Error: Invalid Number!");
		      }
		break;	
	}	
}


/*Print lval with newline*/

void lval_println(lval v)
{
	lval_print(v);
	putchar('\n');
}