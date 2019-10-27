/**************************LC-3.h***************************

This header file declares the LC-3 data structures, methods
and global variables
____________________________________________________________

Name: Michael Johnson
UPI: mjoh781
ID: 6381828
Date: 07/05/2019

***********************************************************/

#define INITIAL 0
#define EXECUTED 1
#define LENGTH 4
#define MAX 100
#define HALT 61477
#define LD 2
#define LEA 14
#define LDI 10
#define AND 5
#define NOT 9
#define ADD 1
#define BR 0

/* HEX ADDRESS STRUCT */

// For processing the raw addresses into instructions
typedef struct hexAddress hexAddr;

struct hexAddress {
    int byte1;
    int byte2;
    int addr;
};

// Global hex address register
hexAddr addrReg[MAX];
hexAddr *addrRegPtr = addrReg;

/* INSTRUCTION STRUCT */

// Implements an instruction using the LC-3 instruction set
typedef struct instructions instr;

struct instructions {
    int opcode;
    int n;
    int z;
    int p;
    // DR
    int reg_11_9;
    // SR1
    int reg_8_6;
    // SR2
    int reg_2_0;
    signed int pcOffset9;
    int imm5;
    // Stores the current hex PC
    char PC;
};

// Global instruction register
instr instrReg[MAX];
instr *instrRegPtr = instrReg;

/* LC-3 STRUCT */

// Emulates the LC-3 machine registers
typedef struct lc3state lc3;

struct lc3state {
    int R0;
    int R1;
    int R2;
    int R3;
    int R4;
    int R5;
    int R6;
    int R7;
    // Program counter
    int PC;
    // Instruction register
    int IR;
    // Condition code
    char *CC;
};

// Global LC-3 machine state
lc3 state;
lc3 *statePtr = &state;
// Contains the .ORIG address
int origAddr;
int callingOpcode;

// Interprets an instruction and inserts it into an instruction slot
void parseInstr();

// Given a register, set the value of that register
void setRegister(int reg, int value);

void ld();

void lea();

void ldi();

void and();

void not();

void add();

void br();

// Sets the condition code while executing an instruction; takes the current addr
void setCC(int addr);

// Gets the value of a register given an integer 0 through 7
int getRegVal(int reg);

// Initialises the LC-3 machine register and condition code states
void initialiseState();

// Takes the input file for instructions from the user, and then loads/executes said instructions
void loadProgram(FILE *file, int opcode);

// Read and store the current intstruction for processing via loadProgram(); returns the instruction count
int readFile(FILE *file);

// Displays the state of the LC-3 registers and condition code; init = INITIAL specifies an initial display state, and init = EXECUTED specifies an instruction post-execution display state 
void displayState(int init);

// Executes an instruction on the LC-3 machine; calls displayState(EXECUTED) to display the post-execution state where applicable
void executeInstr();