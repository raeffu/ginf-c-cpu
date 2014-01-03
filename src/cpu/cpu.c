/*
  cpu.c
  bho1 10.7.2011

  shameless copy of initial cpu.c
  6502 CPU emulation
  
  GPL applies
*/



#include <stdio.h>
#include <string.h>

#include "register.h"
#include "alu.h"
#include "memory.h"

#include <SDL.h>
//#include "monitor.h"
//#include "cpu-util.h"

#include "decode.h"

#include "mu-code.h" //this is needed for the reset cmd. Could re-engineering lookup_operation with a reset (or execption funct. pointer in case op in NULL

#include "cpu-util.h"
#include "cpu.h"

int keyboardchar;


/*
  Registers used in 6502 CPU
*/

/*
  hidden register
*/
// Databus Register, shared with memory.c
char *dbr;
// Addressbus Register, shared with memory.c
char *abrl;
char *abrh;
// Read/Write bit
char read_write;
//Instruction Register
char *ir;

// cycles
int cycles_sum;
int cycles;
/*
  User register
*/
// Accumulator
char *acc;

// index register
char *idx;
char *idy
;
// stack pointer SP
char *sp;

// programm counter
char *pcl;
char *pch;

// flags
char *flags;


// ultra helpers
char *zero, *one;


/* 
   TODO: 
   trace function: Originally used to copy all register on stack 
   and jump to assembler debugger subroutine.
   
   Here we missuse it to debug our Processor
*/
void trace(){

}

void cpu_6502_init(){
  
  abrl = malloc(sizeof(char)*REG_WIDTH+1);
  abrh = malloc(sizeof(char)*REG_WIDTH+1);
  dbr = malloc(sizeof(char)*REG_WIDTH+1);
  
  acc = malloc(sizeof(char)*REG_WIDTH+1);
  idx = malloc(sizeof(char)*REG_WIDTH+1);
  idy = malloc(sizeof(char)*REG_WIDTH+1);
  sp = malloc(sizeof(char)*REG_WIDTH+1);
  pcl = malloc(sizeof(char)*REG_WIDTH+1);
  pch = malloc(sizeof(char)*REG_WIDTH+1);
  flags = malloc(sizeof(char)*REG_WIDTH+1);
  ir = malloc(sizeof(char)*REG_WIDTH+1);

  // helpers
  zero = malloc(sizeof(char)*REG_WIDTH+1);
  one = malloc(sizeof(char)*REG_WIDTH+1);

  cycles = 0;
  cycles_sum = 0;
}

void cpu_6502_reset(){
  cp_register("00000000", abrl);
  cp_register("00000000", abrh);
  cp_register("00000000", dbr);
  
  cp_register("10101010", acc);
  cp_register("00000000", idx);
  cp_register("00000000", idy);
  cp_register("11111101", sp);
  cp_register("00000000", pcl);
  cp_register("00000000", pch);
  cp_register("00010110", flags);
  cp_register("00000000", ir);

  // helpers
  cp_register("00000000", zero);
  cp_register("00000001", one);
}


/*
  fetch()
  load instruction register with content (hopefully a instruction)
  from memory at address <pc>, then adjust pc
  ir <-- mem[pc++]
*/
void fetch(){
  cp_register(pcl,abrl);
  cp_register(pch,abrh);
  set_rw2read();
  access_memory();

  cp_register(dbr,ir);
  
  inc_pc();  
}
/*
  get structure pointer corresponding to ir content
*/
void* getop(char* reg){
  int instr_code;
  instr_code = conv_bitstr2int(reg,0,7);
  return operation_lookup(instr_code);
}

/*
  decode function:
  analyse IR-Register and return corresponding function
void (*decode()){
*/
void *decode(){
  struct cpu_operator* op;

  //  instr_code = conv_bitstr2int(ir,0,7);
  op =   getop(ir);//operation_lookup(instr_code);
  if(op)
	return op->opfunction;
  else
	return NULL;  //TODO: i) creating cpu exception ii) with parameter
}

//void save_cpu(void (*fct)()){
/*
  execute mu-function void fct()
*/
void execute(void (*fct)()){
  if(fct){
	(*fct)();
  } else {
	exit(-1);
  }
  cycles_sum += cycles;
}

void fetch_decode_execute_cycle(){
  void (*mue_code_fct_ptr)();

  while(1){
    fetch();
    mue_code_fct_ptr = decode();
	execute(mue_code_fct_ptr);
	//	monitor("Executed");
  }
}


