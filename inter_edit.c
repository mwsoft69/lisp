#include "mpc/mpc.h"
#include "lval.h"

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


lval eval_op(lval x, char* op, lval y)
{
	
	/**/
	if (x.type == LVAL_ERR){ return x;}
	if (y.type == LVAL_ERR) {return y;}


	if(strcmp(op, "+")==0) {return lval_num(x.num + y.num);}
	if(strcmp(op, "-")==0) {return lval_num(x.num-y.num);}
	if(strcmp(op, "*")==0) {return lval_num(x.num * y.num);}
	if(strcmp(op, "/")==0) {
		return y.num == 0
		? lval_err(LERR_DIV_ZERO)
		: lval_num(x.num / y.num);
	}
	
	
return lval_err(LERR_BAD_OP);

}


lval eval(mpc_ast_t* t)
{
	if(strstr(t->tag, "number"))
	{	
		errno = 0;
		float x = strtof(t->contents,NULL);
		return errno != ERANGE ? lval_num(x) : lval_err(LERR_BAD_NUM);
	}	

	char* op = t->children[1]->contents;
	
	lval x = eval(t->children[2]);

	int i = 3;
	while (strstr(t->children[i]->tag,"expr"))
	{
		x = eval_op(x,op,eval(t->children[i]));
		i++;
	}

return x;

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
			lval result = eval(r.output);
			lval_println(result);
			mpc_ast_delete(r.output);
		}
		
		
		free(input);
	}
	
	mpc_cleanup(4,Number,Operator,Expr,Lisp);

	return 0;
}
