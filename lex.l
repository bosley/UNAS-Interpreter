/*
The MIT License (MIT)

Copyright (c) 2016 Josh Bosley

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
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
