/* les tokens ici sont ceux supposes etre renvoyes par l'analyseur lexical
 * A adapter par chacun en fonction de ce qu'il a ecrit dans tp.l
 *
 * Bison ecrase le contenu de tp_y.h a partir de la description de la ligne
 * suivante. Ce sont donc ces lignes qu'il faut adapter si besoin, pas tp_y.h !
 */
 
%token CLASS IS EXTENDS STATIC OVERRIDE RETURNS DEF THIS SUPER RESULT AS NEW
%token AFF
%token IF THEN ELSE
%token ID TYPE VAR CST
%token RELOP
%token UNARY
 
 
/*%token BEG END IF THEN ELSE GET PUT
%token AFF
%token ID STR
%token CST
*/



/* indications de precedence (en ordre croissant) et d'associativite. Les
 * operateurs sur une meme ligne (separes par un espace) ont la meme priorite.
 * On peut mettre soit un token, soit directement un caractere comme '*'
 */
%nonassoc RELOP
%left ELSE
%left AND
%left ADD SUB
%left MUL DIV
%left '.'
%left UNARY


 /* On declare que la valeur associe a NOM_VAR utilise la variante 'S' du
  * type YYSTYPE. Il s'agira donc d'une chaine de caracteres...
  * Ajouter les indications similaires pour les autres nom-terminaux qui ont
  * aussi une notion de valeur associee.
  */
/*%type <S> NOM_VAR
%type <S> paramStrPut
%type <T> expr
%type <C> relop
*/
%{
#include "tp.h"     /* les definition des types et les etiquettes des noeuds */

extern int yylex();	/* fournie par Flex */
extern void yyerror();  /* definie dans tp.c */
%}

%%

/*  class Nom (param, ...) [extends nom (args, ...)] [bloc]  is  { decl,  ... } */
/* I/ */
declarationClasse : CLASS ID '(' paramsList ')' inherits blocs IS '{' declList '}'
;

paramsList :
| paramsMultiples
;

paramsMultiples : paramName
| paramName ',' paramsMultiples
; 

paramName : var ':' type
;

var : ID
;

type : ID
;

inherits :
| EXTENDS TYPE '(' argumentsList ')'
;

argumentsList :
| expression ',' argumentsList
| expression
;

blocs :
| '{' blocInstructions '}'
;

blocInstructions :
| listInstructions
| listDeclarationVariables IS listInstructions
;

declList :
| declChamp declList
| declMethod declList
;


/* [static] var nom : type [:= expression]; */
/* II/ */
declChamp : isStatic VAR var ':' type exprInitVar ';'
;

isStatic :
| STATIC
;

exprInitVar :
| AFF expression
;


/* [override | static] def nom (params, ...) [returns type] is bloc */
/* III/ */
declMethod : isStaticOrOverride DEF ID '(' paramsList ')' isReturn IS '{' blocInstructions '}'
;

isStaticOrOverride :
| STATIC
| OVERRIDE
;

isReturn : 
| RETURNS type
;

/* IV/ */
/* Sans ";", qui est défini ailleurs */
/* expression ne rajoute JAMAIS de { }  */
expression : selection
| CST
| '(' expression ')'
| '(' AS type ':' expression ')'
| instenciation
| envoiMsg
| exprWithOperator
;


/* -3.toString() */

selection : expression '.' var
;

instenciation : NEW type '(' argumentsList ')'
;

envoiMsg : expression '.' '(' listAttributs ')'
;	

listAttributs :
| expression
| expression',' listAttributs
;

exprWithOperator : var
| expression ADD expression
| expression SUB expression
| expression MUL expression
| expression DIV expression
| expression RELOP expression
| SUB expression %prec UNARY
| ADD expression %prec UNARY
;

instructions : expression ';'
| '{' blocInstructions '}'
| RETURNS ';'
| affectation
| ifThenElse
;

listInstructions : instructions
| instructions listInstructions
;

listDeclarationVariables : VAR var ':' type exprInitVar ';'
;

affectation : selection AFF expression ';'
| ID AFF expression ';'
;

ifThenElse : IF expression THEN instructions ELSE instructions
;




/*
expr :
| expr ADD expr			{$$=makeTree(ADD,2,$1,$3);}
| expr SUB expr			{$$=makeTree(SUB,2,$1,$3);}
| expr MUL expr			{$$=makeTree(MUL,2,$1,$3);}
| expr DIV expr			{$$=makeTree(DIV,2,$1,$3);}
| CST					{$$=makeLeafInt(CST, yylval.I);}
| GET'('')'				{$$=makeLeafInt(GET, getValue());}
| PUT'(' paramStrPut ',' expr ')'	{$$=makeLeafStr(PUT, putValue($3, $5) );}
| NOM_VAR				{$$=makeLeafStr(CST, yylval.S);}
| '-' expr %prec UNARY		{$$=makeTree(UMIN,1,$2);}
| IF expr THEN expr ELSE expr	{$$=makeTree(IF,3,$2,$4,$6);}
| expr relop expr			{$$=makeTree($2 ,2,$1,$3);}
;
*/

/*decl : ID AFF expr ';'
;
*/
/* construire et renvoyer un arbre representant l'expression, en fonction des
 * arbres (deja construits) de ses sous-expressions.
 *
 * Voir les fonctions mises a disposition dans tp.c et en ajouter si besoin.
 */

/*relop : RELOP	{ $$=yyval.C; }
;

paramStrPut : STR { $$=yyval.S; }
;
*/
/* Passage de valeur entre analyseur lexical et analyseur syntaxique via la
 * variable yylval (de type YYSTYPE)
 */
/*NOM_VAR : ID { $$ = yylval.S; }
;*/
