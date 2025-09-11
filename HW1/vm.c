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
# include <string.h>
# define MAX_SIZE 500

int PAS[MAX_SIZE] = {0};

int base(int BP, int L){
    int arb = BP;
    while(L>0){
        arb = PAS[arb];
        L--;
    }
    return arb;
}

void print(int L, int M, int PC, int BP, int SP, int OP){
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
    printf("\t%d\t%d\t%d\t%d\t%d\t", L, M, PC, BP, SP);

    // ...existing code...
    // Stack printing
    int show_bar = 0, callerBP = 0;
    if (BP >= 3) {
        int DL = PAS[BP - 1];      /* caller BP stored by CAL */
        int RA = PAS[BP - 2];      /* return address */
        int header_present = (DL != 0 || RA != 0);          /* not main frame */
        int locals_exist   = header_present && ((BP - 3) >= SP);  /* callee did INC */
        if (locals_exist) {
            show_bar = 1;
            callerBP = DL;
        }
    }

    if (!show_bar) {
        // Print the entire stack from BP down to SP, even if no locals yet
        for (int i = BP; i >= SP; --i) printf(" %d", PAS[i]);
        printf("\n");
        return;
    }

    // Print locals above AR header, if any
    for (int i = callerBP; i >= BP + 1; --i)
        printf(" %d", PAS[i]);
    printf(" |");
    // Print AR header (SL, DL, RA, ?)
    for (int i = BP; i >= BP - 3; --i)
        printf(" %d", PAS[i]);
    // Print any remaining stack values below AR header
    for (int i = BP - 4; i >= SP; --i)
        printf(" %d", PAS[i]);
    printf("\n");
// ...existing code...
    // printf("\t%d %d %d %d %d %d\n", PAS[BP], PAS[BP-1], PAS[BP-2], PAS[BP-3], PAS[BP-4], PAS[SP]);
}

int main(void){
    printf("Enter the file name: ");
    char filename[50];
    scanf("%s", filename);

    FILE * ifp;
    ifp = fopen(filename, "r");
    
    int tracker = 499;
    while(!feof(ifp)){
        fscanf(ifp, "%d", &PAS[tracker]);
        tracker--;
        fscanf(ifp, "%d", &PAS[tracker]);
        tracker--;
        fscanf(ifp, "%d", &PAS[tracker]);
        tracker--;
    }

    int PC = 499;
    int SP = tracker+1;
    int BP = SP-1;

    printf("\tL\tM\tPC\tBP\tSP\tstack\n");
    printf("Initial values:\t\t%d\t%d\t%d\n", PC, BP, SP);

    int halt = 1;
    // PAS[PC-1] = L
    // PAS[PC-2] = M
    while(halt){
        // printf("Inside while loop\n");
        int OP = PAS[PC];
        int L = PAS[PC-1];
        int M = PAS[PC-2];
        PC -= 3;
        if(OP==1){
            // L = base(BP, PC-1);
            SP -= 1;
            PAS[SP] = M;
            print(L, M, PC, BP, SP, OP);   
        }
        else if(OP==2){
            if(M==0){
                SP = BP + 1;
                BP = PAS[SP-2];
                PC = PAS[SP-3];
            }
            else if(M==1){
                PAS[SP+1] = PAS[SP+1] + PAS[SP];
                SP += 1;
            }
            else if(M==2){
                PAS[SP+1] = PAS[SP+1] - PAS[SP];
                SP += 1;
            }
            else if(M==3){
                PAS[SP+1] = PAS[SP+1] * PAS[SP];
                SP += 1;
            }
            else if(M==4){
                PAS[SP+1] = PAS[SP+1] / PAS[SP];
                SP += 1;
            }
            else if(M==5){
                PAS[SP+1] = ((PAS[SP+1] == PAS[SP]) ? 1 : 0);
                SP += 1;
            }
            else if(M==6){
                PAS[SP+1] = ((PAS[SP+1] != PAS[SP]) ? 1 : 0);
                SP += 1;
            }
            else if(M==7){
                PAS[SP+1] = ((PAS[SP+1] < PAS[SP]) ? 1 : 0);
                SP += 1;
            }
            else if(M==8){
                PAS[SP+1] = ((PAS[SP+1] <= PAS[SP]) ? 1 : 0);
                SP += 1;
            }
            else if(M==9){
                PAS[SP+1] = ((PAS[SP+1] > PAS[SP]) ? 1 : 0);
                SP += 1;
            }
            else if(M==10){
                PAS[SP+1] = ((PAS[SP+1] >= PAS[SP]) ? 1 : 0);
                SP += 1;
            }
            print(L, M, PC, BP, SP, OP);
        }
        else if(OP==3){
            int X = base(BP, L);
            SP -= 1;
            PAS[SP] = PAS[X-M];
            print(L, M, PC, BP, SP, OP);
        }
        else if(OP==4){
            int X = base(BP, L);
            PAS[X - M] = PAS[SP];
            SP += 1;
            print(L, M, PC, BP, SP, OP);
        }
        else if(OP==5){
            int X = base(BP, L);
            PAS[SP-1] = X;
            PAS[SP-2] = BP;
            PAS[SP-3] = PC;
            BP = SP-1;
            PC = 499-M;
            print(L, M, PC, BP, SP, OP);
        }
        else if(OP==6){
            SP -= M;
            print(L, M, PC, BP, SP, OP);
        }
        else if(OP==7){
            PC = 499 - M;
            print(L, M, PC, BP, SP, OP);
        }
        else if(OP==8){
            if(PAS[SP]==0){
                PC = 499 - M;
            }
            SP += 1;
            print(L, M, PC, BP, SP, OP);
        }
        else if(OP==9){
            if(M==1){
                printf("Output result is: %d\n", PAS[SP]);
                SP += 1;
                print(L, M, PC, BP, SP, OP);
            }
            if(M==2){
                SP -= 1;
                int input;
                printf("Please Enter an Integer: ");
                scanf("%d", &input);
                PAS[SP] = input;
                print(L, M, PC, BP, SP, OP);
            }
            if(M==3){
                halt = 0;
                print(L, M, PC, BP, SP, OP);
            }
        }
        
    }
    
    return 0;
}