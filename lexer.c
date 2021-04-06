/************************************************************************
University of Leeds
School of Computing
COMP2932- Compiler Design and Construction
Lexer Module

I confirm that the following code has been developed and written by me and it is entirely the result of my own work.
I also confirm that I have not copied any parts of this program from another person or any other source or facilitated someone to copy this program from me.
I confirm that I will not publish the program online or share it with anyone without permission of the module leader.

Student Name: Giorgos Kosta
Student ID: 201257368
Email: sc19gk@leeds.ac.uk
Date Work Commenced: 22/02/2021
*************************************************************************/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"


// YOU CAN ADD YOUR OWN FUNCTIONS, DECLARATIONS AND VARIABLES HERE

FILE *fptr;
unsigned int lineNumber=0;
char *resWords[21];
char symbols[20];
char* filename;
char *buf;
int currTokenIndex=-1;
int nch = 0;





// IMPLEMENT THE FOLLOWING functions
//***********************************


void setUpResWords(){
    for(int i=0; i<21; i++){
        resWords[i]=calloc(16,sizeof(char*));
    }
    //Program components
    strcpy(resWords[0],"class");
    strcpy(resWords[1],"constructor");
    strcpy(resWords[2],"method");
    strcpy(resWords[3],"function");

    //Primitive types
    strcpy(resWords[4],"int");
    strcpy(resWords[5],"boolean");
    strcpy(resWords[6],"char");
    strcpy(resWords[7],"void");

    //Variable declarations
    strcpy(resWords[8],"var");
    strcpy(resWords[9],"static");
    strcpy(resWords[10],"field");

    //Statements
    strcpy(resWords[11],"let");
    strcpy(resWords[12],"do");
    strcpy(resWords[13],"if");
    strcpy(resWords[14],"else");
    strcpy(resWords[15],"while");
    strcpy(resWords[16],"return");

    //Constant values
    strcpy(resWords[17],"true");
    strcpy(resWords[18],"false");
    strcpy(resWords[19],"null");

    //Object reference
    strcpy(resWords[20],"this");

}

//return 1 if reserved word, otherwise 0
int isResWord(char *lexeme){

    for(int i=0;i<21;i++){
        if(strcmp(lexeme,resWords[i])==0)
            return 1;
    }
    return 0;
}


int isLegalID(char* id){

    for(int i=0;i<strlen(id);i++){
        if(! ( (id[i]>=65 && id[i] <= 90) || (id[i]>=48 && id[i] <= 57) || (id[i]>=97 && id[i] <= 122) || id[i]==95))
            return 0;
    }

    return 1;
}


void setUpSymbols(){

    symbols[0] = '(';
    symbols[1] = ')';
    symbols[2] = '[';
    symbols[3] = ']';
    symbols[4] = '{';
    symbols[5] = '}';
    symbols[6] = ',';
    symbols[7] = ';';
    symbols[8] = ';';
    symbols[9] = '=';
    symbols[10] = '.';
    symbols[11] = '+';
    symbols[12] = '-';
    symbols[13] = '*';
    symbols[14] = '/';
    symbols[15] = '&';
    symbols[16] = '|';
    symbols[17] = '~';
    symbols[18] = '<';
    symbols[19] = '>';

}

//returns 1 if symbol, otherwise 0
int isSymbol(char c){

    for(int i=0;i<20;i++){
        if(c==symbols[i])
            return 1;
    }
    return 0;

}

// Initialise the lexer to read from source file
// file_name is the name of the source file
// This requires opening the file and making any necessary initialisations of the lexer
// If an error occurs, the function should return 0
// if everything goes well the function should return 1
int InitLexer (char* file_name)
{
    nch=0;
    filename=file_name;
    currTokenIndex=-1;
    lineNumber=0;

    fptr = fopen(file_name,"r+");
    if(!fptr) {
        printf("File: %s does not exist");
        return 0;
    }else{

        printf("File opened succesfuly, running lexer...\n\n");
        lineNumber=1;
        setUpResWords();
        setUpSymbols();
        char currC;


        unsigned int size = sizeof(char);
        buf = malloc(size);
        buf =0;
        while((currC = (char)fgetc(fptr)) != EOF){

            if(nch >= size-1){ //reallocate more memory

                size += 10*size;
                buf = realloc(buf, size);

            }

            buf[nch++] = currC;
        }
        return 1;
    }



}




TokenType getTokenType(char *lexeme){
    for(int i=0;i<21;i++){
        if(strcmp(lexeme,resWords[i])==0)
            return RESWORD;
    }
    for(int i=0;i<20;i++){
        if(strcmp(lexeme,symbols[i])==0)
            return SYMBOL;
    }


}


// Get the next token from the source file
Token GetNextToken ()
{

    Token t;
    t.tp=0;
    *t.fl=0;
    t.ln=0;
    *t.lx=0;
    t.ec=0;



    unsigned int lexC_counter=0;
    char currLexeme[128];

    for(int i=0;i<128;i++){
        t.lx[i] =0;
        currLexeme[i]='\0';
        if(i<32)
            t.fl[i]='\0';
    }

    char  currC;

    if(nch) {
        currC = buf[++currTokenIndex];


        while (1) {//skip comments, \n, \r and spaces

            if (currC == '/' && (buf[currTokenIndex + 1] == '/')) {

                while (currC != 10 && currC != 13) {
                    currC = buf[++currTokenIndex];
                }

                continue;

            } else if (currC == '/' && buf[currTokenIndex + 1] == '*') {
                currTokenIndex += 2;
                currC = buf[++currTokenIndex];

                while (!(currC == '*' && (buf[currTokenIndex + 1] == '/'))) {
                    if ((currC == 13) && (buf[currTokenIndex + 1] == 10)) {
                        currTokenIndex++;
                        lineNumber++;
                        currC = buf[++currTokenIndex];
                        continue;
                    } else if ((currC == 10) || (currC == 13))
                        lineNumber++;
                    if(currC==0){
                        strcpy(t.lx,"Error: unexpected eof in comment");
                        strcpy(t.fl,filename);
                        t.ln=lineNumber;
                        t.tp=ERR;
                        return t;
                    }
                    currC = buf[++currTokenIndex];


                }
                currTokenIndex++;
                currC = buf[++currTokenIndex];
                continue;
            } else if (currC == 10 || currC == 13) {
                if ((currC == 13) && (buf[currTokenIndex + 1] == 10))
                    currTokenIndex++;
                currC = buf[++currTokenIndex];
                lineNumber++;
                continue;
            } else if (currC == 9 || currC == 32) {
                currC = buf[++currTokenIndex];
                continue;
            } else
                break;
        }

        if(lineNumber>=63)
            printf("d");

        if(currC  == 0){
            strcpy(t.lx, "End of File");
            t.ln = lineNumber;
            t.tp = EOFile;
            strcpy(t.fl, filename);
            return t;
        }

        if (isSymbol(currC)) {//symbol
            strcpy(t.lx, &currC);
            t.tp = SYMBOL;
            t.ln = lineNumber;
            if (currC == 10 || currC == 13) {
                if ((currC == 13) && (buf[currTokenIndex + 1] == 10))
                    currTokenIndex++;
                lineNumber++;
            }
            t.ec = 0;
            strcpy(t.fl, filename);
            return t;

        } else if (currC == '"') {//String
            while (1) {
                currC = buf[++currTokenIndex];
                if(currC==0){
                    strcpy(t.lx,"Error: unexpected eof in string constant");
                    strcpy(t.fl,filename);
                    t.ln=lineNumber;
                    t.tp=ERR;
                    t.ec=EofInStr;
                    return t;
                }
                if((currC == 10) || (currC ==13)){
                    strcpy(t.lx, "Error: new line in string constant");
                    strcpy(t.fl,filename);
                    t.ln=lineNumber;
                    t.tp = ERR;
                    t.ec = NewLnInStr;
                    return t;
                }
                if (currC == '"')
                    break;
                else {
                    currLexeme[lexC_counter++] = currC;

                }

            }
            t.tp = STRING;
            strcpy(t.lx, currLexeme);
            strcpy(t.fl, filename);
            t.ln = lineNumber;
            t.ec = 0;
            return t;

        } else if (currC >= 48 && currC <= 57) {//INT
            currLexeme[lexC_counter++] = currC;
            while (1) {
                currC = buf[++currTokenIndex];
                if (!(currC >= 48 && currC <= 57))
                    break;
                else
                    currLexeme[lexC_counter++] = currC;
            }


            t.tp = INT;
            strcpy(t.lx, currLexeme);
            strcpy(t.fl, filename);
            t.ln = lineNumber;
            if (currC == 10 || currC == 13) {
                if ((currC == 13) && (buf[currTokenIndex + 1] == 10))
                    currTokenIndex++;
                lineNumber++;
            }
            t.ec = 0;
            if (isSymbol(currC))
                currTokenIndex--;
            return t;


        } else {

            while (1) {


                if (currC == 9 || currC == 32 || currC == 10 || currC == 13 || isSymbol(currC)) {
                    if (isResWord(currLexeme)) {
                        strcpy(t.lx, currLexeme);
                        t.ln = lineNumber;
                        if (currC == 10 || currC == 13) {
                            if ((currC == 13) && (buf[currTokenIndex + 1] == 10))
                                currTokenIndex++;
                            lineNumber++;
                        }
                        strcpy(t.fl, filename);
                        t.ec = 0;
                        t.tp = RESWORD;
                        if (isSymbol(currC))
                            currTokenIndex--;
                        return t;
                    } else {
                        strcpy(t.lx, currLexeme);
                        strcpy(t.fl, filename);
                        t.ln = lineNumber;
                        if (currC == 10 || currC == 13) {
                            if ((currC == 13) && (buf[currTokenIndex + 1] == 10))
                                currTokenIndex++;
                            lineNumber++;
                        }
                        t.ec = 0;
                        t.tp = ID;
                        if(!isLegalID(t.lx)){
                            strcpy(t.lx,"Error: illegal symbol in source file");
                            t.tp=ERR;
                            t.ec=IllSym;
                            return t;
                        }
                        if (isSymbol(currC))
                            currTokenIndex--;
                            return t;
                    }

                }


                if (currTokenIndex == nch) {
                    if (isResWord(currLexeme)) {
                        strcpy(t.lx, currLexeme);
                        t.ln = lineNumber;
                        strcpy(t.fl, filename);
                        t.ec = 0;
                        t.tp = RESWORD;
                        return t;
                    } else {
                        strcpy(t.lx, currLexeme);
                        strcpy(t.fl, filename);
                        t.ln = lineNumber;
                        t.ec = 0;
                        t.tp = ID;
                        return t;
                    }
                }



                currLexeme[lexC_counter++] = currC;
                currC = buf[++currTokenIndex];
            }


        }


    }else{
        t.tp=EOFile;
        strcpy(t.lx,"End of File");
        strcpy(t.fl,filename);
        t.ln=lineNumber;
    }





}

// peek (look) at the next token in the source file without removing it from the stream
Token PeekNextToken ()
{
  Token t;

  int tmpIndex = currTokenIndex;
  int tmpLineNumber = lineNumber;

  t = GetNextToken();
  currTokenIndex = tmpIndex;
  lineNumber = tmpLineNumber;

  return t;
}

// clean out at end, e.g. close files, free memory, ... etc
int StopLexer ()
{
    fclose(fptr);
    filename=0;

    for(int i=0;i<nch;i++){
        buf[i]=0;
    }

    free(buf);


    return 0;
}


// do not remove the next line
#ifndef TEST
//int main ()
//{
//	// implement your main function here
//  // NOTE: the autograder will not use your main function
//
////    printf("%s",buf);
//
//    Token t;
//    char *type=calloc(15,sizeof(char));
//
//    if(InitLexer("KeywordsOnly.jack"))
//        while(t.tp!=EOFile) {
//            t = GetNextToken();
//            switch(t.tp){
//                case RESWORD:
//                    strcpy(type,"RESWORD");
//                    break;
//                case ID:
//                    strcpy(type,"ID");
//                    break;
//                case SYMBOL:
//                    strcpy(type,"SYMBOL");
//                    break;
//                case INT:
//                    strcpy(type,"INT");
//                    break;
//                case STRING:
//                    strcpy(type,"STRING");
//                    break;
//                case EOFile:
//                    strcpy(type,"EOFile");
//                    break;
//
//                default:
//                    strcpy(type,"ERR");
//                    break;
//
//            }
//            printf("< %s, %d, %s, %s >\n", t.fl,t.ln,t.lx, type);
//        }
//
//    StopLexer();
//	return 0;
//}
// do not remove the next line
#endif
