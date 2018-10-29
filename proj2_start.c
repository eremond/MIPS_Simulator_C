/* Emmanuel Gonzalez - Project 2 - CDA3101

MIPS Simulator:

Things to Do:

* Implement Pipeline Forwarding so Info can get to the end (Like Test Cases)
* Implement Data Hazard Detection
* Implement Halts / Forwarding for Data Hazards
* Implement BEQ Hazard Fixes

Due Date: Wednesday 11:50PM
-10% : Thursday 11:50PM
-20% : Friday 11:50PM


*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMMEMS 16
#define NUMREGS 8

/* Opcode values for instructions */
#define AL 0
#define LW  35
#define SW  43
#define BEQ 4
#define HALT 63

/* Funct values for R-type instructions */
#define ADD 32
#define SUB 34

typedef struct IFIDReg {
    unsigned int instr;		/* Integer representation of instruction */
    int	  PCPlus4;		/* PC + 4 */
}     IFIDReg_t;

typedef struct IDEXReg {
    unsigned int instr;		/* Integer representation of instruction */
    int	  PCPlus4;		/* PC + 4 */
    int	  readData1;		/* Contents of rs register */
    int	  readData2;		/* Contents of rt register */
    int	  immed;		/* Immediate field */
    int	  rsReg;		/* rs register number */
    int	  rtReg;		/* rt register number */
    int	  rdReg;		/* rd register number */
    int	  branchTarget;		/* Branch target calculated */
}     IDEXReg_t;

typedef struct EXMEMReg {
    unsigned int instr;		/* Integer representation of instruction */
    int	  ALUoutput;		/* Result of ALU operation */
    int	  rtRegData;		/* Contents of the rt register */
    int	  writeRegNum;		/* The destination register */
}     EXMEMReg_t;

typedef struct MEMWBReg {
    unsigned int instr;		/* Integer representation of instruction */
    int	  ALUoutput;		/* Result from ALU operation */
    int	  memOutput;		/* Data read from memory */
    int	  writeRegNum;		/* The destination register */
}     MEMWBReg_t;

typedef struct mipsState {
    int	  PC;			/* Program Counter */
    unsigned int instrMem[NUMMEMS];	/* Instruction memory */
    int	  dataMem[NUMMEMS];	/* Data memory */
    int	  regFile[NUMREGS];	/* Register file */

    IFIDReg_t IFID;		/* Current IFID pipeline register */
    IDEXReg_t IDEX;		/* Current IDEX pipeline register */
    EXMEMReg_t EXMEM;		/* Current EXMEM pipeline register */
    MEMWBReg_t MEMWB;		/* Current MEMWB pipeline register */

    int	  cycles;		/* Number of cycles executed so far */
    int	  forwards;		/* Number of forwarding steps performed */
    int	  stalls;		/* Number of stall cycles executed */
    int	  branches;		/* Number of branch instructions so far */
    int	  taken;		/* Number of taken branches so far */

}     mipsState_t;

void  detectHazards (mipsState_t *);
void  simIFStage (mipsState_t *, int);
void  simIDStage (mipsState_t *);
void  simEXStage (mipsState_t *);
void  simMEMStage (mipsState_t *);
void  simWBStage (mipsState_t *);

void  printState (mipsState_t *);
void  initState (mipsState_t *);
unsigned int instrToInt (char *, char *);
void  printInstruction (unsigned int);

#define get_rs(i) (((i >> 21) & 0x1F))
#define get_rt(i) (((i >> 16) & 0x1F))
#define get_rd(i) (((i >> 11) & 0x1F))

#define get_funct(i) ((i & 0x3F))
#define get_opc(i)   ((i >> 26))

#define isMemWrite(i) (get_opc(i) == SW)
#define isMemRead(i)  (get_opc(i) == LW)
#define isBranch(i)   (get_opc(i) == BEQ)

int get_immed (unsigned int ins)
{
    int	  immed = ((ins & 0xFFFF));
    if (immed > 32767)
	return (immed - 65536);
    else
	return immed;
}

int hasInput (unsigned int ins)
{
    int	  opcode = get_opc (ins);
    return (opcode == LW || opcode == SW || opcode == AL);
}

int hasRtInput (unsigned int ins)
{
    int	  opcode = get_opc (ins);
    return (opcode == AL || opcode == BEQ);
}

int isWriteReg (unsigned int ins)
{
    int	  opcode = get_opc (ins);
    return (opcode == LW || opcode == AL);
}

int main ()
{
    int npc;

    mipsState_t current;
    mipsState_t next;

    initState (&current);		/* Initialize the pipeline current */
int counter = 0;
    while (counter < 9) {

	/* halt the pipeline if a halt instruction is at its WB stage */
	printState (&current);
	if (get_opc (current.MEMWB.instr) == HALT)
	    break;

	next = current;
	next.cycles++;
simWBStage(&next);
simEXStage(&next);
simIDStage(&next);
simIFStage(&next,current.IFID.PCPlus4);





	/* ======  Your code Start Here ====== */

	/*
	 * Suggest to approach the project step-by-step
	 * 1. Add code to simulate a code sequence without hazards
	 * 2. Detect data hazards and resolve them via forwarding/stallking
	 * 3. Detect data hazards for BEQ and resolve them
	 * 4. Detect control hazards from BEQ and resolve them
	 */

	 /* First implement detectHazards and sim{IF,ID,EX,MEM,WB}Stage before calling here */

	 /* ======  Your code ends Here ====== */

	current = next;	/* switch the state*/
  counter++;
    }

    /*
     * Remember to print the number of stalls, branches, and
     * mispredictions!
     */
    printf ("---------------- Final Report ------------------\n");
    printf ("Total cycles (including halt): %d;", current.cycles+1);
    printf (" forwards: %d;", current.forwards);
    printf (" stalls: %d;", current.stalls);
    printf (" branches: %d;", current.branches);
    printf (" taken: %d.\n", current.taken);

    return 0;
}

void detectHazards (mipsState_t * s)
{
     /* ======  Add your code Here ====== */
}

void simIFStage (mipsState_t * s, int npc)
{
    int instr = s->instrMem[npc / 4];
    s->IFID.instr = instr;
    s->IFID.PCPlus4 = s->IFID.PCPlus4 + 4;
}


void simIDStage (mipsState_t * s)
{
  s->IDEX.PCPlus4 = s->IFID.PCPlus4;
  s->IDEX.instr = s->IFID.instr;

  if (isMemRead(s->IDEX.instr)){   // INSTRUCTION IS LW
    s->IDEX.immed = get_immed(s->IDEX.instr);
    s->IDEX.rsReg = get_rs(s->IDEX.instr);
    s->IDEX.rtReg = get_rt(s->IDEX.instr);
  } else if (s->IDEX.instr == 0){
    s->IDEX.immed = 0;
    s->IDEX.rsReg = 0;
    s->IDEX.rtReg = 0;
    s->IDEX.PCPlus4 = 0;
    s->IDEX.branchTarget = 0;
  } else if (get_opc (s->IDEX.instr) == HALT){
    s->IDEX.immed = 0;
    s->IDEX.rsReg = 0;
    s->IDEX.rtReg = 0;
    s->IDEX.branchTarget = s->IDEX.PCPlus4;
  }
}

void simEXStage (mipsState_t * s)
{
  s->EXMEM.instr = s->IDEX.instr;
  s->EXMEM.rtRegData = s->IDEX.rtReg;

  if(isMemRead(s->EXMEM.instr)){
    s->EXMEM.ALUoutput = s->IDEX.immed;
  }
}

void simWBStage (mipsState_t * s)
{
  s->MEMWB.instr = s->EXMEM.instr;
     /* ======  Add your code Here ====== */
}

/******************************************************************/
/*
 * The initState function accepts a pointer to the current state as an
 * argument, initializing the state to pre-execution state. In particular,
 * all registers are zero'd out. All instructions in the pipeline are NOOPS.
 * Data and instruction memory are initialized with the contents of the
 * assembly input file.
 */
/*****************************************************************/
void initState (mipsState_t * sp)
{
    unsigned int dec_inst;
    int	  data_index = 0;
    int	  inst_index = 0;
    char  line[130];
    char  instr[6];
    char  args[130];
    char *arg;

    bzero(sp, sizeof(mipsState_t));

    /* Zero out data, instructions, and registers */
    memset (sp->dataMem, 0, 4 * NUMMEMS);
    memset (sp->instrMem, 0, 4 * NUMMEMS);
    memset (sp->regFile, 0, 4 * NUMREGS);

    /* Parse assembly file and initialize data/instruction memory */
    while (fgets (line, 130, stdin)) {
	if (sscanf (line, "\t.%s %s", instr, args) == 2) {
	    arg = strtok (args, ",");
	    while (arg != NULL) {
		sp->dataMem[data_index] = atoi (arg);
		data_index += 1;
		arg = strtok (NULL, ",");
	    }
	} else if (sscanf (line, "\t%s %s", instr, args) == 2) {
	    dec_inst = instrToInt (instr, args);
	    sp->instrMem[inst_index] = dec_inst;
	    inst_index += 1;
	}
    }

    /* Zero-out all registers in pipeline to start */
    sp->IFID.instr = 0;
    sp->IFID.PCPlus4 = 0;
    sp->IDEX.instr = 0;
    sp->IDEX.PCPlus4 = 0;
    sp->IDEX.branchTarget = 0;
    sp->IDEX.readData1 = 0;
    sp->IDEX.readData2 = 0;
    sp->IDEX.immed = 0;
    sp->IDEX.rsReg = 0;
    sp->IDEX.rtReg = 0;
    sp->IDEX.rdReg = 0;
    sp->EXMEM.instr = 0;
    sp->EXMEM.ALUoutput = 0;
    sp->EXMEM.rtRegData = 0;
    sp->EXMEM.writeRegNum = 0;
    sp->MEMWB.instr = 0;
    sp->MEMWB.memOutput = 0;
    sp->MEMWB.ALUoutput = 0;
    sp->MEMWB.writeRegNum = 0;
}

/***********************************************************
 * You do not need to modify the functions below. They are provided for your
 * convenience. The printState function accepts a pointer to a state as an
 * argument and prints the formatted contents of pipeline register.
 *************************************************************/
void printState (mipsState_t * sp)
{
    int	  i;
    printf ("============== State before Cycle %d ==============\n", sp->cycles + 1);
    printf ("PC = %d\n", sp->PC);
    printf ("IF/ID : ");
    printInstruction (sp->IFID.instr);
    printf ("\tNPC: %d\n", sp->IFID.PCPlus4);

    printf ("ID/EX : ");
    printInstruction (sp->IDEX.instr);
    printf ("\tNPC: %d ", sp->IDEX.PCPlus4);
    printf ("\tData1: %d ", sp->IDEX.readData1);
    printf ("Data2: %d  ", sp->IDEX.readData2);
    printf ("rs: %d ", sp->IDEX.rsReg);
    printf ("rt: %d ", sp->IDEX.rtReg);
    printf ("rd: %d  ", sp->IDEX.rdReg);
    printf ("imm: %d ", sp->IDEX.immed);
    printf ("Target: %d\n", sp->IDEX.branchTarget);

    printf ("EX/MEM: ");
    printInstruction (sp->EXMEM.instr);
    printf ("\tALUoutput: %d", sp->EXMEM.ALUoutput);
    printf ("\trtRegData: %d", sp->EXMEM.rtRegData);
    printf ("\twriteRegNum: %d\n", sp->EXMEM.writeRegNum);

    printf ("MEM/WB: ");
    printInstruction (sp->MEMWB.instr);
    printf ("\tALUoutput: %d", sp->MEMWB.ALUoutput);
    printf ("\tmemOutput: %d", sp->MEMWB.memOutput);
    printf ("\twriteRegNum: %d\n", sp->MEMWB.writeRegNum);

    printf ("Mem[0-7]: ");
    for (i = 0; i < NUMMEMS; i++) {
	if (i == (NUMMEMS / 2))
	    printf ("\nMem[8-F]: ");
	printf ("0x%-8X ", sp->dataMem[i]);
    }
    printf ("\n");

    printf ("Reg[0-7]: ");
    for (i = 0; i < NUMREGS; i++) {
	printf ("0x%-8X ", sp->regFile[i]);
    }
    printf ("\n\n");
}

/*************************************************************/
/* The instrToInt function converts an instruction from the  */
/* assembly file into an unsigned integer representation.    */
/* For example, consider the add $0,$1,$2 instruction.       */
/* In binary, this instruction is:                           */
/* 000000 00001 00010 00000 00000 100000                     */
/* The unsigned representation in decimal is therefore:      */
/* 2228256                                                   */
/*************************************************************/
unsigned int
instrToInt (char *inst, char *args)
{
    int	  opcode, rs, rt, rd, shamt, funct, immed, newval;
    unsigned int dec_inst, tmp_inst;
    if ((strcmp (inst, "add") == 0) || (strcmp (inst, "sub") == 0)) {
	opcode = 0;
	if (strcmp (inst, "add") == 0)
	    funct = ADD;
	else
	    funct = SUB;
	shamt = 0;
	rd = atoi (strtok (args, ",$"));
	rs = atoi (strtok (NULL, ",$"));
	rt = atoi (strtok (NULL, ",$"));
	dec_inst = (opcode << 26) + (rs << 21) + (rt << 16) + (rd << 11) + (shamt << 6) + funct;
    } else if ((strcmp (inst, "lw") == 0) || (strcmp (inst, "sw") == 0)) {
	if (strcmp (inst, "lw") == 0)
	    opcode = LW;
	else
	    opcode = SW;
	rt = atoi (strtok (args, ",$"));
	newval = immed = atoi (strtok (NULL, ",("));
	rs = atoi (strtok (NULL, "($)"));
	if (immed < 0)
	    newval = 65536 + immed;
	dec_inst = (opcode << 26) + (rs << 21) + (rt << 16) + newval;
    } else if (strcmp (inst, "beq") == 0) {
	opcode = 4;
	rs = atoi (strtok (args, ",$"));
	rt = atoi (strtok (NULL, ",$"));
	newval = immed = atoi (strtok (NULL, ","));
	if (immed < 0)
	    newval = 65536 + immed;
	dec_inst = (opcode << 26) + (rs << 21) + (rt << 16) + newval;
    } else if (strcmp (inst, "halt") == 0) {
	opcode = 63;
	dec_inst = (opcode << 26);
    } else if (strcmp (inst, "noop") == 0) {
	dec_inst = 0;
    }

    return dec_inst;
}

/*************************************************/
/* The printInstruction decodes an unsigned      */
/* integer representation of an instruction      */
/* into its string representation and prints     */
/* the result to stdout.                         */
/*************************************************/
void printInstruction (unsigned int instr)
{
    char  opcmd[10];
    if (instr == 0) {
	printf ("NOOP");
    } else if (get_opc (instr) == AL) {
	if (get_funct (instr) != 0) {
	    if (get_funct (instr) == ADD)
		strcpy (opcmd, "add");
	    else
		strcpy (opcmd, "sub");
	    printf ("%s $%d,$%d,$%d", opcmd, get_rd (instr), get_rs (instr), get_rt (instr));
	} else {
	    printf ("NOOP");
	}
    } else if (get_opc (instr) == LW) {
	printf ("%s $%d,%d($%d)", "lw", get_rt (instr), get_immed (instr), get_rs (instr));
    } else if (get_opc (instr) == SW) {
	printf ("%s $%d,%d($%d)", "sw", get_rt (instr), get_immed (instr), get_rs (instr));
    } else if (get_opc (instr) == BEQ) {
	printf ("%s $%d,$%d,%d", "beq", get_rs (instr), get_rt (instr), get_immed (instr));
    } else if (get_opc (instr) == HALT) {
	printf ("%s", "halt");
    }
}
