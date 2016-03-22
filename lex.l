%{
#include <stdio.h>
#include "yac.tab.h"

#define YY_DECL int yylex()
%}

comment		"//"[^\n]*
delim		[ \t\n]
ws		{delim}+
letter		[A-Za-z_]
string		\"[^\"]*\"
unstring	\"[^\"]*
digit		[0-9]
var		{letter}({letter}|{digit})*

%%

[\t]
[0-9]+\.[0-9]+		{ yylval.fval = atol(yytext); return FLOAT; }
[0-9]+			{ yylval.ival = atoi(yytext); return INT  ; }
[\n]			{ return NL; 		}
"+"			{ return PLUS;		}
"-"			{ return MINUS;		}
"*"			{ return MULT;		}
"^"			{ return HAT;		}
"/"			{ return DIV;		}
"("			{ return LEFT;		}
")"			{ return RIGHT;		}
"="			{ return EQUATE;	}
"<"			{ return LT;		}
">"			{ return GT;		}
"IF"			{ return IF;		}
"THEN"			{ return THEN;		}
"TO"			{ return TO;		}
"FOR"			{ return FOR;		}
"LET"			{ return LET;		}
"NEXT"			{ return NEXT;		}
"STEP"			{ return STEP;		}
"READ"			{ return READ;		}
"END"			{ return END;		}
"GOTO"			{ return GOTO;		}
"PRINT"			{ yylval.str = yytext; return PRINT; 	}
{string}		{ yylval.str = yytext; return STRING;	}
{var}			{ yylval.str = yytext; return ID;	}
{comment}		{ }
%%
