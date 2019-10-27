/**************************LC-3.c***************************

This c file defines the LC-3 methods for use by parts 2
through 8
____________________________________________________________

Name: Michael Johnson
Date: 07/05/2019

***********************************************************/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "lc3.h"

void loadProgram(FILE *file, int opcode) {
    callingOpcode = opcode;
    bool progHalt = false;
    initialiseState();
    int count = readFile(file);
    state.PC = addrReg[0].addr;
    origAddr = addrReg[0].addr;
    if (callingOpcode == 2)
        displayState(INITIAL);
    state.IR = addrRegPtr[0].addr;
    addrRegPtr = &addrReg[0];
    instrRegPtr = &instrReg[0];
    for (int i = 1; i <= count; i++) {
        addrRegPtr++;
        instrRegPtr++;
        parseInstr(addrRegPtr);
        state.IR = addrReg[i].addr;
        state.PC++;
        if (addrRegPtr->addr == HALT) {
            progHalt = true;
        }
        if (!progHalt) {
            executeInstr();
        }
    }
}

int readFile(FILE *file) {
    int i = 0;
    if (file == 0) {
        return 0;
    }
    int hex1;
    int hex2;
    while ((hex1 = fgetc(file)) != EOF) {
        if ((hex2 = fgetc(file)) == EOF) {
            break;
        }
        hexAddr curr = {hex1 << 8, hex2, ((hex1 << 8) + hex2)};
        addrRegPtr[i] = curr;
        i++;
    }
    return i;
}

void initialiseState() {
    statePtr->R0 = 0,
    statePtr->R1 = 0,
    statePtr->R2 = 0,
    statePtr->R3 = 0,
    statePtr->R4 = 0,
    statePtr->R5 = 0,
    statePtr->R6 = 0,
    statePtr->R7 = 0,
    statePtr->PC = 0,
    statePtr->IR = 0,
    statePtr->CC = "Z";
}

void parseInstr() {
    int offset;
    if ((addrRegPtr->addr & 0x01FF) > 255) {
        offset = (~(addrRegPtr->addr & 0x01FF) + 1) >> 9;
    } else {
        offset = (addrRegPtr->addr & 0x01FF);
    }
    instrRegPtr->opcode = (addrRegPtr->addr & 0xF000) >> 12;
    switch (instrRegPtr->opcode) {
        case LD:
            instrRegPtr->reg_11_9 = (addrRegPtr->addr & 0x0E00) >> 9;
            instrRegPtr->pcOffset9 = offset;
            break;
        case LEA:
            instrRegPtr->reg_11_9 = (addrRegPtr->addr & 0x0E00) >> 9;
            instrRegPtr->pcOffset9 = offset;
            break;
        case LDI:
            instrRegPtr->reg_11_9 = (addrRegPtr->addr & 0x0E00) >> 9;
            instrRegPtr->pcOffset9 = offset;
            break;
        case AND:
            if (((addrRegPtr->addr & 0x0020) >> 5) == 1) {
                instrRegPtr->reg_11_9 = (addrRegPtr->addr & 0x0E00) >> 9;
                instrRegPtr->reg_8_6 = (addrRegPtr->addr & 0x01C0) >> 6;
                instrRegPtr->imm5 = (addrRegPtr->addr & 0x001F);
            } else {
                instrRegPtr->reg_11_9 = (addrRegPtr->addr & 0x0E00) >> 9;
                instrRegPtr->reg_8_6 = (addrRegPtr->addr & 0x01C0) >> 6;
                instrRegPtr->reg_2_0 = (addrRegPtr->addr & 0x0007);
            }
            break;
        case NOT:
            instrRegPtr->reg_11_9 = (addrRegPtr->addr & 0x0E00) >> 9;
            instrRegPtr->reg_8_6 = (addrRegPtr->addr & 0x01C0) >> 6;
            break;
        case ADD:
            if (((addrRegPtr->addr & 0x0020) >> 5) == 1) {
                instrRegPtr->reg_11_9 = (addrRegPtr->addr & 0x0E00) >> 9;
                instrRegPtr->reg_8_6 = (addrRegPtr->addr & 0x01C0) >> 6;
                instrRegPtr->imm5 = (addrRegPtr->addr & 0x001F);
            } else {
                instrRegPtr->reg_11_9 = (addrRegPtr->addr & 0x0E00) >> 9;
                instrRegPtr->reg_8_6 = (addrRegPtr->addr & 0x01C0) >> 6;
                instrRegPtr->reg_2_0 = (addrRegPtr->addr & 0x0007);
            }
            break;
        case BR:
            instrRegPtr->n = (addrRegPtr->addr & 0x0800) >> 11;
            instrRegPtr->z = (addrRegPtr->addr & 0x0400) >> 10;
            instrRegPtr->p = (addrRegPtr->addr & 0x0200) >> 9;
            instrRegPtr->pcOffset9 = offset;
            break;
    }
}

void displayState(int init) {
    if (init == INITIAL) {
        printf("Initial state\n");
    } else if (init == EXECUTED) {
        printf("after executing instruction\t0x%04x\n", state.IR);
    }
    printf("R0\t0x%04x\n", state.R0);
    printf("R1\t0x%04x\n", state.R1);
    printf("R2\t0x%04x\n", state.R2);
    printf("R3\t0x%04x\n", state.R3);
    printf("R4\t0x%04x\n", state.R4);
    printf("R5\t0x%04x\n", state.R5);
    printf("R6\t0x%04x\n", state.R6);
    printf("R7\t0x%04x\n", state.R7);
    printf("PC\t0x%04x\n", state.PC);
    printf("IR\t0x%04x\n", state.IR);
    printf("CC\t%s\n", state.CC);
    printf("==================\n");
}

void executeInstr() {
    switch (instrRegPtr->opcode) {
        case LD:
            ld(instrRegPtr);
            if (callingOpcode == LD)
                displayState(EXECUTED);
            break;
        case LEA:
            lea(instrRegPtr);
            if (callingOpcode == LEA)
                displayState(EXECUTED);
            break;
        case LDI:
            ldi(instrRegPtr);
            if (callingOpcode == LDI)
                displayState(EXECUTED);
            break;
        case AND:
            and(instrRegPtr);
            if (callingOpcode == AND)
                displayState(EXECUTED);
            break;
        case NOT:
            not(instrRegPtr);
            if (callingOpcode == NOT)
                displayState(EXECUTED);
            break;
        case ADD:
            add(instrRegPtr);
            if (callingOpcode == ADD)
                displayState(EXECUTED);
            break;
        case BR:
            br(instrRegPtr);
            if (callingOpcode == BR)
                displayState(EXECUTED);
            break;
        default:
            return;
    }
}

void setCC(int addr) {
    int MSB = (addr & 0x8000) >> 15;
    if (addr == 0) {
        state.CC = "Z";
    } else if (MSB == 1) {
        state.CC = "N";
    } else if (MSB == 0) {
        state.CC = "P";
    }
}

void setRegister(int reg, int value) {
    switch (reg) {
        case 0:
            state.R0 = value;
            break;
        case 1:
            state.R1 = value;
            break;
        case 2:
            state.R2 = value;
            break;
        case 3:
            state.R3 = value;
            break;
        case 4:
            state.R4 = value;
            break;
        case 5:
            state.R5 = value;
            break;
        case 6:
            state.R6 = value;
            break;        
        case 7:
            state.R7 = value;
            break;
    }
}

int getRegVal(int reg) {
    switch (reg) {
        case 0:
            return state.R0;
            break;
        case 1:
            return state.R1;
            break;
        case 2:
            return state.R2;
            break;
        case 3:
            return state.R3;
            break;
        case 4:
            return state.R4;
            break;
        case 5:
            return state.R5;
            break;
        case 6:
            return state.R6;
            break;        
        case 7:
            return state.R7;
            break;
    }
}

void ld() {
    int index = instrRegPtr->pcOffset9 + (state.PC - origAddr);
    index++;
    int value = addrReg[index].addr;
    setRegister(instrRegPtr->reg_11_9, value);
    setCC(value);
}

void lea() {
    int addr = instrRegPtr->pcOffset9 + state.PC;
    setRegister(instrRegPtr->reg_11_9, addr);
    setCC(addr);
}

void ldi() {
    int index = instrRegPtr->pcOffset9 + (state.PC - origAddr);
    index++;
    int memLoc = addrReg[index].addr - origAddr + 1;
    int value = addrReg[memLoc].addr;
    setRegister(instrRegPtr->reg_11_9, value);
    setCC(value);
}

void and() {
    int result;
    int bit5 = (addrRegPtr->addr & 0x0020) >> 5;
    if (bit5 == 1) {
        result = instrRegPtr->imm5 & getRegVal(instrRegPtr->reg_8_6);
        setRegister(instrRegPtr->reg_11_9, result);
    } else {
        result = getRegVal(instrRegPtr->reg_2_0) & getRegVal(instrRegPtr->reg_8_6);
        setRegister(instrRegPtr->reg_11_9, result);
    }
    setCC(result);
}

void not() {
    int result = ~getRegVal(instrRegPtr->reg_8_6) & 0xFFFF;
    setRegister(instrRegPtr->reg_11_9, result);
    setCC(result);
}

void add() {
    int result;
    int bit5 = (addrRegPtr->addr & 0x0020) >> 5;
    if (bit5 == 1) {
        result = instrRegPtr->imm5 + getRegVal(instrRegPtr->reg_8_6) & 0xFFFF;
        setRegister(instrRegPtr->reg_11_9, result);
    } else {
        result = getRegVal(instrRegPtr->reg_2_0) + getRegVal(instrRegPtr->reg_8_6) & 0xFFFF;
        setRegister(instrRegPtr->reg_11_9, result);
    }
    setCC(result);
}

void br() {
    if ((instrRegPtr->n == 1 & state.CC == "N") | (instrRegPtr->z == 1 & state.CC == "Z") | (instrRegPtr->p == 1 & state.CC == "P")) {
        state.PC = state.PC + instrRegPtr->pcOffset9;
    }
}
