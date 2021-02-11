/*
    Remove all unnecessary lines (including this one) 
    in this comment.
    REFER TO THE SUBMISSION INSTRUCTION FOR DETAILS

    Name 1: guy farmer
    Name 2: michael hilborn
    UTEID 1: gcf375
    UTEID 2: mth2433
*/

/***************************************************************/
/*                                                             */
/*   LC-3b Instruction Level Simulator                         */
/*                                                             */
/*   EE 460N                                                   */
/*   The University of Texas at Austin                         */
/*                                                             */
/***************************************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/***************************************************************/
/*                                                             */
/* Files: isaprogram   LC-3b machine language program file     */
/*                                                             */
/***************************************************************/

/***************************************************************/
/* These are the functions you'll have to write.               */
/***************************************************************/

void process_instruction();

/***************************************************************/
/* A couple of useful definitions.                             */
/***************************************************************/
#define FALSE 0
#define TRUE  1

/***************************************************************/
/* Use this to avoid overflowing 16 bits on the bus.           */
/***************************************************************/
#define Low16bits(x) ((x) & 0xFFFF)

/***************************************************************/
/* Main memory.                                                */
/***************************************************************/
/* MEMORY[A][0] stores the least significant byte of word at word address A
   MEMORY[A][1] stores the most significant byte of word at word address A 
*/

#define WORDS_IN_MEM    0x08000 
int MEMORY[WORDS_IN_MEM][2];

/***************************************************************/

/***************************************************************/

/***************************************************************/
/* LC-3b State info.                                           */
/***************************************************************/
#define LC_3b_REGS 8

int RUN_BIT;	/* run bit */


typedef struct System_Latches_Struct{

  int PC,		/* program counter */
    N,		/* n condition bit */
    Z,		/* z condition bit */
    P;		/* p condition bit */
  int REGS[LC_3b_REGS]; /* register file. */
} System_Latches;

/* Data Structure for Latch */

System_Latches CURRENT_LATCHES, NEXT_LATCHES;

/***************************************************************/
/* A cycle counter.                                            */
/***************************************************************/
int INSTRUCTION_COUNT;

/***************************************************************/
/*                                                             */
/* Procedure : help                                            */
/*                                                             */
/* Purpose   : Print out a list of commands                    */
/*                                                             */
/***************************************************************/
void help() {                                                    
  printf("----------------LC-3b ISIM Help-----------------------\n");
  printf("go               -  run program to completion         \n");
  printf("run n            -  execute program for n instructions\n");
  printf("mdump low high   -  dump memory from low to high      \n");
  printf("rdump            -  dump the register & bus values    \n");
  printf("?                -  display this help menu            \n");
  printf("quit             -  exit the program                  \n\n");
}

/***************************************************************/
/*                                                             */
/* Procedure : cycle                                           */
/*                                                             */
/* Purpose   : Execute a cycle                                 */
/*                                                             */
/***************************************************************/
void cycle() {                                                

  process_instruction();
  CURRENT_LATCHES = NEXT_LATCHES;
  INSTRUCTION_COUNT++;
}

/***************************************************************/
/*                                                             */
/* Procedure : run n                                           */
/*                                                             */
/* Purpose   : Simulate the LC-3b for n cycles                 */
/*                                                             */
/***************************************************************/
void run(int num_cycles) {                                      
  int i;

  if (RUN_BIT == FALSE) {
    printf("Can't simulate, Simulator is halted\n\n");
    return;
  }

  printf("Simulating for %d cycles...\n\n", num_cycles);
  for (i = 0; i < num_cycles; i++) {
    if (CURRENT_LATCHES.PC == 0x0000) {
	    RUN_BIT = FALSE;
	    printf("Simulator halted\n\n");
	    break;
    }
    cycle();
  }
}

/***************************************************************/
/*                                                             */
/* Procedure : go                                              */
/*                                                             */
/* Purpose   : Simulate the LC-3b until HALTed                 */
/*                                                             */
/***************************************************************/
void go() {                                                     
  if (RUN_BIT == FALSE) {
    printf("Can't simulate, Simulator is halted\n\n");
    return;
  }

  printf("Simulating...\n\n");
  while (CURRENT_LATCHES.PC != 0x0000)
    cycle();
  RUN_BIT = FALSE;
  printf("Simulator halted\n\n");
}

/***************************************************************/ 
/*                                                             */
/* Procedure : mdump                                           */
/*                                                             */
/* Purpose   : Dump a word-aligned region of memory to the     */
/*             output file.                                    */
/*                                                             */
/***************************************************************/
void mdump(FILE * dumpsim_file, int start, int stop) {          
  int address; /* this is a byte address */

  printf("\nMemory content [0x%.4x..0x%.4x] :\n", start, stop);
  printf("-------------------------------------\n");
  for (address = (start >> 1); address <= (stop >> 1); address++)
    printf("  0x%.4x (%d) : 0x%.2x%.2x\n", address << 1, address << 1, MEMORY[address][1], MEMORY[address][0]);
  printf("\n");

  /* dump the memory contents into the dumpsim file */
  fprintf(dumpsim_file, "\nMemory content [0x%.4x..0x%.4x] :\n", start, stop);
  fprintf(dumpsim_file, "-------------------------------------\n");
  for (address = (start >> 1); address <= (stop >> 1); address++)
    fprintf(dumpsim_file, " 0x%.4x (%d) : 0x%.2x%.2x\n", address << 1, address << 1, MEMORY[address][1], MEMORY[address][0]);
  fprintf(dumpsim_file, "\n");
  fflush(dumpsim_file);
}

/***************************************************************/
/*                                                             */
/* Procedure : rdump                                           */
/*                                                             */
/* Purpose   : Dump current register and bus values to the     */   
/*             output file.                                    */
/*                                                             */
/***************************************************************/
void rdump(FILE * dumpsim_file) {                               
  int k; 

  printf("\nCurrent register/bus values :\n");
  printf("-------------------------------------\n");
  printf("Instruction Count : %d\n", INSTRUCTION_COUNT);
  printf("PC                : 0x%.4x\n", CURRENT_LATCHES.PC);
  printf("CCs: N = %d  Z = %d  P = %d\n", CURRENT_LATCHES.N, CURRENT_LATCHES.Z, CURRENT_LATCHES.P);
  printf("Registers:\n");
  for (k = 0; k < LC_3b_REGS; k++)
    printf("%d: 0x%.4x\n", k, CURRENT_LATCHES.REGS[k]);
  printf("\n");

  /* dump the state information into the dumpsim file */
  fprintf(dumpsim_file, "\nCurrent register/bus values :\n");
  fprintf(dumpsim_file, "-------------------------------------\n");
  fprintf(dumpsim_file, "Instruction Count : %d\n", INSTRUCTION_COUNT);
  fprintf(dumpsim_file, "PC                : 0x%.4x\n", CURRENT_LATCHES.PC);
  fprintf(dumpsim_file, "CCs: N = %d  Z = %d  P = %d\n", CURRENT_LATCHES.N, CURRENT_LATCHES.Z, CURRENT_LATCHES.P);
  fprintf(dumpsim_file, "Registers:\n");
  for (k = 0; k < LC_3b_REGS; k++)
    fprintf(dumpsim_file, "%d: 0x%.4x\n", k, CURRENT_LATCHES.REGS[k]);
  fprintf(dumpsim_file, "\n");
  fflush(dumpsim_file);
}

/***************************************************************/
/*                                                             */
/* Procedure : get_command                                     */
/*                                                             */
/* Purpose   : Read a command from standard input.             */  
/*                                                             */
/***************************************************************/
void get_command(FILE * dumpsim_file) {                         
  char buffer[20];
  int start, stop, cycles;

  printf("LC-3b-SIM> ");

  scanf("%s", buffer);
  printf("\n");

  switch(buffer[0]) {
  case 'G':
  case 'g':
    go();
    break;

  case 'M':
  case 'm':
    scanf("%i %i", &start, &stop);
    mdump(dumpsim_file, start, stop);
    break;

  case '?':
    help();
    break;
  case 'Q':
  case 'q':
    printf("Bye.\n");
    exit(0);

  case 'R':
  case 'r':
    if (buffer[1] == 'd' || buffer[1] == 'D')
	    rdump(dumpsim_file);
    else {
	    scanf("%d", &cycles);
	    run(cycles);
    }
    break;

  default:
    printf("Invalid Command\n");
    break;
  }
}

/***************************************************************/
/*                                                             */
/* Procedure : init_memory                                     */
/*                                                             */
/* Purpose   : Zero out the memory array                       */
/*                                                             */
/***************************************************************/
void init_memory() {                                           
  int i;

  for (i=0; i < WORDS_IN_MEM; i++) {
    MEMORY[i][0] = 0;
    MEMORY[i][1] = 0;
  }
}

/**************************************************************/
/*                                                            */
/* Procedure : load_program                                   */
/*                                                            */
/* Purpose   : Load program and service routines into mem.    */
/*                                                            */
/**************************************************************/
void load_program(char *program_filename) {                   
  FILE * prog;
  int ii, word, program_base;

  /* Open program file. */
  prog = fopen(program_filename, "r");
  if (prog == NULL) {
    printf("Error: Can't open program file %s\n", program_filename);
    exit(-1);
  }

  /* Read in the program. */
  if (fscanf(prog, "%x\n", &word) != EOF)
    program_base = word >> 1;
  else {
    printf("Error: Program file is empty\n");
    exit(-1);
  }

  ii = 0;
  while (fscanf(prog, "%x\n", &word) != EOF) {
    /* Make sure it fits. */
    if (program_base + ii >= WORDS_IN_MEM) {
	    printf("Error: Program file %s is too long to fit in memory. %x\n",
             program_filename, ii);
	    exit(-1);
    }

    /* Write the word to memory array. */
    MEMORY[program_base + ii][0] = word & 0x00FF;
    MEMORY[program_base + ii][1] = (word >> 8) & 0x00FF;
    ii++;
  }

  if (CURRENT_LATCHES.PC == 0) CURRENT_LATCHES.PC = (program_base << 1);

  printf("Read %d words from program into memory.\n\n", ii);
}

/************************************************************/
/*                                                          */
/* Procedure : initialize                                   */
/*                                                          */
/* Purpose   : Load machine language program                */ 
/*             and set up initial state of the machine.     */
/*                                                          */
/************************************************************/
void initialize(char *program_filename, int num_prog_files) { 
  int i;

  init_memory();
  for ( i = 0; i < num_prog_files; i++ ) {
    load_program(program_filename);
    while(*program_filename++ != '\0');
  }
  CURRENT_LATCHES.Z = 1;  
  NEXT_LATCHES = CURRENT_LATCHES;
    
  RUN_BIT = TRUE;
}

/***************************************************************/
/*                                                             */
/* Procedure : main                                            */
/*                                                             */
/***************************************************************/
int main(int argc, char *argv[]) {                              
  FILE * dumpsim_file;

  /* Error Checking */
  if (argc < 2) {
    printf("Error: usage: %s <program_file_1> <program_file_2> ...\n",
           argv[0]);
    exit(1);
  }

  printf("LC-3b Simulator\n\n");

  initialize(argv[1], argc - 1);

  if ( (dumpsim_file = fopen( "dumpsim", "w" )) == NULL ) {
    printf("Error: Can't open dumpsim file\n");
    exit(-1);
  }

  while (1)
    get_command(dumpsim_file);
    
}

/***************************************************************/
/* Do not modify the above code.
   You are allowed to use the following global variables in your
   code. These are defined above.

   MEMORY

   CURRENT_LATCHES
   NEXT_LATCHES

   You may define your own local/global variables and functions.
   You may use the functions to get at the control bits defined
   above.

   Begin your code here 	  			       */

/***************************************************************/


void decode(int instruction);
void execute(int instruction);
void update(int instruction);
int sign_extend(int offset, int size);
int opcode,dr, sr1, sr2, steering_bit, imm5, off9, baser, off11, off6, amount4, nzp_bits, trap_vector;
void process_instruction(){
  /*  function: process_instruction
   *  
   *    Process one instruction at a time  
   *       -Fetch one instruction
   *       -Decode 
   *       -Execute
   *       -Update NEXT_LATCHES
   */  


  // fetch 
  int instruction = MEMORY[CURRENT_LATCHES.PC/2][0] | (MEMORY[CURRENT_LATCHES.PC/2][1]<<8); 
  NEXT_LATCHES.PC = CURRENT_LATCHES.PC +2;
  opcode = (instruction & 0xf000)>>12;
  decode(instruction);
  execute(instruction);
  update(instruction);

}

void decode(int instruction){
  
  // add 
  if(opcode == 1){
    dr = (instruction & 0x0e00) >> 9;
    sr1 = (instruction & 0x01c0) >>6;
    steering_bit = (instruction & 0x0020) >>5;
    sr2 = instruction & 0x0007;
    imm5 = instruction & 0x001f;
    if(instruction & 0x0010){
      imm5 = imm5 | 0xffe0;
    }
  }//ldb instruction
  else if(opcode == 2){
    dr = (instruction & 0x0e00)>>9;
    baser = (instruction & 0x01c0) >>6;
    off6 = (instruction & 0x003F);
    if((instruction & 0x0020)){
      off6 = off6 | 0xffc0;
    }
  }//stb instruction
  else if(opcode == 3){
    sr1 = (instruction & 0x0e00)>>9;
    baser = (instruction & 0x01c0) >>6;
    off6 = (instruction & 0x003F);
    if((instruction & 0x0020)){
      off6 = off6 | 0xffc0;
    }
  }//JSR Instruction
  else if(opcode == 4){
    steering_bit = (instruction & 0x0800)>>11;
    if(steering_bit){
      off11 = (instruction & 0x07FF);
      if(instruction & 0x0400){
        off11 = off11 | 0xF800;
      }
    }else{
      baser = (instruction & 0x01C0)>>6;
    }
  }//And Instruction
  else if(opcode == 5){
    dr = (instruction & 0x0e00) >> 9;
    sr1 = (instruction & 0x01c0) >>6;
    steering_bit = (instruction & 0x0020) >>5;
    sr2 = instruction & 0x0007;
    imm5 = instruction & 0x001f;
    if(instruction & 0x0010){
      imm5 = imm5 | 0xffe0;
    }
  }//ldw instruction
  else if(opcode == 6){
    dr = (instruction & 0x0e00)>>9;
    baser = (instruction & 0x01c0) >>6;
    off6 = (instruction & 0x003F);
    if((instruction & 0x0020)){
      off6 = off6 | 0xffc0;
    }
  }//stw instruction
  else if(opcode == 7){
    sr1 = (instruction & 0x0e00)>>9;
    baser = (instruction & 0x01c0) >>6;
    off6 = (instruction & 0x003F);
    if((instruction & 0x0020)){
      off6 = off6 | 0xffc0;
    }
  }//rti instruction
  else if(opcode == 8){
    //do nothing
  }//xor instruction
  else if(opcode == 9){
    dr = (instruction & 0x0e00) >> 9;
    sr1 = (instruction & 0x01c0) >>6;
    steering_bit = (instruction & 0x0020) >>5;
    sr2 = instruction & 0x0007;
    imm5 = instruction & 0x001f;
    if(instruction & 0x0010){
      imm5 = imm5 | 0xffe0;
    }
  }//not used
  else if(opcode == 10){
    //do nothing
  }//not used
  else if(opcode == 11){
    //do nothing
  }//jmp instruction
  else if(opcode == 12){
    baser = (instruction & 0x01c0)>>6;
  }//shf instruction
  else if(opcode == 13){
    dr = (instruction & 0x0e00) >> 9;
    sr1 = (instruction & 0x01c0) >>6;
    steering_bit = (instruction & 0x0030)>>4;
    amount4 = instruction & 0x000F;
  }//lea
  else if(opcode == 14){
    baser = (instruction & 0x0e00)>>9;
    off9 = (instruction & 0x01ff);
    if(instruction & 0x0100){
      off9 = off9 | 0xFE00;
    }
  }// trap
  else if(opcode == 15){
    trap_vector = instruction & 0x00FF;
  }//branch instruction
  else if(opcode == 0){
    nzp_bits = (instruction & 0x0e00)>>9;
    off9 = (instruction & 0x01ff);
    if(instruction & 0x0100){
      off9 = off9 | 0xFE00;
    }
  }
}
void execute(int instruction){
  // branch
  if(opcode == 0){
    int nzp = (CURRENT_LATCHES.N<<2) | (CURRENT_LATCHES.Z<<1) | CURRENT_LATCHES.P;
    if(nzp == nzp_bits){
      NEXT_LATCHES.PC = CURRENT_LATCHES.PC + (sign_extend(off9,9)<<1);
    }
    else NEXT_LATCHES.PC = CURRENT_LATCHES.PC + 2;
  } // add 
  else if(opcode == 1){
    if(!steering_bit){
      NEXT_LATCHES.REGS[dr] = CURRENT_LATCHES.REGS[sr1] + CURRENT_LATCHES.REGS[sr2];
    }
    else NEXT_LATCHES.REGS[dr] = CURRENT_LATCHES.REGS[sr1] + sign_extend(imm5, 5);
  }// ldb
  else if(opcode == 2){
    NEXT_LATCHES.REGS[dr] = MEMORY[(CURRENT_LATCHES.REGS[baser] + (sign_extend(off6,6)*2))/2][(CURRENT_LATCHES.REGS[baser] + (sign_extend(off6,6))*2)%2];
  }// stb
  else if(opcode == 3){
    MEMORY[(CURRENT_LATCHES.REGS[baser]+ sign_extend(off6,6))/2][(CURRENT_LATCHES.REGS[baser]+ sign_extend(off6,6))%2] = (CURRENT_LATCHES.REGS[sr1] & 0x00ff);
  }// jsr / jsrr
  else if(opcode == 4){
    NEXT_LATCHES.REGS[7] = NEXT_LATCHES.PC;
    if(!steering_bit){
      NEXT_LATCHES.PC = CURRENT_LATCHES.REGS[baser];
    }else{
      NEXT_LATCHES.PC = NEXT_LATCHES.PC + (sign_extend(off11,11))<<1;
    }
  }// and 
  else if(opcode == 5){
    if(!steering_bit){
      NEXT_LATCHES.REGS[dr] = CURRENT_LATCHES.REGS[sr1] & CURRENT_LATCHES.REGS[sr2];
    }
    else NEXT_LATCHES.REGS[dr] = CURRENT_LATCHES.REGS[sr1] & sign_extend(imm5, 5); 
  }// ldw
  else if(opcode == 6){
    NEXT_LATCHES.REGS[dr] = (MEMORY[(CURRENT_LATCHES.REGS[baser] + (sign_extend(off6,6))*2)/2][1]<<8)
                            | (MEMORY[(CURRENT_LATCHES.REGS[baser] + (sign_extend(off6,6))*2)/2][0]);
  }// stw
  else if(opcode == 7){
    MEMORY[(CURRENT_LATCHES.REGS[baser]+ sign_extend(off6,6))/2][1] = (CURRENT_LATCHES.REGS[sr1] & 0x00ff);
    MEMORY[(CURRENT_LATCHES.REGS[baser]+ sign_extend(off6,6))/2][0] = (CURRENT_LATCHES.REGS[sr1] & 0xff00);
  }// rti
  else if(opcode == 8){
    //do nothing
  }// not / xor
  else if(opcode == 9){
    if(!steering_bit){
      NEXT_LATCHES.REGS[dr] = CURRENT_LATCHES.REGS[sr1] ^ CURRENT_LATCHES.REGS[sr2];
    }
    else NEXT_LATCHES.REGS[dr] = CURRENT_LATCHES.REGS[sr1] ^ sign_extend(imm5, 5);
  }// n/a 
  else if(opcode == 10){
    //do nothing
  }// n/a
  else if(opcode == 11){
    //do nothing
  }// ret
  else if(opcode == 12){
    NEXT_LATCHES.PC = CURRENT_LATCHES.REGS[7];
  }// shf
  else if(opcode == 13){
    if(steering_bit == 0){
      NEXT_LATCHES.REGS[dr] = CURRENT_LATCHES.REGS[sr1] << amount4;
    }
    else if(steering_bit == 1){
      NEXT_LATCHES.REGS[dr] = CURRENT_LATCHES.REGS[sr1] >> amount4;
    }
    else if(steering_bit == 3){
      NEXT_LATCHES.REGS[dr] = CURRENT_LATCHES.REGS[sr1] >> amount4;
      sign_extend(NEXT_LATCHES.REGS[dr], 16-amount4);
    }
  }// lea
  else if(opcode == 14){
    NEXT_LATCHES.REGS[dr] = NEXT_LATCHES.PC + (sign_extend(off9, 9)<<1)
  }// trap
  else if(opcode == 15){
    
  }
};
void update(int instruction){};
int sign_extend(int offset, int size){
  if(offset>>size){
    return offset | (0xffff<<(16-size));
  }
  else return offset;
}
