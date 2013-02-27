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

%{
#include "projet.h"
#include "projet_y.h"
#include <string.h>	/* pour strdup */

extern char *strdup(const char *s);
extern void setError(int code);

extern YYSTYPE yylval;
extern int errorCode;
extern bool noEval;

/* Il est plus economique de ne pas creer une expression reguliere pour chaque
 * mot-clef mais plutot de les reconnaitre dans un premier temps comme des
 * identificateurs et de faire la distinction par un bout de code
 */
int keywordOrId() {
  if (! strcmp(yytext, "is"))		return(IS);
  if (! strcmp(yytext, "as"))		return(AS);
  if (! strcmp(yytext, "if"))		return(IF);
  if (! strcmp(yytext, "then"))		return(THEN);
  if (! strcmp(yytext, "else"))		return(ELSE);
  if (! strcmp(yytext, "new"))		return(NEW);
  if (! strcmp(yytext, "this"))		return(THIS);
  if (! strcmp(yytext, "super"))	return(SUPER);
  if (! strcmp(yytext, "class"))	return(CLASS);
  if (! strcmp(yytext, "extends"))	return(EXTENDS);
  if (! strcmp(yytext, "override"))	return(OVERRIDE);
  if (! strcmp(yytext, "static"))	return(STATIC);
  if (! strcmp(yytext, "var"))		return(VAR);
  if (! strcmp(yytext, "def"))		return(DEF);
  if (! strcmp(yytext, "result"))	return(RESULT);
  if (! strcmp(yytext, "return"))	return(RETURN);
  if (! strcmp(yytext, "returns"))	return(RETURNS);
  /* si ce n'est pas un mot-clef, sauvegarder le texte de l'identificateur
   * et renvoyer le token associe aux identificateurs
   */
  yylval.S = strdup(yytext);
  return(ID);
}


void lexError(char c) {
  fprintf(stderr, "Error: Skipping invalid character '%c' on line %d\n",
          c, yylineno);
  setError(LEXICAL_ERROR);
}

%}

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

"="             { yylval.C = EQ; return(RELOP); }
"<>"            { yylval.C = NE; return(RELOP); }
">"             { yylval.C = GT; return(RELOP); }
">="            { yylval.C = GE; return(RELOP); }
"<"             { yylval.C = LT; return(RELOP); }
"<="            { yylval.C = LE; return(RELOP); }

":"		{ return(DEF); }
":="		{ return(AFF); }
"::"		{ return(NS); }
"."		{ return(POINT); }


{Accolade}	{ return(yytext[0]); }
{Parentheses}	{ return(yytext[0]); }
{Symbol}        { return(yytext[0]); }
%%
