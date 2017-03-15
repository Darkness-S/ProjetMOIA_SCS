typedef enum {
	OK,
	OP_INCORRECT,
	OP_OVERFLOW,
	DIV_ZERO,
	OP_INCONNU
}TCodeRep;    

typedef enum {
	SUM,
	SUB,
	MULT,
	DIV
}TCodeOp;

typedef struct {
    TCodeOp oprt;
    int opd1, opd2;
}TOperation;

typedef struct {
    TCodeRep codeErr;
    int res;
}TOpRes;
