/*
Assignment :
vm.c - Implement a P- machine virtual machine
Authors : Archi Mehta, Ramyanaga Nagarur
Language : C ( only )
To Compile :
gcc -O2 -Wall -std=c11 -o vm vm.c
To Execute (on Eustis ):
./ vm input .txt
where :
input .txt is the name of the file containing PM /0 instructions ;
each line has three integers (OP L M)
Notes :
- Implements the PM /0 virtual machine described in the homework
instructions .
- No dynamic memory allocation or pointer arithmetic .
- Does not implement any VM instruction using a separate function .
- Runs on Eustis .
Class : COP 3402 - Systems Software - Fall 2025
Instructor : Dr. Jie Lin
Due Date : Friday , September 12th , 2025
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

// Print the instructions
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

    // Determine if currently in a callee function and if there are local variables
    int inCaller = 0; 
    int hasLocals = 0;
    if (BP >= 2 && BP < MAX_SIZE){
        int RA = PAS[BP-2];
        inCaller = (RA != 0);
        hasLocals = (inCaller && (BP-3) >= SP);
    }

    // Calculate stack bounds for printing
    int leftUpper;
    int leftLower;
    if (inCaller){
        int callerBP = PAS[BP-1];        
        leftUpper = (callerBP >= 0 && callerBP < MAX_SIZE) ? callerBP : BP;
        leftLower = BP + 1;    // Locals start after BP               
    } 
    else{
        leftUpper = BP;
        leftLower = SP;
    }

    if (leftUpper >= MAX_SIZE){
        leftUpper = MAX_SIZE - 1;
    }
    
    if (leftLower < 0){
        leftLower = 0;
    } 

    // Print stack contents for current AR
    if (leftUpper >= leftLower){
        for (int i = leftUpper; i >= leftLower; i--){
            printf("%d ", PAS[i]); 
        }   
    }

    // If there are locals, print static link, dynamic link, return address, and locals
    if (hasLocals){
        printf(" | %d %d %d", PAS[BP], PAS[BP-1], PAS[BP-2]); /* SL DL RA */
        for (int i = BP - 3; i >= SP; i--){
            printf(" %d", PAS[i]);
        }
    }
    printf("\n");
}

int main(int argc, char *argv[]){
    // Check number of arguments
    if (argc != 2) {
        printf("Error: Expected exactly one argument (input file).\n");
        return 1;
    }

    // Open the file
    FILE *ifp = fopen(argv[1], "r");
    if (!ifp) {
        printf("Error: Could not open file %s\n", argv[1]);
        return 1;
    }

    // Get the data
    int tracker = MAX_SIZE - 1; //499
    while (!feof(ifp)) {
        if (fscanf(ifp, "%d", &PAS[tracker]) != 1){
            printf("Error reading PAS value\n");
        }
        tracker--;
        if (fscanf(ifp, "%d", &PAS[tracker]) != 1){
            printf("Error reading PAS value\n");
        }
        tracker--;
        if (fscanf(ifp, "%d", &PAS[tracker]) != 1){
            printf("Error reading PAS value\n");
        }
        tracker--;
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