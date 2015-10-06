/*declared functions*/
int compile(int *, int*, int*, int*, int*, int*);
void execute(int*, int*, int*, int*, int*, int*);
int operationC(int*, char*);
void halt(int m[], int*, int*, int*, int*, int*);
int compile_error(int *);

/*line holder and pointer*/
char str[60];
char *ptr;

int cc = 0;            /*accumulator*/
int inst;              /*instructionCounter(memory address)*/
int inst_register = 0; /*instruction command name e.g. READ*/

/*to split data retrieved from memory*/
int op_code = -1;      /*code of command e.g. if READ then 10*/
int operand = -1;      /*value is used by execute*/

/*memory space*/
int memory[100];
