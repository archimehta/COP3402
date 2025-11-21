/*
Assignment:
HW4 - Complete Parser and Code Generator for PL/0
    (with Procedures, Call, and Else)

Author(s): Archi Mehta, Ramyanaga Nagarur

Language: C (only)

To Compile:
    Scanner:
        gcc -O2 -std=c11 -o lex lex.c
    Parser/Code Generator:
        gcc -O2 -std=c11 -o parsercodegen_complete parsercodegen_complete.c
    Virtual Machine:
        gcc -O2 -std=c11 -o vm vm.c

To Execute (on Eustis):
    ./lex <input_file.txt>
    ./parsercodegen_complete
    ./vm elf.txt

where:
    <input_file.txt> is the path to the PL/0 source program

Notes:
    - lex.c accepts ONE command-line argument (input PL/0 source file)
    - parsercodegen_complete.c accepts NO command-line arguments
    - Input filename is hard-coded in parsercodegen_complete.c
    - Implements recursive-descent parser for extended PL/0 grammar
    - Supports procedures, call statements, and if-then-else
    - Generates PM/0 assembly code (see Appendix A for ISA)
    - VM must support EVEN instruction (OPR 0 11)
    - All development and testing performed on Eustis

Class: COP3402 - System Software - Fall 2025

Instructor: Dr. Jie Lin

Due Date: Friday, November 21, 2025 at 11:59 PM ET
*/

# include <stdio.h>
# define MAX_SIZE 500

// Initialize stack with zeros
int PAS[MAX_SIZE] = {0};

// Returns the base address L levels down by following static links
int base(int BP, int L){
    int arb = BP;
    while(L>0){
        arb = PAS[arb];
        L--;
    }
    return arb;
}

void print(int L, int M, int PC, int BP, int SP, int OP){
    // Print the mneumonic for the OP code
    if(OP==1){
        printf("LIT");
    }
    if(OP==2){
        if(M==0){
            printf("RTN");
        }
        if(M==1){
            printf("ADD");
        }
        if(M==2){
            printf("SUB");
        }
        if(M==3){
            printf("MUL");
        }
        if(M==4){
            printf("DIV");
        }
        if(M==5){
            printf("EQL");
        }
        if(M==6){
            printf("NEQ");
        }
        if(M==7){
            printf("LSS");
        }
        if(M==8){
            printf("LEQ");
        }
        if(M==9){
            printf("GTR");
        }
        if(M==10){
            printf("GEQ");
        }
        if(M==11){
            printf("EVEN");
        }
    }
    if(OP==3){
        printf("LOD");
    }
    if(OP==4){
        printf("STO");
    }
    if(OP==5){
        printf("CAL");
    }
    if(OP==6){
        printf("INC");
    }
    if(OP==7){
        printf("JMP");
    }
    if(OP==8){
        printf("JPC");
    }
    if(OP==9){
        printf("SYS");
    }

    // Print the instruction details and register values
    printf("\t%d\t%d\t%d\t%d\t%d\t", L, M, PC, BP, SP);

    // If the BP is not valid, we don't print
    if(BP <= 0){
        printf("\n");
        return;
    }

    // Collect the BP's to find the AR boundaries
    // Creating additional array to assist with printing as permitted by updated assignment guidelines
    int boundaries[50];
    int count = 0;
    int temp_bp = BP;
    while(temp_bp > 0){
        boundaries[count++] = temp_bp;
        temp_bp = PAS[temp_bp - 1];
    }

    // Print each AR from most recent to oldest
    for(int i = count - 1; i >= 0; i--){
        int frame_bp = boundaries[i];
        int frame_sp;

        if(i == 0){
            frame_sp = SP;
        } 
        else{
            frame_sp = boundaries[i-1] + 1;
        }

        // Print the values in the current frame's boundary
        if(frame_bp >= frame_sp){
            for(int j = frame_bp; j >= frame_sp; j--){
                printf("%d ", PAS[j]);
            }
        }

        // Prints the separator only if there is a next frame and it has locals
        if(i > 0){
            int next_frame_bp = boundaries[i-1];
            int next_frame_sp = (i-1 == 0) ? SP : boundaries[i-2] + 1;
            
            if(next_frame_bp >= next_frame_sp){
                printf("| ");
            }
        }
    }
    printf("\n");
}

int main(int argc, char *argv[]){
    // Check number of arguments
    if(argc != 2){
        printf("Error: Expected exactly one argument (input file).\n");
        return 1;
    }

    // Open the file
    FILE *ifp = fopen(argv[1], "r");
    if(!ifp){
        printf("Error: Could not open file %s\n", argv[1]);
        return 1;
    }

    // Get the data
    int tracker = MAX_SIZE - 1;  // 499
    int op, l, m;

    while (fscanf(ifp, "%d %d %d", &op, &l, &m) == 3){
        PAS[tracker--] = op;
        PAS[tracker--] = l;
        PAS[tracker--] = m;
    }

    fclose(ifp);

    // Set up registers
    int PC = MAX_SIZE - 1;
    int SP = tracker+1;
    int BP = SP-1;

    // Print the current values
    printf("\tL\tM\tPC\tBP\tSP\tstack\n");
    printf("Initial values:\t\t%d\t%d\t%d\n", PC, BP, SP);

    // Flag to keep track if we halt (SYS 9 0 3)
    int halt = 1;

    // Loop to go through all instructions
    while(halt){
        // The Fetch Cycle: Copy instruction at PC and decrement by 3
        int OP = PAS[PC];
        int L = PAS[PC-1];
        int M = PAS[PC-2];
        PC -= 3;

        // Execute Cycle: Took all the OP's from the HW1 PDF and translated from pseudocode
        // LIT
        if(OP==1){
            SP -= 1;
            PAS[SP] = M;
            print(L, M, PC, BP, SP, OP);   
        }
        //OPR
        else if(OP==2){
            //RTN
            if(M==0){
                SP = BP + 1;
                BP = PAS[SP-2];
                PC = PAS[SP-3];
            }
            // ADD
            else if(M==1){
                PAS[SP+1] = PAS[SP+1] + PAS[SP];
                SP += 1;
            }
            //SUB
            else if(M==2){
                PAS[SP+1] = PAS[SP+1] - PAS[SP];
                SP += 1;
            }
            //MUL
            else if(M==3){
                PAS[SP+1] = PAS[SP+1] * PAS[SP];
                SP += 1;
            }
            // DIV
            else if(M==4){
                PAS[SP+1] = PAS[SP+1] / PAS[SP];
                SP += 1;
            }
            // EQL
            else if(M==5){
                PAS[SP+1] = ((PAS[SP+1] == PAS[SP]) ? 1 : 0);
                SP += 1;
            }
            //NEQ
            else if(M==6){
                PAS[SP+1] = ((PAS[SP+1] != PAS[SP]) ? 1 : 0);
                SP += 1;
            }
            //LSS
            else if(M==7){
                PAS[SP+1] = ((PAS[SP+1] < PAS[SP]) ? 1 : 0);
                SP += 1;
            }
            // LEQ
            else if(M==8){
                PAS[SP+1] = ((PAS[SP+1] <= PAS[SP]) ? 1 : 0);
                SP += 1;
            }
            // GTR
            else if(M==9){
                PAS[SP+1] = ((PAS[SP+1] > PAS[SP]) ? 1 : 0);
                SP += 1;
            }
            // GEQ
            else if(M==10){
                PAS[SP+1] = ((PAS[SP+1] >= PAS[SP]) ? 1 : 0);
                SP += 1;
            }
            // EVEN
            else if(M==11){
                PAS[SP] = (PAS[SP] % 2 == 0);
            }
            print(L, M, PC, BP, SP, OP);
        }
        // LOD
        else if(OP==3){
            int X = base(BP, L);
            SP -= 1;
            PAS[SP] = PAS[X-M];
            print(L, M, PC, BP, SP, OP);
        }
        // STO
        else if(OP==4){
            int X = base(BP, L);
            PAS[X - M] = PAS[SP];
            SP += 1;
            print(L, M, PC, BP, SP, OP);
        }
        // CAL
        else if(OP==5){
            int X = base(BP, L);
            PAS[SP-1] = X;
            PAS[SP-2] = BP;
            PAS[SP-3] = PC;
            BP = SP-1;
            PC = MAX_SIZE-1-M;
            print(L, M, PC, BP, SP, OP);
        }
        // INC
        else if(OP==6){
            SP -= M;
            print(L, M, PC, BP, SP, OP);
        }
        // JMP
        else if(OP==7){
            PC = MAX_SIZE - 1 - M;
            print(L, M, PC, BP, SP, OP);
        }
        // JPC
        else if(OP==8){
            if(PAS[SP]==0){
                PC = MAX_SIZE - 1 - M;
            }
            SP += 1;
            print(L, M, PC, BP, SP, OP);
        }
        // SYS
        else if(OP==9){
            // Output an integer
            if(M==1){
                printf("Output result is: %d\n", PAS[SP]);
                SP += 1;
                print(L, M, PC, BP, SP, OP);
            }
            //Read an integer
            if(M==2){
                SP -= 1;
                int input;
                printf("Please Enter an Integer: ");
                if (scanf("%d", &input) != 1) {
                    printf("Error reading input\n");
                }
                PAS[SP] = input;
                print(L, M, PC, BP, SP, OP);
            }
            // Halt the program
            if(M==3){
                halt = 0;
                print(L, M, PC, BP, SP, OP);
            }
        } 
    }
    return 0;
}