Digit [0-9]
Letter  [a-zA-Z]
LD ({Letter}|{Digit})
Delim [ \n\t\r]
Accolade [{}]
Parentheses [()]
Symbol [;,]
Star \*
NotStar [^*]
NotStarSlash [^*/]
StrChar [^\\"\n]
StrEsc \\.


%%
{Delim}+        { yylineno+=(strtol(yytext, NULL, 0)=='\n')?1:0;/* espaces : rien ! on passe au token suivant */ }

{Letter}{LD}*   { return keywordOrId(); }
\"({StrChar}|{StrEsc})*\" { yylval.S = strdup(yytext); return(STR); }
"/*"({NotStar}|{Star}+{NotStarSlash})*{Star}*"*/" { }

{Digit}+        { yylval.I = strtol(yytext, NULL, 0); return(CST); }

"/"		{ return(DIV); }
"*"		{ return(MUL); }
"+"		{ return(ADD); }
"-"		{ return(SUB); }

"&"		{ return(AND); }
"|"		{ return(OR); }

"&&"		{ return(AAND); }
"||"		{ return(OOR); }

"<>"		{ return(DIF); }
">"		{ return(SUP); }
"<"		{ return(INF); }
"<="		{ return(IEQ); }
">="		{ return(SEQ); }

":"		{ return(DEF); }
":="		{ return(EQ); }
"::"		{ return(NS); }
"."		{ return(POINT); }

{Accolade}	{ return(yytext[0]); }
{Parentheses}	{ return(yytext[0]); }
{Symbol}        { return(yytext[0]); }
%%
