#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "declaration_variables.h"
#define EXEC() execute(m, count, oc, oprnd, acc, ireg);
#define HALT() halt(m, acc, count, ireg, oc, oprnd);

 /*
 Program Virtual Machine
 
 Collaborators: None.
 */

int main (void) {
  cc = compile(&inst_register, memory, &cc, &inst, &op_code, &operand);
  if(cc == 1) {
    stdin = fopen("/dev/tty","r");
    cc = 0;
    inst = 0;
    execute(memory, &inst, &op_code, &operand, &cc, &inst_register);
  } 
  return 1;
}

/*accepts the written program*/
int compile (int *ireg, int *m, int *acc, int *count, int *oc, int *oprnd) {
  /*fills the 2d array with zeros*/
  while(sizeof(memory)/sizeof(*m) > (*count)++) {
      m[*count] = 0;
  }
  /*error -4 is no HALT use*/
  *acc = -4;
  /*gets user input & stores it as digits in memory*/
  ptr = "is empty now";
  while(fgets (str, 60, stdin) != NULL) {
    /*gets address to ptr, then to counter*/
    ptr = strtok(str, " ");
    *count = strtol(str, &ptr, 10);
    /*gets instruction to ptr then to register*/
    ptr = strtok(NULL, " ");
    *ireg = *ptr;
    /*checks for unknown command*/
    if(operationC(oc, ptr) == -1) {
        *acc = -1;
      return compile_error(acc);
    }
    /*gets operand from str*/
    ptr = strtok(NULL, "\n");
    *oprnd = strtol(ptr, &ptr, 10);
    /*stores numeric values into current address as
    long as there is no more than 4 digits to store*/
    if((100*(*oc) + *oprnd) < 10000 && (100*(*oc) + *oprnd) > -10000) {
      m[*count] = 100*(*oc) + *oprnd;
    } else {
        *acc=-2;
      return compile_error(acc);
    }

    if(*oc == 99 && *oprnd == 99) { *acc = 1; }
  }

  /*verification of HALT() use within compile*/
  if(*acc == -4) {
    return compile_error(acc);
  } else {
    return 1;
  }
}

/*converts string instruction like READ to its code value
  if there is not code returns error -1*/
int operationC(int *oC, char *p) {
  *oC = -1;
  if(strcmp(p, "READ") == 0) {  *oC =10;  }
  if(strcmp(p, "LOAD") == 0) {  *oC =20;  }
  if(strcmp(p, "PRNT") == 0) {  *oC =12;  }
  if(strcmp(p, "HALT") == 0) {  *oC =99;  }
  if(strcmp(p, "SET" ) == 0) {  *oC =0;   }
  if(strcmp(p, "SUB" ) == 0) {  *oC =31;  }
  if(strcmp(p, "BRNG") == 0) {  *oC =41;  }
  if(strcmp(p, "WRIT") == 0) {  *oC =11;  }
  if(strcmp(p, "STOR") == 0) {  *oC =21;  }
  if(strcmp(p, "ADD" ) == 0) {  *oC =30;  }
  if(strcmp(p, "DIV" ) == 0) {  *oC =32;  }
  if(strcmp(p, "MULT") == 0) {  *oC =33;  }
  if(strcmp(p, "MOD" ) == 0) {  *oC =34;  }
  if(strcmp(p, "BRAN") == 0) {  *oC =40;  }
  if(strcmp(p, "BRZR") == 0) {  *oC =42;  }

  return *oC;
}

/*executes what is stored in memory*/
void execute(int *m, int *count, int *oc, int *oprnd, int *acc, int *ireg) {
  /*digits divide further into the instruction and memory
  address that is used by the instruction*/
  *ireg = m[*count];
  *oc = *ireg/100;
  *oprnd = *ireg%100;
  /*if(*oc > 9999) { *count=101;}*/
  while(100 > *count && m[*count] != 0) {
    /*printf("%d", *count);*/
    if(*acc > 9999) {
      printf("\n -word overflow: attempt to store in accumulator1!\n\n");
      *count= 100;
    }
    if(*oc == 99 && *oprnd == 99) { /*for HALT*/
      HALT()
      *count=100;
      EXEC() /*defined Macro*/
    }
    if(*oc == 12) { /*for PRINT*/
      while(*ireg != 0) {
        *oc = m[*oprnd]/100;
        *ireg = m[*oprnd]%100;
        if(*oc==0||*oc==10||(*oc<91&&*oc>68)) {
          /*print casted op_code and operand*/
          printf("%c%c", (char)*oc, (char)*ireg);
          (*oprnd)++;
        } else {
          *ireg = 0;
          *count=100;
          printf("\n Unknown Character Reached while printing\n\n");
        }
      }
      (*count)++;
      EXEC()
    }
    if(*oc == 10) { /*for READ*/
        scanf("%d", &m[*oprnd]);
      if(!isdigit(m[*oprnd]) && m[*oprnd] < 10000 && m[*oprnd] > -10000) {
        (*count)++;
        EXEC()
      } else {
        *count=100;
        printf("\n-word overflow: attempt to store in memory!\n\n");
        EXEC()
      }
    }
    if(*oc == 20) { /*for LOAD*/
      if(m[*oprnd] < 10000) {
        *acc = m[*oprnd];
      } else {
        *count=100;
        printf("\n -word overflow: attempt to store in accumulator2!\n\n");
      }
      (*count)++;
      EXEC()
    }

    if(*oc == 21) { /*for STOR*/
      if(*acc < 10000) {
        m[*oprnd] = *acc;
      } else {
        *count=100;
        printf("\n -word overflow: attempt to store in memory! Memory cannot hold\n\n");
      }
      (*count)++;
      EXEC()
    }
    if(*oc == 30) { /*for ADD*/
      if(*acc+m[*oprnd] < 10000) {
        *acc += m[*oprnd];
      } else {
        *count=100;
        printf("\n -word overflow: attempt to store in accumulator. Check your code file!\n\n");
      }
      (*count)++;
      EXEC()
    }
    if(*oc == 32) { /*for DIV*/
      if(m[*oprnd] == 0) {
        *count=100;
        printf("\n -divide by 0 was attempted\n\n");
      } else {
        *acc /= m[*oprnd];
        (*count)++;
      }
      EXEC()
    }
    if(*oc == 33) { /*for MULT*/
      *acc *= m[*oprnd];
      (*count)++;
      EXEC()
    }
    if(*oc == 34) { /*for MOD*/
      *acc %= m[*oprnd];
      (*count)++;
      EXEC()
    }
    if(*oc == 11) { /*for WRIT*/
      printf("%d", m[*oprnd]);
      (*count)++;
      EXEC()
    }
    if(*oc == 31) { /*for SUB*/
      if(*acc-m[*oprnd] < 10000) {
        *acc -= m[*oprnd];
      } else {
        *count=100;
        printf("\n -word overflow: attempt to store in accumulator4!\n\n");
      }
      (*count)++;
      EXEC()
    }
    if(*oc == 40) { /*for BRAN*/
      *count = *oprnd;
      EXEC()
    }
    if(*oc == 41) { /*for BRNG*/
      /*printf("line225: %d", *acc);*/
      if(*acc < 0) {
        *count = *oprnd;
        EXEC()
      } else {
        (*count)++;
        EXEC()
      }
    }
    if(*oc == 42) { /*for BRZR*/
      if(*acc == 0) {
        *count = *oprnd;
        EXEC()
      } else {
        (*count)++;
        EXEC()
      }
    }
    (*count)++;
  }
}

/*uses HALT to print out the state of memory*/
void halt(int *m, int *acc, int *count, int *inst_reg, int *oc, int *oprnd) {

  printf ("\nREGISTERS:\n");
  *acc>-1?printf ("accumulator %20c", '+'):printf("accumulator %19c", ' ');
  printf("%04d\n", *acc);
  printf ("instructionCounter %15c%02d\n", ' ', *count);
  printf ("instructionRegister %12c%04d\n", '+', *inst_reg);
  printf ("operationCode %20c%02d\n", ' ', *oc);
  printf ("instructionCounter %15c%02d\n", ' ', *oprnd);
  printf ("MEMORY:\n");
  printf ("%9d%6d%6d%6d%6d%6d%6d%6d%6d%6d", 0,1,2,3,4,5,6,7,8,9);
  *count =0;

  /*
     size of array cannot be found using a pointer since *m does not know what
     it is pointing to. Therefore, I used the array memory as a global variable 
     in order to avoid bad programming practices e.g. using the known value of 
     400 to divide by size of integer, four, or for that matter use integer 100.
  */
  while(sizeof(memory)/sizeof(*m) > *count) {
    if(*count%10 == 0)
      printf("\n%2d  ", *count);
    if(m[*count] > -1) {
      printf ("%c%.04d ", '+', m[*count]);
    } else {
      printf ("%.04d ", m[*count]);
    }
    (*count)++;
  }
  printf("\n");
}

int compile_error(int *acc) {
  if(*acc == -3) {
    printf("\nUndefined use - command is not in the proper format\n\n");
  } else if(*acc == -1) {
    printf("\nUnknown command - unrecognized command word\n\n");
  } else if(*acc == -2) {
    printf("\n -word overflow - attempts to place a word in memory that is larger than 4 digits\n\n");
  } else if(*acc == -4) {
    printf("\n HALT- no HALT command is ever given\n\n");
  }
  return *acc;
}
