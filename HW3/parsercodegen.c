/*
Assignment:
HW3 - Parser and Code Generator for PL/0

Author(s): Archi Mehta, Ramyanaga Nagarur

Language: C (only)

To Compile:
    Scanner:
        gcc -O2 -std=c11 -o lex lex.c
    Parser/Code Generator:
        gcc -O2 -std=c11 -o parsercodegen parsercodegen.c

To Execute (on Eustis):
    ./lex <input_file.txt>
    ./parsercodegen

where:
    <input_file.txt> is the path to the PL/0 source program
Notes:
    - lex.c accepts ONE command-line argument (input PL/0 source file)
    - parsercodegen.c accepts NO command-line arguments
    - Input filename is hard-coded in parsercodegen.c
    - Implements recursive-descent parser for PL/0 grammar
    - Generates PM/0 assembly code (see Appendix A for ISA)
    - All development and testing performed on Eustis

Class: COP3402 - System Software - Fall 2025

Instructor: Dr. Jie Lin

Due Date: Friday, October 31, 2025 at 11:59 PM ET
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_SYMBOL_TABLE_SIZE 500
#define MAX_CODE_LENGTH 50

typedef struct{
    int kind;       // const = 1, var = 2, proc = 3
    char name[12];  // nape up to 11 chars
    int val;        // number (ASCII value)
    int level;      // L level
    int addr;       // M address
    int mark;       // to indicate unavailable or deleted
} symbol;

typedef struct{
    int OP;
    int L;
    int M;
} instruction;

typedef struct {
    int token;
    char lexeme[12];
}TokenListEntry;

// function prototypes
void program(FILE *ofp);
void block(FILE *ofp);
void const_declaration(FILE *ofp);
int var_declaration(FILE *ofp);
void statement(FILE *ofp);
void condition(FILE *ofp);
void expression(FILE *ofp);
void term(FILE *ofp);
void factor(FILE *ofp);

// global variables
symbol symbol_table[MAX_SYMBOL_TABLE_SIZE];
instruction instr[MAX_CODE_LENGTH];
TokenListEntry tokenList[100];

int instrCodeIndex = 0;
int symbolCount = 0;
int currentIndex = 0;
int currentToken;

void nextToken(void){
    currentToken = tokenList[currentIndex].token;
    currentIndex++;
}

// add entry to symbol table
void addSymbolTable(int kind, char * name, int val, int level, int addr){
    symbol_table[symbolCount].kind = kind;
    strcpy(symbol_table[symbolCount].name, name);
    symbol_table[symbolCount].val = val;
    symbol_table[symbolCount].level = level;
    symbol_table[symbolCount].addr = addr;
    symbol_table[symbolCount].mark = 0;
    symbolCount++;
}

// search symbol table entries by name
int searchSymbolTable(char * name){
    for(int i=0; i<symbolCount; i++){
        // find unmarked symbol with the desired name
        // should it check symbol_table[i].mark == 0?
        if((strcmp(symbol_table[i].name, name)==0) && symbol_table[i].mark == 0){
            return i;
        }
    }
    // if symbol is not found, return -1
    return -1;
}

// add an instruction with OP, L, & M to the instructions array
void emit(int OP, int L, int M){
    instr[instrCodeIndex].OP = OP;
    instr[instrCodeIndex].L = L;
    instr[instrCodeIndex].M = M;
    instrCodeIndex++;
}

void const_declaration(FILE * ofp){
    // if token == const
    if(currentToken == 28){
        // get next token
        nextToken();

        do{
            // if token != identsym
            if (currentToken != 2) {
                printf("Error: const, var, and read keywords must be followed by identifier\n");
                fprintf(ofp, "Error: const, var, and read keywords must be followed by identifier\n");
                fclose(ofp);
                exit(0);
            }

            // if SYMBOLTABLECHECK(token) != -1
            if (searchSymbolTable(tokenList[currentIndex-1].lexeme) != -1) {
                printf("Error: symbol name has already been declared\n");
                fprintf(ofp, "Error: symbol name has already been declared\n");
                fclose(ofp);
                exit(0);
            }

            // save ident name
            char name[12];
            strcpy(name, tokenList[currentIndex - 1].lexeme);
            nextToken();

            // if token != eqlsym
            if (currentToken != 8) {
                printf("Error: constants must be assigned with =\n");
                fprintf(ofp, "Error: constants must be assigned with =\n");
                fclose(ofp);
                exit(0);
            }
            nextToken();

            // if token != numbersym
            if (currentToken != 3) {
                printf("Error: constants must be assigned an integer value\n");
                fprintf(ofp, "Error: constants must be assigned an integer value\n");
                fclose(ofp);
                exit(0);
            }

            // add to symbol table (kind 1, saved name, number, 0, 0)
            int value = atoi(tokenList[currentIndex - 1].lexeme);
            addSymbolTable(1, name, value, 0, 0);
            nextToken();

            // loop again if comma follows
        }while(currentToken == 16);

        // if token != semicolonsym
        if(currentToken != 17){
            printf("Error: constant and variable declarations must be followed by a semicolon\n");
            fprintf(ofp, "Error: constant and variable declarations must be followed by a semicolon\n");
            fclose(ofp);
            exit(0);
        }
        nextToken();
    }
}

int var_declaration(FILE * ofp){
    int numVars = 0;
    // if token == varsym
    if(currentToken == 29){
        do{
            numVars++;
            nextToken();

            // if token != identsym
            if(currentToken != 2){
                printf("Error: const, var, and read keywords must be followed by identifier\n");
                fprintf(ofp, "Error: const, var, and read keywords must be followed by identifier\n");
                fclose(ofp);
                exit(0);
            }

            // if SYMBOLTABLECHECK(token) != -1
            if (searchSymbolTable(tokenList[currentIndex-1].lexeme) != -1) {
                printf("Error: symbol name has already been declared\n");
                fprintf(ofp, "Error: symbol name has already been declared\n");
                fclose(ofp);
                exit(0);
            }

            // add to symbol table (kind 2, ident, 0, 0, var#+2)
            addSymbolTable(2, tokenList[currentIndex-1].lexeme, 0, 0, numVars+2); 
            nextToken();
        }while(currentToken == 16);

        // if token != semicolonsym
        if(currentToken != 17){
            printf("Error: constant and variable declarations must be followed by a semicolon\n");
            fprintf(ofp, "Error: constant and variable declarations must be followed by a semicolon\n");
            fclose(ofp);
            exit(0);
        }
        nextToken();
    }
    return numVars;
}

void statement(FILE * ofp){
    // if token == identsym
    if(currentToken == 2){
        int symIdx = searchSymbolTable(tokenList[currentIndex-1].lexeme);
        if(symIdx == -1){
            printf("Error: undeclared identifier\n");
            fprintf(ofp, "Error: undeclared identifier\n");
            fclose(ofp);
            exit(0);
        }
        // if table[symIdx].kind != 2 (not a var): error
        if(symbol_table[symIdx].kind != 2){
            printf("Error: only variable values may be altered\n");
            fprintf(ofp, "Error: only variable values may be altered\n");
            fclose(ofp);
            exit(0);
        }
        nextToken();
        
        // if token != becomesym
        if(currentToken != 19){
            printf("Error: assignment statements must use :=\n");
            fprintf(ofp, "assignment statements must use :=\n");
            fclose(ofp);
            exit(0);
        }
        nextToken();
        expression(ofp);
        
        // emit STO (M = table[symIdx].addr)
        emit(4,0, symbol_table[symIdx].addr);
    }

    // if token == beginsym
    else if(currentToken == 20){
        nextToken();             
        statement(ofp);          

        // while token == semicolonsym
        while(currentToken == 17){
            nextToken();
            statement(ofp);
        }

        // if token != endsym
        if(currentToken != 21){
            printf("Error: begin must be followed by end\n");
            fprintf(ofp, "Error: begin must be followed by end\n");
            fclose(ofp);
            exit(0);
        }
        nextToken();
        return;
    }

    // if token == ifsym
    else if(currentToken == 22){
        nextToken();
        condition(ofp);
        
        // jpcIdx = current code index
        int jpcIdx = instrCodeIndex;
        emit(8, 0, 0);

        // if token != thensym
        if(currentToken != 24){
            printf("Error: if must be followed by them\n");
            fprintf(ofp, "Error: if must be followed by them\n");
            fclose(ofp);
            exit(0);
        }
        nextToken();
        statement(ofp);
        if (currentToken == 33) { // elsesym (token 33)
        int jmpIdx = instrCodeIndex;
        emit(7, 0, 0); // JMP to skip else
        instr[jpcIdx].M = 3 * instrCodeIndex; // JPC to start of else
        nextToken();
        statement(ofp);
        instr[jmpIdx].M = 3 * instrCodeIndex; // JMP to after else
    }
    else {
        instr[jpcIdx].M = 3 * instrCodeIndex; // no else: jump to after if
    }

    // ✅ consume 'fi'
    if (currentToken != 23) { // fisym
        printf("Error: if must be followed by fi\n");
        fprintf(ofp, "Error: if must be followed by fi\n");
        fclose(ofp);
        exit(0);
    }
    nextToken(); // consume fi
    }

    // if token == whilesym
    else if(currentToken == 25){
        nextToken();
        int loopIdx = instrCodeIndex;
        condition(ofp);
        
        // if token != dosym
        if(currentToken != 26){
            printf("Error: while must be followed by do\n");
            fprintf(ofp, "Error: while must be followed by do\n");
            fclose(ofp);
            exit(0);
        }
        nextToken();
        int jpcIdx = instrCodeIndex;
        emit(8, 0, 0);
        statement(ofp);
        // emit JMP (M = loopIdx)
        emit(7, 0, loopIdx);
        instr[jpcIdx].M = instrCodeIndex;
        return;
    }

    // if token == readsym
    else if(currentToken == 32){
        nextToken();

        // if token != identsym
        if(currentToken != 2){
            printf("Error: read must be followed by an identifier\n");
            fprintf(ofp, "Error: read must be followed by an identifier\n");
            fclose(ofp);
            exit(0);
        }

        // symIdx = SYMBOLTABLECHECK(token)
        int symIdx = searchSymbolTable(tokenList[currentIndex-1].lexeme);

        if(symIdx == -1){
            printf("Error: undeclared identifier\n");
            fprintf(ofp, "Error: undeclared identifier\n");
            fclose(ofp);
            exit(0);
        }
        nextToken();
        
        // emit READ
        emit(9, 0, 2);

        // emit STO (M = table[symIdx].addr)
        emit(4, 0 ,symbol_table[symIdx].addr);
        return;
    }

    // if token == writesym
    else if(currentToken == 31){
        nextToken();
        expression(ofp);

        // emit WRITE
        emit(9, 0, 1);
        return;
    }
}

void condition(FILE * ofp){
    // if token == oddsym
    if(currentToken == 34){
        nextToken();
        expression(ofp);
        
        // emit ODD
        emit(2, 0, 11); // ODD (15 is opcode for OPR, M=6 for odd);
    }
    else{
        expression(ofp);

        // if token == eqlsym
        if(currentToken == 8){
            nextToken();
            expression(ofp);

            // emit EQL
            emit(2, 0, 5);
        }
        // else if token == neqsym
        else if(currentToken == 9){
            nextToken();
            expression(ofp);

            // emit NEQ
            emit(2, 0, 6);
        }
        // else if token == lessym
        else if(currentToken == 10){
            nextToken();
            expression(ofp);
            
            // emit LSS
            emit(2, 0, 7);
        }
        // else if token == leqsym
        else if(currentToken == 11){
            nextToken();
            expression(ofp);
            
            // emit LEQ
            emit(2, 0, 8);
        }

        // else if token == gtrsym
        else if(currentToken == 12){
            nextToken();
            expression(ofp);

            // emit GTR
            emit(2, 0, 9);
        }
        // else if token == geqsym
        else if(currentToken == 13){
            nextToken();
            expression(ofp);

            // emit GEQ
            emit(2, 0, 10);
        }
        else{
            printf("Error: condition must contain comparison operator\n");
            fprintf(ofp, "Error: condition must contain comparison operator\n");
            fclose(ofp);
            exit(0);
        }
    }
}

void factor(FILE * ofp){
    // if token == identsym
    if(currentToken == 2){
        int symIdx = searchSymbolTable(tokenList[currentIndex-1].lexeme);
        if(symIdx == -1){
            printf("Error: undeclared identifier\n");
            fprintf(ofp, "Error: undeclared identifier\n");
            fclose(ofp);
            exit(0);
        }

        // if table[symIdx].kind == 1 (const)
        if(symbol_table[symIdx].kind == 1){
            // emit LIT (M = table[symIdx].value)
            emit(1, 0, symbol_table[symIdx].val);
        }
        // else (var)
        else{
            // symbol_table[symIdx].mark = 1;
            // emit LOD (M = table[symIdx].addr)
            emit(3, 0, symbol_table[symIdx].addr);
        }
        nextToken();
    }
    // else if token == numbersym
    else if(currentToken == 3){
        // emit LIT
        int value = atoi(tokenList[currentIndex-1].lexeme);
        emit(1, 0, value);
        nextToken();
    }
    // else if token == lparentsym
    else if(currentToken == 14){
        nextToken();
        expression(ofp);
        // if token != rparentsym
        if(currentToken != 15){
            printf("Error: right parenthesis is missing\n");
            fprintf(ofp, "Error: right parenthesis is missing\n");
            fclose(ofp);
            exit(0);
        }
        nextToken();
    }
    else{
        printf("Error: invalid factor\n");
        fprintf(ofp, "Error: invalid factor\n");
        fclose(ofp);
        exit(0);
    }
}

void term(FILE * ofp){
    factor(ofp);

    // while token == multsym || token == slashsym || token == modsym
    while(currentToken == 6 || currentToken == 7 || currentToken == 35){
        if(currentToken == 6){
            nextToken();
            factor(ofp);

            // emit MUL
            emit(2, 0, 3);
        }
        else if(currentToken == 7){
            nextToken();
            factor(ofp);

            // emit DIV
            emit(2, 0, 4);
        }
        else{
            nextToken();
            factor(ofp);

            // emit MOD 
            emit(2, 0, 16);
        }
    }

}

void expression(FILE * ofp){
    // if token == minussym
    if(currentToken == 5){
        nextToken();
        term(ofp);

        // emit NEG
        emit(2, 0, 12);

        // while token == plussym || token == minussym
        while(currentToken == 4 || currentToken == 5){
            if(currentToken == 4){
                nextToken();
                term(ofp);

                // emit ADD
                emit(2, 0, 1);
            }
            else{
                nextToken();
                term(ofp);

                // emit SUB
                emit(2, 0, 2);
            }
        }
    }
    else{
        // if token == plussym
        if(currentToken == 4){
            nextToken();
        }
        term(ofp);

        // while token == plussym || token == minussym
        while(currentToken == 4 || currentToken == 5){
            if(currentToken == 4){
                nextToken();
                term(ofp);

                // emit ADD
                emit(2, 0, 1);
            }
            else{
                nextToken();
                term(ofp);

                // emit SUB
                emit(2, 0, 2);
            }
        }
    }
}

// F3 BLOCK Function
void block(FILE * ofp){
    int jmpIdx = instrCodeIndex;
    emit(7, 0, 0);
    
    const_declaration(ofp);
    int numVars = var_declaration(ofp);

    instr[jmpIdx].M = 3 * instrCodeIndex;

    emit(6,0,3+numVars);
    
    statement(ofp);

    for(int i=0; i<symbolCount; i++){
        if(symbol_table[i].mark == 0){
            symbol_table[i].mark = 1;
        }
    }
}

// F2 PROGRAM Function
void program(FILE *ofp){
    // BLOCK
    block(ofp);
    // if token != periodsym
    if(currentToken != 18){
        // print error to console and output file
        printf("Error: program must end with period\n");
        fprintf(ofp, "Error: program must end with period\n");
        fclose(ofp);
        // exit the code
        exit(0);
    }
    // emit HALT
    emit(9,0,3);
}


int main(void){
    FILE *ifp = fopen("tokens.txt", "r");
    FILE *ofp = fopen("elf.txt", "w");

    if(ifp == NULL || ofp == NULL){
        printf("Error: could not open file\n");
        return 1;
    }

    int count = 0;
    while(!feof(ifp)){
        if(fscanf(ifp, "%d", &tokenList[count].token));
        if(tokenList[count].token == 1){
            printf("Error: Scanning error detected by lexer (skipsym present)\n");
            fprintf(ofp, "Error: Scanning error detected by lexer (skipsym present)\n");
            fclose(ifp);
            fclose(ofp);
            exit(0);
        }
        if(tokenList[count].token == 2 || tokenList[count].token == 3){
            if(fscanf(ifp, "%s", tokenList[count].lexeme));
        }
        count++;
    }
    fclose(ifp);

    // reset index for parsing
    currentIndex = 0;
    nextToken();
    program(ofp);

    printf("Assembly Code:\n\n");
    printf("Line\tOP\tL\tM\n");

    for(int i=0; i<instrCodeIndex; i++){
        fprintf(ofp, "%d %d %d\n", instr[i].OP, instr[i].L, instr[i].M);
        printf("%d\t", i);
        if(instr[i].OP == 1){
            printf("LIT\t");
        }
        else if(instr[i].OP == 2){
            printf("OPR\t");
        }
        else if(instr[i].OP == 3){
            printf("LOD\t");
        }
        else if(instr[i].OP == 4){
            printf("STO\t");
        }
        else if(instr[i].OP == 5){
            printf("CAL\t");
        }
        else if(instr[i].OP == 6){
            printf("INC\t");
        }
        else if(instr[i].OP == 7){
            printf("JMP\t");
        }
        else if(instr[i].OP == 8){
            printf("JPC\t");
        }
        else if(instr[i].OP == 9){
            printf("SYS\t");
        }
        printf("%d\t%d\n", instr[i].L, instr[i].M);
    }

    printf("\nSymbol Table:\n\n");
    printf("Kind | Name\t| Value | Level | Address | Mark\n");
    printf("------------------------------------------------\n");

    for(int i=0; i<symbolCount; i++){
        printf("   %d |\t      %s |     %d |     %d |       %d |    %d\n", symbol_table[i].kind, symbol_table[i].name, symbol_table[i].val, symbol_table[i].level, symbol_table[i].addr, symbol_table[i].mark);
    }

    fclose(ofp);
    return 0;
}