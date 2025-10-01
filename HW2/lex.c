/*
    Assignment:
    lex - Lexical Analyzer for PL/0

    Author: Archi Mehta, Ramyanaga Nagarur
    Language: C(only)

    To Compile:
        gcc -O2 -std=c11 -o lex lex.c

    To Execute (on Eustis):
        ./lex <input file>

    where:
        <input file> is the path to the PL/0 source program

    Notes:
    - Implement a lexical analyser for the PL/0 language.
    - The program must detect errors such as
        - numbers longer than five digits
        - identifiers longer than eleven characters
        - invalid characters.
    - The output format must exactly match the specification.
    - Tested on Eustis.

    Class: COP 3402 - System Software - Fall 2025

    Instructor: Dr. Jie Lin

    Due Date: Friday , October 3, 2025 at 11:59 PM ET
*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>

typedef enum{
    skipsym = 1,    // Skip / ignore token
    identsym ,      // Identifier
    numbersym ,     // Number
    plussym ,       // +
    minussym ,      // -
    multsym ,       // *
    slashsym ,      // /
    eqsym ,         // =
    neqsym ,        // <>
    lessym ,        // <
    leqsym ,        // <=
    gtrsym ,        // >
    geqsym ,        // >=
    lparentsym ,    // (
    rparentsym ,    // )
    commasym ,      // ,
    semicolonsym ,  // ;
    periodsym ,     // .
    becomessym ,    // :=
    beginsym ,      // begin
    endsym ,        // end
    ifsym ,         // if
    fisym ,         // fi
    thensym ,       // then
    whilesym ,      // while
    dosym ,          // do
    callsym ,       // call
    constsym ,      // const
    varsym ,        // var
    procsym ,       // procedure
    writesym ,      // write
    readsym ,       // read
    elsesym ,       // else
    evensym         // even
}TokenType;

// additional struct to store token list
typedef struct {
    TokenType token;
    char lexeme[12];
}TokenListEntry;

int main(int argc, char *argv[]){
    // check # of arguments
    if(argc != 2){
        printf("Error: Expected exactly one argument (input file).\n");
        return 1;
    }
    
    // open file
    FILE * ifp = fopen(argv[1], "r");

    // print source program on initial read of file
    printf("Source Program:\n\n");
    while(!feof(ifp)){
        printf("%c", fgetc(ifp));
    }
    printf("\n\n");
    
    // go back to beginning of file
    rewind(ifp);
    
    // create a lexeme array to store string values
    char lexeme[12];
    int tracker = 0;
    // create instances of structs
    TokenType token;
    TokenListEntry tokenList[100];
    // keep track of tokenList index
    int listCount = 0;
    int ch;

    printf("Lexeme Table:\n\n");
    printf("lexeme\ttoken type\n");

    // continue looping while characters can be read from file
    while((ch = fgetc(ifp)) != EOF){
        // skip invisible characters (space, newline, tab, or escape)
        if(isspace(ch)){
            continue;
        }
        if(ch == '/'){
            int nextChar = fgetc(ifp);
            if(nextChar == '*'){
                int prev = 0;
                while((ch = fgetc(ifp)) != EOF){
                    if(prev == '*' && ch == '/'){
                        break;
                    }
                    prev = ch;
                }
                continue;
            }
            else{
                ungetc(nextChar, ifp);
            }
        }
        // if the first character is alphabetical
        if(isalpha(ch)){
            tracker = 0;
            // check if the next characters are alphanumeric to be valid identifier
            // or reserved word
            while(isalnum(ch)){
                if(tracker < 12){
                    lexeme[tracker] = ch;
                }
                tracker++;
                ch = fgetc(ifp);
            }
            // end string with null char (if array not full)
            if(tracker < 12){
                lexeme[tracker] = '\0';
            }
            // unget the previous char 
            ungetc(ch, ifp);
            
            // check if reserved word
            if(strcmp("begin", lexeme)==0){
                token = beginsym;
            }
            else if(strcmp("end", lexeme) == 0){
                token = endsym;
            }
            else if(strcmp("if", lexeme) == 0){
                token = ifsym;
            }
            else if(strcmp("fi", lexeme) == 0){
                token = fisym;
            }
            else if(strcmp("then", lexeme) == 0){
                token = thensym;
            }
            else if(strcmp("while", lexeme) == 0){
                token = whilesym;
            }
            else if(strcmp("do", lexeme) == 0){
                token = dosym;
            }
            else if(strcmp("call", lexeme) == 0){
                token = callsym;
            }
            else if(strcmp("const", lexeme) == 0){
                token = constsym;
            }
            else if(strcmp("var", lexeme) == 0){
                token = varsym;
            }
            else if(strcmp("procedure", lexeme) == 0){
                token = procsym;
            }
            else if(strcmp("write", lexeme) == 0){
                token = writesym;
            }
            else if(strcmp("read", lexeme) == 0){
                token = readsym;
            }
            else if(strcmp("else", lexeme) == 0){
                token = elsesym;
            }
            else if(strcmp("even", lexeme) == 0){
                token = evensym;
            }
            // if identifier, check if max 11 characters
            else {
                if(tracker > 11){
                    token = skipsym;
                    printf("%s\tIdentifier too long\n", lexeme);
                }
                else{
                    token = identsym;
                }
            }
            // print lexeme and token
            if(tracker < 11){
                printf("%s\t%d\n", lexeme, token);
            }
        }
        // check if character is digit
        else if(isdigit(ch)){
            tracker = 0;
            while(isdigit(ch)){
                if(tracker < 12){
                    lexeme[tracker] = ch;
                    tracker++;
                }
                ch = fgetc(ifp);
            }
            if(tracker < 12){
                lexeme[tracker] = '\0';
            }
            ungetc(ch, ifp);
            // check if number is max 5 digits
            if(tracker > 5){
                printf("%s\tNumber too long\n", lexeme);
                token = skipsym;
            }
            else{
                token = numbersym;
                printf("%s\t%d\n", lexeme, token);
            }
        }
        // check if char is special symbol
        else{
            if(ch == '+'){
                token = plussym;
            }
            else if(ch == '-'){
                token = minussym;
            }
            else if(ch == '*'){
                token = multsym;
            }
            else if(ch == '/'){
                token = slashsym;
            }
            else if(ch == '='){
                token = eqsym;
            }
            // read first char in the sequence, then check next char
            else if(ch == '<'){
                int nextChar = fgetc(ifp);
                if(nextChar == '>'){
                    token = neqsym;
                    printf("%c%c\t%d\n", ch, nextChar, token);
                }
                else if(nextChar == '='){
                    token = leqsym;
                    printf("%c%c\t%d\n", ch, nextChar, token);
                }
                // unget nextChar if it is not > or =
                else{
                    token = lessym;
                    ungetc(nextChar, ifp);
                }
            }
            else if(ch == '>'){
                int nextChar = fgetc(ifp);
                if(nextChar == '='){
                    token = geqsym;
                    printf("%c%c\t%d\n", ch, nextChar, token);
                }
                // unget nextChar is it is not =
                else{
                    token = gtrsym;
                    ungetc(nextChar, ifp);    
                }
            }
            else if(ch == '('){
                token = lparentsym;
            }
            else if(ch == ')'){
                token = rparentsym;
            }
            else if(ch == ','){
                token = commasym;
            }
            else if(ch == ';'){
                token = semicolonsym;
            }
            else if(ch == '.'){
                token = periodsym;
            }
            else if(ch == ':'){
                int nextChar = fgetc(ifp);
                if(nextChar == '='){
                    token = becomessym;
                    printf("%c%c\t%d\n", ch, nextChar, token);
                }
                else{
                    printf("Invalid symbols\n");
                }
            }
            else{
                printf("Invalid symbols\n");
                continue;
            }
            // print statement for one char special symbols
            if(token != neqsym && token != leqsym && token != geqsym && token !=becomessym){
                printf("%c\t%d\n", ch, token);
            }
        }
        // populate tokenList
        tokenList[listCount].token = token;
        // fill lexeme array with corresponding identifier or number
        if(token == identsym || token == numbersym){
            strcpy(tokenList[listCount].lexeme, lexeme);
        }
        // empty lexeme array
        else{
            tokenList[listCount].lexeme[0] = '\0';
        }
        // increment listCount
        listCount++;

    }
    // print Token List
    printf("\nToken List:\n\n");
    for(int i=0; i<listCount; i++){
        printf("%d ", tokenList[i].token);
        // if identifier or number, also print the regex representation
        if(tokenList[i].token == identsym || tokenList[i].token == numbersym){
            printf("%s ", tokenList[i].lexeme);
        }
    }

    // close file
    fclose(ifp);
    return 0;
}
