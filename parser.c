#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "lexer.h"
#include "parser.h"
char *resWords[21];
char symbols[20];

// you can declare prototypes of parser functions below



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

int isKeyword(char* lexeme){
    for(int i=0;i<21;i++){
        if(strcmp(lexeme,resWords[i])==0)
            return 1;
    }
    return 0;
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

int isSymbol(char* lexeme){
    for(int i=0;i<20;i++){
        if(strcmp(lexeme,&symbols[i])==0)
            return 1;
    }
    return 0;
}



//lexical elements

void keyword(){
    Token token = GetNextToken();
    if(isKeyword(token.lx))
        ;
    else
        printf("Error: keyword expected");

}

void symbol(){
    Token token = GetNextToken();
    if(isSymbol(token.lx))
        ;
    else
        printf("Error: symbol expected at %s:%d",token.fl,token.ln);

}

void type(){
    Token token = GetNextToken();
    if(strcmp(token.lx,"int")==0 ||  strcmp(token.lx,"char")==0||  strcmp(token.lx,"boolean")==0 ||  token.tp==ID)
        ;
    else
        printf("Error: 'int', 'char', 'boolean' or another identifier expected at %s:%d",token.fl,token.ln);
}

void paramList(){// type identifier {, type identifier} | ε
    Token token = PeekNextToken();

    if(strcmp(token.lx,"int")==0 ||  strcmp(token.lx,"char")==0||  strcmp(token.lx,"boolean")==0 ||  token.tp==ID) {
        token = GetNextToken();
        token = GetNextToken();
        if (token.tp == ID)
            ;
        else
            printf("Error: identifier expected at %s:%d", token.fl, token.ln);


        token = GetNextToken();
        if (strcmp(token.lx, ",") == 0) {
            Token next_token = PeekNextToken(); // notice the peek
            while (next_token.tp == ID) { // notice the while
                token = GetNextToken();
                if (strcmp(token.lx, ",") == 0)
                    ;
                else
                    printf("Error: identifier expected at %s:%d", token.fl, token.ln);

                next_token = PeekNextToken();

            }
        }
    }

}

void varDeclarStatement(){// var type identifier { , identifier } ;
    Token token = GetNextToken();
    if(strcmp(token.lx,"var")==0)
        ;
    else
        printf("Error: 'var' expected at %s:%d",token.fl,token.ln);

    type();

    token = GetNextToken();
    if(token.tp==ID)
        ;
    else
        printf("Error: identifier expected at %s:%d",token.fl,token.ln);

    token = GetNextToken();
    if (strcmp(token.lx, ",") == 0) {
        Token next_token = PeekNextToken(); // notice the peek
        while (next_token.tp == ID) { // notice the while
            token = GetNextToken();
            if (strcmp(token.lx, ",") == 0)
                ;
            else
                printf("Error: identifier expected at %s:%d", token.fl, token.ln);

            next_token = PeekNextToken();

        }
    }else if(strcmp(token.lx,";")==0)
        ;
    else
        printf("Error: ';' expected at %s:%d",token.fl,token.ln);


}


void factor(){
    Token next_token = PeekNextToken();
    if(strcmp(next_token.lx,"-")==0 || strcmp(next_token.lx,"~")==0)
        next_token=GetNextToken();

    operand();

}




void term(){
    factor();
    Token next_token = PeekNextToken();

    while(strcmp(next_token.lx,"*")==0 || strcmp(next_token.lx,"/")==0){
        factor();
        next_token=PeekNextToken();
    }
}

void ArithmeticExpression(){
    term();
    Token next_token = PeekNextToken();

    while(strcmp(next_token.lx,"+")==0 || strcmp(next_token.lx,"-")==0){
        term();
        next_token=PeekNextToken();
    }

}

void relationalExpression(){
    ArithmeticExpression();
    Token next_token = PeekNextToken();

    while(strcmp(next_token.lx,"=")==0 || strcmp(next_token.lx,">")==0 || strcmp(next_token.lx,"<")==0){
        ArithmeticExpression();
        next_token=PeekNextToken();
    }
}
void expression(){
    relationalExpression();
    Token next_token = PeekNextToken();
    while(strcmp(next_token.lx,"&")==0 || strcmp(next_token.lx,"|")==0){
        relationalExpression();
        next_token=PeekNextToken();
    }

}

void expressionList(){
    expression();
    Token next_token = PeekNextToken();

    if(strcmp(next_token.lx,",")==0){
        expression();
        next_token=PeekNextToken();
        while(strcmp(next_token.lx,",")==0){
            expression();
            next_token=PeekNextToken();
        }

    }
}

void operand(){
    Token token = GetNextToken();

    if(token.tp == INT || token.tp==ID)
        ;
    else
        printf("Error: identifier or integer constant expected at %s:%d",token.fl,token.ln);

    Token next_token = PeekNextToken();
    if(strcmp(token.lx,".")==0) {
        token = GetNextToken();
        token = GetNextToken();
        if(token.tp ==ID)
            ;
        else
            printf("Error: identifier expected at %s:%d",token.fl,token.ln);
    }

    next_token = PeekNextToken();

    if(strcmp(token.lx,"[")==0)
        expression();
    else if(strcmp(token.lx,"(")==0)
        expressionList();
    else if(strcmp(token.lx,"true")==0)
        ;
    else if(strcmp(token.lx,"false")==0)
        ;
    else if(strcmp(token.lx,"null")==0)
        ;
    else if(strcmp(token.lx,"this")==0)
        ;
    else if(token.tp == STRING)
        ;
    else
        printf("Error: '[', '(', 'true', 'false', 'null', 'this' or string literal expected at %s:%d",token.fl,token.ln);

}










void letStatement() {//let identifier [ [ expression ] ] = expression ;

    Token token = GetNextToken();

    if (strcmp(token.lx, "let") == 0);
    else
        printf("Error: 'let' expected at %s:%d", token.fl, token.ln);

    Token next_token = PeekNextToken();

    if (strcmp(next_token.lx, "[") == 0){
        token = GetNextToken();
        expression();
        token = GetNextToken();
        if(strcmp(token.lx, "[") == 0)
            ;
        else
            printf("Error: ']' expected at %s:%d",token.fl,token.ln);

    }else
        printf("Error: expression expected at %s:%d",next_token.fl,next_token.ln);

    token = GetNextToken();
    if(strcmp(token.lx, "=") == 0)
        ;
    else
        printf("Error: '=' expected at %s:%d",token.fl,token.ln);

    expression();
    token = GetNextToken();
    if(strcmp(token.lx, ";") == 0)
        ;
    else
        printf("Error: ';' expected at %s:%d",token.fl,token.ln);



    if(token.tp==ID)
        ;
    else
        printf("Error: identifier expected at %s:%d",token.fl,token.ln);


    token = GetNextToken();

    if(strcmp(token.lx,"let")==0)
        ;
    else
        printf("Error: 'let' expected at %s:%d",token.fl,token.ln);


}

void statement(){
    Token token = PeekNextToken();
    if(strcmp(token.lx,"var")==0)//varDeclarStatement
        varDeclarStatement();
    else if(strcmp(token.lx,"let")==0)//letStatement
        letStatement();
    else if(strcmp(token.lx,"if")==0)//ifStatement
        ;
    else if(strcmp(token.lx,"while")==0)//whileStatement
        ;
    else if(strcmp(token.lx,"do")==0)//doStatement
        ;
    else if(strcmp(token.lx,"return")==0)//returnStatement
        ;
    else
        printf("Error: 'var', 'let', 'if', 'while', 'do' or 'return' expected at %s:%d",token.fl,token.ln);
}




void ifStatement(){
    Token token = GetNextToken();
    if(strcmp(token.lx,"if")==0)
        ;
    else
        printf("Error: 'if' expected at %s:%d",token.fl,token.ln);

    token = GetNextToken();
    if(strcmp(token.lx,"(")==0)
        ;
    else
        printf("Error: '(' expected at %s:%d",token.fl,token.ln);

    expression();
    token = GetNextToken();
    if(strcmp(token.lx,")")==0)
        ;
    else
        printf("Error: ')' expected at %s:%d",token.fl,token.ln);


    token = GetNextToken();
    if(strcmp(token.lx,"{")==0)
        ;
    else
        printf("Error: '{' expected at %s:%d",token.fl,token.ln);


    Token next_token = PeekNextToken ();
    while ( strcmp(next_token.lx,"var")==0 ||
            strcmp(next_token.lx,"if")==0 ||
            strcmp(next_token.lx,"let")==0 ||
            strcmp(next_token.lx,"do")== 0 ||
            strcmp(next_token.lx,"return")==0 ||
            strcmp(next_token.lx,"while")==0)
    {
        statement();
        next_token = PeekNextToken();
    }
    token = GetNextToken();
    if(strcmp(token.lx,"}")==0)
        ;
    else
        printf("Error: '}' expected at %s:%d",token.fl,token.ln);

    next_token = PeekNextToken();
    if(strcmp(next_token.lx,"else")==0){
        token = GetNextToken();
        token = GetNextToken();
        if(strcmp(token.lx,"{")==0)
            ;
        else
            printf("Error: '{' expected at %s:%d",token.fl,token.ln);
        next_token = PeekNextToken ();
        while ( strcmp(next_token.lx,"var")==0 ||
                strcmp(next_token.lx,"if")==0 ||
                strcmp(next_token.lx,"let")==0 ||
                strcmp(next_token.lx,"do")== 0 ||
                strcmp(next_token.lx,"return")==0 ||
                strcmp(next_token.lx,"while")==0)
        {
            statement();
            next_token = PeekNextToken();
        }
        token = GetNextToken();
        if(strcmp(token.lx,"}")==0)
            ;
        else
            printf("Error: '}' expected at %s:%d",token.fl,token.ln);

    }

}


void whileStatement(){
    Token token = GetNextToken();
    if(strcmp(token.lx,"while")==0)
        ;
    else
        printf("Error: 'while' expected at %s:%d",token.fl,token.ln);

    token = GetNextToken();
    if(strcmp(token.lx,"(")==0)
        ;
    else
        printf("Error: '(' expected at %s:%d",token.fl,token.ln);

    expression();
    token = GetNextToken();
    if(strcmp(token.lx,")")==0)
        ;
    else
        printf("Error: ')' expected at %s:%d",token.fl,token.ln);


    token = GetNextToken();
    if(strcmp(token.lx,"{")==0)
        ;
    else
        printf("Error: '{' expected at %s:%d",token.fl,token.ln);


    Token next_token = PeekNextToken ();
    while ( strcmp(next_token.lx,"var")==0 ||
            strcmp(next_token.lx,"if")==0 ||
            strcmp(next_token.lx,"let")==0 ||
            strcmp(next_token.lx,"do")== 0 ||
            strcmp(next_token.lx,"return")==0 ||
            strcmp(next_token.lx,"while")==0)
    {
        statement();
        next_token = PeekNextToken();
    }
    token = GetNextToken();
    if(strcmp(token.lx,"}")==0)
        ;
    else
        printf("Error: '}' expected at %s:%d",token.fl,token.ln);
}



void returnStatement(){
    Token token = GetNextToken();
    if(strcmp(token.lx,"return")==0)
        ;
    else
        printf("Error: 'return' expected at %s:%d",token.fl,token.ln);

    token = GetNextToken();
    if(strcmp(token.lx,"-")==0 || strcmp(token.lx,"~")==0 || token.tp==INT || token.tp==ID)
        expression();

    token = GetNextToken();
    if(strcmp(token.lx,";")==0)
        ;
    else
        printf("Error: ';' expected at %s:%d",token.fl,token.ln);


}

void subroutineCall(){
    Token token = GetNextToken();
    if(token.tp==ID)
        ;
    else
        printf("Error: identifier expected at %s:%d",token.fl,token.ln);

    Token next_token = PeekNextToken();
    if(strcmp(next_token.lx,".")==0){
        token = GetNextToken();
        token = GetNextToken();
        if(token.tp==ID)
            ;
        else
            printf("Error: identifier expected at %s:%d",token.fl,token.ln);
    }

    token = GetNextToken();

    if(strcmp(token.lx,"(")==0)
        ;
    else
        printf("Error: '(' expected at %s:%d",token.fl,token.ln);

    expressionList();

    token = GetNextToken();

    if(strcmp(token.lx,")")==0)
        ;
    else
        printf("Error: ')' expected at %s:%d",token.fl,token.ln);


}

void doStatement(){
    Token token = GetNextToken();
    if(strcmp(token.lx,"do")==0)
        ;
    else
        printf("Error: 'do' expected at %s:%d",token.fl,token.ln);

    subroutineCall();

}



void subroutineBody(){
    Token token = GetNextToken();
    if(strcmp(token.lx,"{")==0)
        ;
    else
        printf("Error: '(' expected at %s:%d",token.fl,token.ln);

    Token next_token = PeekNextToken();
    while(next_token.tp!=EOFile || strcmp(token.lx,"}")==0)
        statement();

    token = GetNextToken();
    if(strcmp(token.lx,"}")==0)
        ;
    else
        printf("Error: '}' expected at %s:%d",token.fl,token.ln);}



void subroutineDeclar(){// (constructor | function | method) (type|void) identifier (paramList) subroutineBody
    Token token = GetNextToken();
    if(strcmp(token.lx,"constructor")==0 || strcmp(token.lx,"function")==0 || strcmp(token.lx,"method")==0)
        ;
    else
        printf("Error: 'constructor', 'function', 'method' expected at %s:%d",token.fl,token.ln);

    token = GetNextToken();
    if(strcmp(token.lx,"void")==0 || strcmp(token.lx,"int")==0 ||  strcmp(token.lx,"char")==0||  strcmp(token.lx,"boolean")==0 ||  token.tp==ID)
        ;
    else
        printf("Error: 'void', 'int', 'char', 'boolean' or another identifier expected at %s:%d",token.fl,token.ln);


    token = GetNextToken();
    if(token.tp==ID)
        ;
    else
        printf("Error: identifier expected at %s:%d",token.fl,token.ln);



    token = GetNextToken();
    if(strcmp(token.lx,"(")==0)
        ;
    else
        printf("Error: '(' expected at %s:%d",token.fl,token.ln);

    paramList();

    token = GetNextToken();
    if(strcmp(token.lx,")")==0)
        ;
    else
        printf("Error: ')' expected at %s:%d",token.fl,token.ln);


    subroutineBody();


}


void memberDeclar(){
    Token token = GetNextToken();

    if(strcmp(token.lx,"static")==0 ||  strcmp(token.lx,"field")==0){
        type();
        token = GetNextToken();
        if(token.tp==ID) {
            token = GetNextToken();
            if (strcmp(token.lx, ",") == 0) {
                Token next_token = PeekNextToken(); // notice the peek
                while (next_token.tp == ID) { // notice the while
                    token= GetNextToken();
                    if(strcmp(token.lx,",")==0)
                        ;
                    else
                        printf("Error: identifier expected at %s:%d", token.fl, token.ln);

                    next_token = PeekNextToken();

                }
            }else if(strcmp(token.lx, ";") == 0)
                ;
            else
                printf("Error: ';' expected at %s:%d", token.fl, token.ln);

        }else
            printf("Error: identifier expected at %s:%d",token.fl,token.ln);


    }else if(strcmp(token.lx,"constructor")==0 || strcmp(token.lx,"function")==0 || strcmp(token.lx,"method")==0)
        subroutineDeclar();
    else
        printf("Error: 'constructor', 'function', 'method','static' or 'field' expected at %s:%d",token.fl,token.ln);


}

void classDeclar(){
    Token token = GetNextToken();
    if(strcmp(token.lx,"class")==0)
        ;
    else
        printf("Error: 'class' expected at %s:%d",token.fl,token.ln);

    token = GetNextToken();
    if(strcmp(token.lx,"{")==0)
        ;
    else
        printf("Error: '{' expected at %s:%d",token.fl,token.ln);

    Token next_token = PeekNextToken (); // notice the peek
    while (strcmp(next_token.lx,"}")==0) // notice the while
    {
        memberDeclar();
        next_token = PeekNextToken ();
    }

    token = GetNextToken();
    if(strcmp(token.lx,"}")==0)
        ;
    else
        printf("Error: '}' expected at %s:%d",token.fl,token.ln);



}





int InitParser (char* file_name)
{
    if (InitLexer(file_name)){
        return 1;

    }
    return 0;
}

ParserInfo Parse ()
{
	ParserInfo pi;

	// implement the function


	pi.er = none;
	return pi;
}


int StopParser ()
{
	return 1;
}

#ifndef TEST_PARSER
int main ()
{

	return 1;
}
#endif
