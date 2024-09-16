#include "mpc/mpc.h"

/*Fake functions for readline and add_history on Windows.*/
#ifdef _WIN32
#include <string.h>

static char buffer[2048];

/*Fake readline()*/
char* readline(char* prompt){
	fputs(prompt,stdout);
	fgets(buffer,2048,stdin);
	char* cpy = malloc(strlen(buffer)+1);
	strcpy(cpy,buffer);
	cpy[strlen(cpy)-1] = '/0';
	return cpy;
}

/*Fake add_hisory()*/
void add_history(char* unused){}

#elif __linux__
#include <readline/readline.h>
#include <readline/history.h>

#else
#include <edit/readline/readline.h>
#include <edit/readline/history.h>
#endif

/*Enums of possible lval.*/
enum{LVAL_NUM, LVAL_ERR};

/*Enum for posible error types.*/
enum {LERR_DIV_ZERO,LERR_BAD_OP,LERR_BAD_NUM};



/*Struct for lval*/
typedef struct {
	int type;
	float num;
	int err;
} lval;


float eval_op(float x, char* op, float y)
{
	
	/**/
	if {x.type == LVAL_ERR} return x;}
	if {y.type == LVAL_ERR} return y;}


	if(strcmp(op, "+")==0) {return x+y;}
	if(strcmp(op, "-")==0) {return x-y;}
	if(strcmp(op, "*")==0) {return x*y;}
	if(strcmp(op, "/")==0) {return x/y;}
//	if(strcmp(op, "%")==0) {return x%y;} 
	
return 0;

}

float eval(mpc_ast_t* t)
{
	if(strstr(t->tag, "number"))
	{	
		return atof(t->contents);
	}	

	char* op = t->children[1]->contents;
	
	float x = eval(t->children[2]);

	int i = 3;
	while (strstr(t->children[i]->tag,"expr"))
	{
		x = eval_op(x,op,eval(t->children[i]));
		i++;
	}

return x;

}


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

int main(int argc, char** argv)
{
	
	/*Create some parsers.*/
	
	mpc_parser_t* Number = mpc_new("number");
	mpc_parser_t* Operator = mpc_new("operator");
	mpc_parser_t* Expr = mpc_new("expr");
	mpc_parser_t* Lisp = mpc_new("lisp");

	/*Define language*/
  mpca_lang(MPCA_LANG_DEFAULT,
    "                                                      \
      number   : /-?(([0-9]*[.])?[0-9]+([.][0-9]*)?)/;     \
      operator : '+' | '-' | '*' | '/' | '%'  ;            \
      expr     : <number> | '(' <operator> <expr>+ ')' ;   \
      lisp     : /^/ <operator> <expr>+ /$/ ;              \
    ",
    Number, Operator, Expr, Lisp);


	char* version = "0.0.3";
	/*Print Version and Exit information*/
	printf("Lisp Version %s\n",version);
	puts("Press Ctrl+c to Quit");

/*Endless loop lol*/
	while(1)
	{

		char* input = readline("lispy> ");
		add_history(input);
		
		mpc_result_t r;
		
		if(mpc_parse("<stdin>",input,Lisp,&r))
		{
			float result = eval(r.output);
			printf("%f\n",result);
			mpc_ast_delete(r.output);
		}
		
		//else
		//{
		//	mpc_err_print(r.error);
		//	mpc_err_delete(r.error);
		//}
		
		
		
		free(input);
	}
	
	mpc_cleanup(4,Number,Operator,Expr,Lisp);

	return 0;
}
