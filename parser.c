#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "lexer.h"
#include "parser.h"

// you can declare prototypes of parser functions below





ParserInfo type(){
    ParserInfo parserInfo;
    Token token = GetNextToken();
    parserInfo.tk=token;

    if(strcmp(token.lx,"int")==0 ||  strcmp(token.lx,"char")==0||  strcmp(token.lx,"boolean")==0 ||  token.tp==ID)
        ;
    else{
        parserInfo.er=illegalType;
        parserInfo.tk=token;
        printf("Error: illegal type at %s:%d\n", token.fl, token.ln);
        return parserInfo;
    }

    parserInfo.tk=token;
    parserInfo.er=none;
    return parserInfo;

}

ParserInfo paramList(){// type identifier {, type identifier} | ε
    ParserInfo parserInfo;

    Token token = PeekNextToken();
    parserInfo.tk=token;

    if(strcmp(token.lx,"int")==0 ||  strcmp(token.lx,"char")==0||  strcmp(token.lx,"boolean")==0 ||  token.tp==ID) {
        token = GetNextToken();
        token = GetNextToken();
        if (token.tp == ID)
            ;
        else{
            parserInfo.er=idExpected;
            parserInfo.tk=token;
            printf("Error: identifier expected at %s:%d\n", token.fl, token.ln);
            return parserInfo;
        }

        token = GetNextToken();
        if (strcmp(token.lx, ",") == 0) {
            Token next_token = PeekNextToken(); // notice the peek
            while (next_token.tp == ID) { // notice the while
                token = GetNextToken();
                if (strcmp(token.lx, ",") == 0)
                    ;
                else{
                    parserInfo.er=idExpected;
                    parserInfo.tk=token;
                    printf("Error: identifier expected at %s:%d\n", token.fl, token.ln);
                    return parserInfo;
                }
                next_token = PeekNextToken();

            }
        }
    }
    parserInfo.tk=token;
    parserInfo.er=none;
    return parserInfo;

}

ParserInfo varDeclarStatement(){// var type identifier { , identifier } ;
    ParserInfo parserInfo;
    Token token = GetNextToken();
    parserInfo.tk=token;
    if(strcmp(token.lx,"var")==0)
        ;
    else{
        parserInfo.er=syntaxError;
        parserInfo.tk=token;
        printf("Error: 'var' expected at %s:%d\n", token.fl, token.ln);
        return parserInfo;
    }
    type();

    token = GetNextToken();
    if(token.tp==ID)
        ;
    else{
        parserInfo.er=idExpected;
        parserInfo.tk=token;
        printf("Error: identifier expected at %s:%d\n", token.fl, token.ln);
        return parserInfo;
    }
    token = GetNextToken();
    if (strcmp(token.lx, ",") == 0) {
        Token next_token = PeekNextToken(); // notice the peek
        while (next_token.tp == ID) { // notice the while
            token = GetNextToken();
            if (strcmp(token.lx, ",") == 0)
                ;
            else{
                parserInfo.er=idExpected;
                parserInfo.tk=token;
                printf("Error: 'identifier' expected at %s:%d\n", token.fl, token.ln);
                return parserInfo;
            }
            next_token = PeekNextToken();

        }
    }else if(strcmp(token.lx,";")==0)
        ;
    else{
        parserInfo.er=semicolonExpected;
        parserInfo.tk=token;
        printf("Error: ';' expected at %s:%d\n", token.fl, token.ln);
        return parserInfo;
    }

    parserInfo.tk=token;
    parserInfo.er=none;
    return parserInfo;

}

static ParserInfo expression();
static ParserInfo expressionList();

ParserInfo operand(){
    ParserInfo parserInfo;
    Token token = GetNextToken();
    parserInfo.tk=token;
    if(token.tp == INT || token.tp==ID)
        ;
    else{
        parserInfo.er=syntaxError;
        parserInfo.tk=token;
        printf("Error: identifier or int expected at %s:%d\n", token.fl, token.ln);
        return parserInfo;
    }
    Token next_token = PeekNextToken();
    if(strcmp(next_token.lx,".")==0) {
        token = GetNextToken();
        token = GetNextToken();
        if(token.tp ==ID)
            ;
        else{
            parserInfo.er=idExpected;
            parserInfo.tk=token;
            printf("Error: identifier expected at %s:%d\n", token.fl, token.ln);
            return parserInfo;
        }
    }

    next_token = PeekNextToken();

    if(strcmp(next_token.lx,"[")==0){
        token = GetNextToken();
        expression();
        token = GetNextToken();
        if(strcmp(token.lx,"]")!=0){
            parserInfo.er=closeBracketExpected;
            parserInfo.tk=token;
            printf("Error: ']' expected at %s:%d\n", token.fl, token.ln);
            return parserInfo;
        }
    }
    else if(strcmp(next_token.lx,"(")==0){
        token = GetNextToken();
        expressionList();
        token = GetNextToken();
        if(strcmp(token.lx,")")!=0){
            parserInfo.er=closeBracketExpected;
            parserInfo.tk=token;
            printf("Error: ')' expected at %s:%d\n", token.fl, token.ln);
            return parserInfo;
        }

    }
    else if(strcmp(next_token.lx,"true")==0)
        token = GetNextToken();
    else if(strcmp(next_token.lx,"false")==0)
        token = GetNextToken();
    else if(strcmp(next_token.lx,"null")==0)
        token = GetNextToken();
    else if(strcmp(next_token.lx,"this")==0)
        token = GetNextToken();
    else if(next_token.tp == STRING)
        token = GetNextToken();
    else{
        parserInfo.er=syntaxError;
        parserInfo.tk=token;
        printf("Error: value expected at %s:%d\n", token.fl, token.ln);
        return parserInfo;
    }

    parserInfo.tk=token;
    parserInfo.er=none;
    return parserInfo;
}



ParserInfo factor(){
    ParserInfo parserInfo;
    Token next_token = PeekNextToken();
    if(strcmp(next_token.lx,"-")==0 || strcmp(next_token.lx,"~")==0)
        next_token=GetNextToken();

    operand();

    parserInfo.tk=next_token;
    parserInfo.er=none;
    return parserInfo;

}




ParserInfo term(){
    ParserInfo parserInfo;
    factor();
    Token next_token = PeekNextToken();

    while(strcmp(next_token.lx,"*")==0 || strcmp(next_token.lx,"/")==0){
        next_token=GetNextToken();
        factor();
        next_token=PeekNextToken();
    }
    parserInfo.tk=next_token;
    parserInfo.er=none;
    return parserInfo;
}



ParserInfo ArithmeticExpression(){
    ParserInfo parserInfo;
    term();
    Token next_token = PeekNextToken();

    while(strcmp(next_token.lx,"+")==0 || strcmp(next_token.lx,"-")==0){
        next_token=GetNextToken();
        term();
        next_token=PeekNextToken();
    }
    parserInfo.tk=next_token;
    parserInfo.er=none;
    return parserInfo;
}

ParserInfo relationalExpression(){
    ParserInfo parserInfo;
    ArithmeticExpression();
    Token next_token = PeekNextToken();

    while(strcmp(next_token.lx,"=")==0 || strcmp(next_token.lx,">")==0 || strcmp(next_token.lx,"<")==0){
        next_token=GetNextToken();
        ArithmeticExpression();
        next_token=PeekNextToken();
    }
    parserInfo.tk=next_token;
    parserInfo.er=none;
    return parserInfo;

}


ParserInfo expression(){
    ParserInfo parserInfo;
    relationalExpression();
    Token next_token = PeekNextToken();
    while(strcmp(next_token.lx,"&")==0 || strcmp(next_token.lx,"|")==0){
        next_token=GetNextToken();
        relationalExpression();
        next_token=PeekNextToken();
    }
    parserInfo.tk=next_token;
    parserInfo.er=none;
    return parserInfo;
}

ParserInfo expressionList(){
    expression();
    ParserInfo parserInfo;
    Token next_token = PeekNextToken();

    if(strcmp(next_token.lx,",")==0){
        next_token=GetNextToken();
        expression();
        next_token=GetNextToken();
        while(strcmp(next_token.lx,",")==0){
            expression();
            next_token=PeekNextToken();
        }

    }

    parserInfo.tk=next_token;
    parserInfo.er=none;
    return parserInfo;
}




ParserInfo letStatement() {//let identifier [ [ expression ] ] = expression ;
    ParserInfo parserInfo;
    Token token = GetNextToken();

    if (strcmp(token.lx, "let") == 0)
        ;
    else{
        parserInfo.er=syntaxError;
        parserInfo.tk=token;
        printf("Error: 'let' expected at %s:%d\n", token.fl, token.ln);
        return parserInfo;
    }
    Token next_token = PeekNextToken();

    if (strcmp(next_token.lx, "[") == 0){
        token = GetNextToken();
        expression();
        token = GetNextToken();
        if(strcmp(token.lx, "]") == 0)
            ;
        else{
            parserInfo.er=closeBracketExpected;
            parserInfo.tk=token;
            printf("Error: ']' expected at %s:%d\n", token.fl, token.ln);
            return parserInfo;
        }
    }else if(next_token.tp ==ID)
        next_token=GetNextToken();
    else{
        parserInfo.er=syntaxError;
        parserInfo.tk=token;
        printf("Error: expression expected at %s:%d\n", token.fl, token.ln);
        return parserInfo;
    }
    token = GetNextToken();
    if(strcmp(token.lx, "=") == 0)
        ;
    else{
        parserInfo.er=equalExpected;
        parserInfo.tk=token;
        printf("Error: '=' expected at %s:%d\n", token.fl, token.ln);
        return parserInfo;
    }
    expression();
    token = GetNextToken();
    if(strcmp(token.lx, ";") == 0)
        return parserInfo;

    else{
        parserInfo.er=semicolonExpected;
        parserInfo.tk=token;
        printf("Error: ';' expected at %s:%d\n", token.fl, token.ln);
        return parserInfo;
    }

    if(token.tp==ID)
        ;
    else{
        parserInfo.er=idExpected;
        parserInfo.tk=token;
        printf("Error: identifier expected at %s:%d\n", token.fl, token.ln);
        return parserInfo;
    }

    token = GetNextToken();

    if(strcmp(token.lx,"let")==0)
        ;
    else{
        parserInfo.er=syntaxError;
        parserInfo.tk=token;
        printf("Error: 'let' expected at %s:%d\n", token.fl, token.ln);
        return parserInfo;
    }

    parserInfo.tk=token;
    parserInfo.er=none;
    return parserInfo;
}

ParserInfo statement(){
    ParserInfo parserInfo;
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
    else{
        parserInfo.er=syntaxError;
        parserInfo.tk=token;
        printf("Error: 'var', 'let', 'if', 'while', 'do' or 'return' expected at %s:%d\n",token.fl,token.ln);
        return parserInfo;
    }

    parserInfo.tk=token;
    parserInfo.er=none;
    return parserInfo;
}




ParserInfo ifStatement(){
    ParserInfo parserInfo;
    Token token = GetNextToken();
    if(strcmp(token.lx,"if")==0)
        ;
    else{
        parserInfo.er=syntaxError;
        parserInfo.tk=token;
        printf("Error: 'if' expected at %s:%d\n", token.fl, token.ln);
        return parserInfo;
    }
    token = GetNextToken();
    if(strcmp(token.lx,"(")==0)
        ;
    else{
        parserInfo.er=openParenExpected;
        parserInfo.tk=token;
        printf("Error: '(' expected at %s:%d\n", token.fl, token.ln);
        return parserInfo;
    }
    expression();
    token = GetNextToken();
    if(strcmp(token.lx,")")==0)
        ;
    else{
        parserInfo.er=closeParenExpected;
        parserInfo.tk=token;
        printf("Error: ')' expected at %s:%d\n", token.fl, token.ln);
        return parserInfo;
    }

    token = GetNextToken();
    if(strcmp(token.lx,"{")==0)
        ;
    else{
        parserInfo.er=openBraceExpected;
        parserInfo.tk=token;
        printf("Error: '{' expected at %s:%d\n", token.fl, token.ln);
        return parserInfo;
    }

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
    else{
        parserInfo.er=closeBraceExpected;
        parserInfo.tk=token;
        printf("Error: '}' expected at %s:%d\n", token.fl, token.ln);
        return parserInfo;
    }
    next_token = PeekNextToken();
    if(strcmp(next_token.lx,"else")==0){
        token = GetNextToken();
        token = GetNextToken();
        if(strcmp(token.lx,"{")==0)
            ;
        else{
            parserInfo.er=openBraceExpected;
            parserInfo.tk=token;
            printf("Error: '{' expected at %s:%d\n", token.fl, token.ln);
            return parserInfo;
        }

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
        else{
            parserInfo.er=closeBraceExpected;
            parserInfo.tk=token;
            printf("Error: '}' expected at %s:%d\n", token.fl, token.ln);
            return parserInfo;
        }
    }
    parserInfo.tk=token;
    parserInfo.er=none;
    return parserInfo;
}


ParserInfo whileStatement(){
    ParserInfo parserInfo;
    Token token = GetNextToken();
    if(strcmp(token.lx,"while")==0)
        ;
    else{
        parserInfo.er=syntaxError;
        parserInfo.tk=token;
        printf("Error: 'while' expected at %s:%d\n", token.fl, token.ln);
        return parserInfo;
    }
    token = GetNextToken();
    if(strcmp(token.lx,"(")==0)
        ;
    else{
        parserInfo.er=openParenExpected;
        parserInfo.tk=token;
        printf("Error: '(' expected at %s:%d\n", token.fl, token.ln);
        return parserInfo;
    }

    expression();
    token = GetNextToken();
    if(strcmp(token.lx,")")==0)
        ;
    else{
        parserInfo.er=closeParenExpected;
        parserInfo.tk=token;
        printf("Error: ')' expected at %s:%d\n", token.fl, token.ln);
        return parserInfo;
    }

    token = GetNextToken();
    if(strcmp(token.lx,"{")==0)
        ;
    else{
        parserInfo.er=openBraceExpected;
        parserInfo.tk=token;
        printf("Error: '{' expected at %s:%d\n", token.fl, token.ln);
        return parserInfo;
    }

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
    else{
        parserInfo.er=closeBraceExpected;
        parserInfo.tk=token;
        printf("Error: '}' expected at %s:%d\n", token.fl, token.ln);
        return parserInfo;
    }

    parserInfo.tk=token;
    parserInfo.er=none;
    return parserInfo;}



ParserInfo returnStatement(){
    ParserInfo parserInfo;
    Token token = GetNextToken();
    if(strcmp(token.lx,"return")==0)
        ;
    else{
        parserInfo.er=syntaxError;
        parserInfo.tk=token;
        printf("Error: 'return' expected at %s:%d\n", token.fl, token.ln);
        return parserInfo;
    }
    token = GetNextToken();
    if(strcmp(token.lx,"-")==0 || strcmp(token.lx,"~")==0 || token.tp==INT || token.tp==ID)
        expression();

    token = GetNextToken();
    if(strcmp(token.lx,";")==0)
        ;
    else{
        parserInfo.er=semicolonExpected;
        parserInfo.tk=token;
        printf("Error: ';' expected at %s:%d\n", token.fl, token.ln);
        return parserInfo;
    }

    parserInfo.tk=token;
    parserInfo.er=none;
    return parserInfo;
}

ParserInfo subroutineCall(){
    ParserInfo parserInfo;
    Token token = GetNextToken();
    if(token.tp==ID)
        ;
    else{
        parserInfo.er=idExpected;
        parserInfo.tk=token;
        printf("Error: identifier expected at %s:%d\n", token.fl, token.ln);
        return parserInfo;
    }
    Token next_token = PeekNextToken();
    if(strcmp(next_token.lx,".")==0){
        token = GetNextToken();
        token = GetNextToken();
        if(token.tp==ID)
            ;
        else{
            parserInfo.er=idExpected;
            parserInfo.tk=token;
            printf("Error: identifier expected at %s:%d\n", token.fl, token.ln);
            return parserInfo;
        }
    }

    token = GetNextToken();

    if(strcmp(token.lx,"(")==0)
        ;
    else{
        parserInfo.er=openParenExpected;
        parserInfo.tk=token;
        printf("Error: '(' expected at %s:%d\n", token.fl, token.ln);
        return parserInfo;
    }
    expressionList();

    token = GetNextToken();

    if(strcmp(token.lx,")")==0)
        ;
    else{
        parserInfo.er=closeParenExpected;
        parserInfo.tk=token;
        printf("Error: ')' expected at %s:%d\n", token.fl, token.ln);
        return parserInfo;
    }
    parserInfo.tk=token;
    parserInfo.er=none;
    return parserInfo;
}

ParserInfo doStatement(){
    ParserInfo parserInfo;
    Token token = GetNextToken();
    parserInfo.tk=token;
    if(strcmp(token.lx,"do")==0)
        ;
    else{
        parserInfo.er=syntaxError;
        parserInfo.tk=token;
        printf("Error: 'do' expected at %s:%d\n", token.fl, token.ln);
        return parserInfo;
    }
    subroutineCall();
    parserInfo.tk=token;
    parserInfo.er=none;
    return parserInfo;

}



ParserInfo subroutineBody(){
    ParserInfo parserInfo;
    Token token = GetNextToken();
    if(strcmp(token.lx,"{")==0)
        ;
    else
    {
        parserInfo.er=closeBracketExpected;
        parserInfo.tk=token;
        printf("Error: '{' expected at %s:%d\n", token.fl, token.ln);
        return parserInfo;
    }
    Token next_token = PeekNextToken();
    while(next_token.tp!=EOFile || strcmp(token.lx,"}")==0)
        statement();

    token = GetNextToken();
    if(strcmp(token.lx,"}")==0)
        ;
    else{
        parserInfo.er=closeBracketExpected;
        parserInfo.tk=token;
        printf("Error: '}' expected at %s:%d\n", token.fl, token.ln);
        return parserInfo;
    }
    parserInfo.tk=token;
    parserInfo.er=none;
    return parserInfo;
}



ParserInfo subroutineDeclar(){// (constructor | function | method) (type|void) identifier (paramList) subroutineBody
    ParserInfo parserInfo;
    Token token = GetNextToken();
//    if(strcmp(token.lx,"constructor")==0 || strcmp(token.lx,"function")==0 || strcmp(token.lx,"method")==0)
//        ;
//    else{
//        parserInfo.er=subroutineDeclarErr;
//        parserInfo.tk=token;
//        printf("Error: class member declaration must begin with static, field, constructor , function , or method expected at %s:%d\n", token.fl, token.ln);
//        return parserInfo;
//    }
//    token = GetNextToken();
    if(strcmp(token.lx,"void")==0 || strcmp(token.lx,"int")==0 ||  strcmp(token.lx,"char")==0||  strcmp(token.lx,"boolean")==0 ||  token.tp==ID)
        ;
    else{
        parserInfo.er=illegalType;
        parserInfo.tk=token;
        printf("Error: a type must be int, char, boolean, or identifier expected at %s:%d\n", token.fl, token.ln);
        return parserInfo;
    }


    token = GetNextToken();
    if(token.tp==ID)
        ;
    else{
        parserInfo.er=idExpected;
        parserInfo.tk=token;
        printf("Error: identifier expected at %s:%d\n", token.fl, token.ln);
        return parserInfo;
    }


    token = GetNextToken();
    if(strcmp(token.lx,"(")==0)
        ;
    else{
        parserInfo.er=openParenExpected;
        parserInfo.tk=token;
        printf("Error: '(' expected at %s:%d\n", token.fl, token.ln);
        return parserInfo;
    }
    paramList();

    token = GetNextToken();
    if(strcmp(token.lx,")")==0)
        ;
    else{
        parserInfo.er=closeParenExpected;
        parserInfo.tk=token;
        printf("Error: ')' expected at %s:%d\n", token.fl, token.ln);
        return parserInfo;
    }

    subroutineBody();
    parserInfo.tk=token;
    parserInfo.er=none;
    return parserInfo;

}


ParserInfo memberDeclar(){
    ParserInfo parserInfo;
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
                    else{
                        parserInfo.er=idExpected;
                        parserInfo.tk=token;
                        printf("Error: identifier expected at %s:%d\n", token.fl, token.ln);
                        return parserInfo;
                    }

                    next_token = PeekNextToken();

                }
            }else if(strcmp(token.lx, ";") == 0)
                ;
            else{
                parserInfo.er=semicolonExpected;
                parserInfo.tk=token;
                printf("Error: ';' expected at %s:%d\n", token.fl, token.ln);
                return parserInfo;
            }
        }else{
            parserInfo.er=idExpected;
            parserInfo.tk=token;
            printf("Error: identifier expected at %s:%d\n", token.fl, token.ln);
            return parserInfo;
        }


    }else if(strcmp(token.lx,"constructor")==0 || strcmp(token.lx,"function")==0 || strcmp(token.lx,"method")==0)
        subroutineDeclar();
    else{
        parserInfo.er=classVarErr;
        parserInfo.tk=token;
        printf("Error: class member declaration must begin with static, field, constructor , function , or method at %s:%d\n", token.fl, token.ln);
        return parserInfo;
    }

    parserInfo.tk=token;
    parserInfo.er=none;
    return parserInfo;
}

ParserInfo classDeclar(){
    ParserInfo parserInfo;
    Token token = GetNextToken();

    if(strcmp(token.lx,"class")==0)
        ;
    else {
        parserInfo.er=classExpected;
        parserInfo.tk=token;
        printf("Error: 'class' expected at %s:%d\n", token.fl, token.ln);
        return parserInfo;
    }

    token = GetNextToken();
    if(token.tp==ID)
        ;
    else{
        parserInfo.er=idExpected;
        parserInfo.tk=token;
        printf("Error: identifier expected at %s:%d\n",token.fl,token.ln);
        return parserInfo;
    }

    token = GetNextToken();
    if(strcmp(token.lx,"{")==0)
        ;
    else{
        parserInfo.er=openBraceExpected;
        parserInfo.tk=token;
        printf("Error: '{' expected at %s:%d\n",token.fl,token.ln);
        return parserInfo;
    }

    Token next_token = PeekNextToken (); // notice the peek
    while (strcmp(next_token.lx,"}")!=0) // notice the while
    {
        memberDeclar();
        next_token = PeekNextToken ();
    }

    token = GetNextToken();
    if(strcmp(token.lx,"}")==0)
        ;
    else{
        parserInfo.er=closeBraceExpected;
        parserInfo.tk=token;
        printf("Error: '}' expected at %s:%d\n",token.fl,token.ln);
        return parserInfo;
    }

    parserInfo.tk=token;
    parserInfo.er=none;
    return parserInfo;
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

	pi=classDeclar();
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
    InitParser("SquareGame.jack");
    Parse();
    StopParser();
	return 1;
}
#endif
