%{

#include <stdlib.h>
#include <iostream>

// my include
#include<string>
#include "framework/NUBasic.hpp"

extern int yylex();
extern int yyparse();
extern FILE* yyin;

void yyerror(const char *);

// My class 
NUBasic nub;

%}

%union 
{
	int ival;
	float fval;
	char *str;
}

%type<ival> 	expr
%type<fval> 	mexpr
%type<str>	vexpr
%type<str>	sexpr
%type<str>	pexpr

%token<str> 	ID
%token<str> 	TO
%token<str> 	IF
%token<str> 	LET
%token<str> 	FOR
%token<str> 	THEN
%token<str> 	NEXT
%token<str> 	STEP
%token<str> 	GOTO
%token<str> 	READ
%token<str> 	PRINT
%token<str> 	STRING
%token<ival> 	INT
%token<fval>	FLOAT
%token		PLUS MINUS MULT DIV HAT
%token		LEFT RIGHT NL END EQUATE LT GT

%left 		PLUS MINUS
%left 		MULT DIV

%start input

%%

input	:
		| input line
		;

line	: NL				{}
		| mexpr NL		{}
		| expr NL		{}
		| vexpr NL		{}
		| pexpr NL		{}
		| GOTO expr NL		{ nub.doGoto($2); }
		| END NL		{ nub.doEnd(); }
		;

expr	: INT					{$$ = $1;}
		| expr PLUS expr		{$$ = $1 + $3;}
		| expr MINUS expr		{$$ = $1 - $3;}
		| expr MULT expr		{$$ = $1 * $3;}
		| expr HAT expr			{$$ = $1 * $3;}
		| expr DIV expr			{$$ = $1 / $3;}
		| LEFT expr RIGHT		{$$ = $2;}
		;

mexpr	: FLOAT					{$$ = $1;}
		| mexpr PLUS mexpr		{$$ = $1 + $3;}
		| mexpr MINUS mexpr		{$$ = $1 - $3;}
		| mexpr MULT mexpr		{$$ = $1 * $3;}
		| mexpr HAT mexpr		{$$ = $1 * $3;}
		| mexpr DIV mexpr		{$$ = $1 / $3;}
		| LEFT mexpr RIGHT		{$$ = $2     ;}
		| expr PLUS mexpr		{$$ = $1 + $3;}
		| expr MINUS mexpr		{$$ = $1 - $3;}
		| expr HAT mexpr		{$$ = $1 - $3;}
		| expr MULT mexpr		{$$ = $1 * $3;}
		| expr DIV mexpr		{$$ = $1 / $3;}
		| mexpr PLUS expr		{$$ = $1 + $3;}
		| mexpr MINUS expr		{$$ = $1 - $3;}
		| mexpr HAT expr		{$$ = $1 - $3;}
		| mexpr MULT expr		{$$ = $1 * $3;}
		| mexpr DIV expr		{$$ = $1 / $3;}
		;

sexpr	: STRING				{$$ = $1;}
		| ID ' ' ID			{$$ = $1;}
		| ID ' ' sexpr			{$$ = $1;}
		| ID PLUS sexpr			{$$ = $1;}
		| ID HAT sexpr			{$$ = $1;}
		| ID MINUS sexpr		{$$ = $1;}
		| ID MULT sexpr			{$$ = $1;}
		| ID DIV sexpr			{$$ = $1;}
		| ID PLUS expr			{$$ = $1;}
		| ID MINUS expr			{$$ = $1;}
		| ID HAT expr			{$$ = $1;}
		| ID MULT expr			{$$ = $1;}
		| ID DIV expr			{$$ = $1;}
		| ID PLUS mexpr			{$$ = $1;}
		| ID MINUS mexpr		{$$ = $1;}
		| ID HAT mexpr			{$$ = $1;}
		| ID MULT mexpr			{$$ = $1;}
		| ID DIV mexpr			{$$ = $1;}
		| ID PLUS ID			{$$ = $1;}
		| ID MINUS ID			{$$ = $1;}
		| ID MULT ID			{$$ = $1;}
		| ID HAT ID			{$$ = $1;}
		| ID DIV ID			{$$ = $1;}
		| sexpr PLUS ID			{$$ = $3;}
		| sexpr MINUS ID		{$$ = $1;}
		| sexpr MULT ID			{$$ = $1;}
		| sexpr HAT ID			{$$ = $1;}
		| sexpr DIV ID			{$$ = $1;}
		| sexpr PLUS sexpr		{$$ = $3;}
		| sexpr MINUS sexpr		{$$ = $1;}
		| sexpr MULT sexpr		{$$ = $1;}
		| sexpr HAT sexpr		{$$ = $1;}
		| sexpr DIV sexpr		{$$ = $1;}
		| expr PLUS ID			{$$ = $3;}
		| expr MINUS ID			{$$ = $3;}
		| expr MULT ID			{$$ = $3;}
		| expr DIV ID			{$$ = $3;}
		| mexpr PLUS ID			{$$ = $3;}
		| mexpr MINUS ID		{$$ = $3;}
		| mexpr MULT ID			{$$ = $3;}
		| mexpr DIV ID			{$$ = $3;}
		| LEFT ID RIGHT			{$$ = $2;}
		| LEFT ID PLUS ID RIGHT		{$$ = $2;}
		;

vexpr	: ID						{$$ = $1;}
		| LET ID EQUATE sexpr			{ nub.assignVariable($2); }
		| LET ID EQUATE mexpr			{ nub.assignVariable($2); }
		| LET ID EQUATE expr			{ nub.assignVariable($2); }
		| LET ID EQUATE ID			{ nub.assignVariable($2); }
		| FOR ID EQUATE expr TO expr		{ nub.doForLoop($2, 0); }
		| FOR ID EQUATE expr TO expr STEP expr	{ nub.doForLoop($2, 1); }
		| NEXT ID				{ nub.doFLNext($2); }

		| IF expr LT ID THEN expr		{ nub.doIf(ITOS($2), "<", $4, $6); }
		| IF expr GT ID THEN expr		{ nub.doIf(ITOS($2), ">", $4, $6); }
		| IF expr LT expr THEN expr		{ nub.doIf(ITOS($2), "<", ITOS($4), $6); }
		| IF expr GT expr THEN expr		{ nub.doIf(ITOS($2), ">", ITOS($4), $6); }
		| IF ID LT expr THEN expr		{ nub.doIf($2, "<", ITOS($4), $6); }
		| IF ID GT expr THEN expr		{ nub.doIf($2, ">", ITOS($4), $6); }
		| IF ID LT ID THEN expr			{ nub.doIf($2, "<", $4, $6); }
		| IF ID GT ID THEN expr			{ nub.doIf($2, ">", $4, $6); }

		| IF expr LT EQUATE expr THEN expr	{ nub.doIf(ITOS($2), "<=", ITOS($5), $7); }
		| IF expr GT EQUATE expr THEN expr	{ nub.doIf(ITOS($2), ">=", ITOS($5), $7); }
		| IF expr GT EQUATE ID THEN expr	{ nub.doIf(ITOS($2), ">=", $5, $7); }
		| IF expr LT EQUATE ID THEN expr	{ nub.doIf(ITOS($2), "<=", $5, $7); }
		| IF ID LT EQUATE expr THEN expr	{ nub.doIf($2, "<=", ITOS($5), $7); }
		| IF ID GT EQUATE expr THEN expr	{ nub.doIf($2, ">=", ITOS($5), $7); }
		| IF ID GT EQUATE ID THEN expr		{ nub.doIf($2, ">=", $5, $7); }
		| IF ID LT EQUATE ID THEN expr		{ nub.doIf($2, "<=", $5, $7); }

		| IF expr EQUATE EQUATE  expr THEN expr	{ nub.doIf(ITOS($2), "==", ITOS($5), $7); }
		| IF expr EQUATE EQUATE ID THEN expr	{ nub.doIf(ITOS($2), "==", $5, $7); }
		| IF ID EQUATE EQUATE expr THEN expr	{ nub.doIf($2, "==", ITOS($5), $7); }
		| IF ID EQUATE EQUATE ID THEN expr	{ nub.doIf($2, "==", $5, $7); }
		| READ sexpr				{ nub.doRead($2); }
		| READ ID				{ nub.doRead($2); }
		;	

pexpr	: STRING
                | PRINT ID                      { nub.doPrint($2); }

                | PRINT sexpr                   { nub.doPrint($2); }
                | PRINT sexpr PLUS expr         { nub.doPrint($2); }
                | PRINT sexpr PLUS sexpr        { nub.doPrint($2); }
                | PRINT sexpr PLUS mexpr        { nub.doPrint($2); }
                | PRINT sexpr PLUS pexpr        { nub.doPrint($2); }

                | PRINT expr                    { nub.doPrint(ITOS($2)); }
                | PRINT expr PLUS sexpr         { nub.doPrint(ITOS($2)); }
                | PRINT expr PLUS pexpr         { nub.doPrint(ITOS($2)); }

                | PRINT mexpr                   { nub.doPrint(ITOS($2)); }
                | PRINT mexpr PLUS sexpr        { nub.doPrint(ITOS($2)); }
                | PRINT mexpr PLUS pexpr        { nub.doPrint(ITOS($2)); }

                | PRINT pexpr                   { nub.doPrint($2); }
                | PRINT pexpr PLUS ID           { nub.doPrint($2); }
                | PRINT pexpr PLUS expr         { nub.doPrint($2); }
                | PRINT pexpr PLUS sexpr        { nub.doPrint($2); }
                | PRINT pexpr PLUS mexpr        { nub.doPrint($2); }
                | PRINT pexpr PLUS pexpr        { nub.doPrint($2); }

		;
%%

/*
	Entry point
*/

int main(int argc, char **argv)
{
	// Check input
	if(argc < 2)
	{
		std::cout << "Usage: " << argv[0] << " file " <<  std::endl;
		exit(1);
	}

	// Evaluate memory usage	
	nub.doMemoryEvaluation(argv[1]);

	// Set debug on
	if(argc == 3)
	{
		std::string tog = argv[2];
		if(tog == "1")
			nub.toggleDebug();
	}

	// Set yyin to the file
	yyin = fopen(argv[1], "r");

	// Ensure it is a file, and start parsing	
	if(yyin == NULL)
	{
		std::cout << "Error: File [" << argv[1] << "] > NOT FOUND" << std::endl;
		exit(1);	
	}
	do
	{
		yyparse();
	} while(!feof(yyin));
	return 0;
}

void yyerror(const char* c)
{
	std::cout << " yyerror >> " << c << std::endl;
	exit(1);
}
