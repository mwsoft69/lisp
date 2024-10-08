#ifndef LVAL_H
#define LVAL_H
/*Enums of possible lval.*/
enum{LVAL_NUM, LVAL_ERR,LVAL_SYM,LVAL_SEXPR};

/*Enum for posible error types.*/
enum {LERR_DIV_ZERO,LERR_BAD_OP,LERR_BAD_NUM};



/*Struct for lval*/
typedef struct {
	int type;
	float num;
	/*Error and symbol types.*/
	char * err;
	char* sym;
	/*Count and pointer to a list of lvals*/
	int count;
	struct lval** cell;
	
} lval;


lval* lval_num(float x);
lval* lval_err(int x);
void lval_print(lval v);
void lval_println(lval v);

#endif /* LVAL_H */
