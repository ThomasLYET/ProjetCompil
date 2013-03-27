/* les tokens ici sont ceux supposes etre renvoyes par l'analyseur lexical
 * A adapter par chacun en fonction de ce qu'il a ecrit dans tp.l
 *
 * Bison ecrase le contenu de tp_y.h a partir de la description de la ligne
 * suivante. Ce sont donc ces lignes qu'il faut adapter si besoin, pas tp_y.h !
 */
 
%token CLASS IS EXTENDS STATIC OVERRIDE RETURNS DEF THIS SUPER RESULT AS NEW
%token AFF
%token ADD SUB MUL DIV
%token IF THEN ELSE
%token ID VAR CST
%token RELOP
%token UNARY

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
/*
%type <S> NOM_VAR
%type <D> paramsList
%type <D> paramsMultiples
%type <D> paramName
%type <S> paramStrPu
<<<<<<< HEAD
%type <I> isStatic
%type <T> exprInitVar
=======
%type <M> inherits
>>>>>>> 83d41e4cc4716da08ea96df715eb33f940a41878
*/
/* %type<T> expression // TODO : A REMETTRE après avoir bien défini expression !!! \\ */
/* %type<T> exprWithOperator // IDEM \\ */

%{
#include "arbre.h"     /* les definition des types et les etiquettes des noeuds */

extern int yylex();	/* fournie par Flex */
extern void yyerror();  /* definie dans arbre.c */
%}

%%

/* class Nom (param, ...) [extends nom (args, ...)] [bloc]  is  { decl,  ... } */
/* I/ */
declarationClasse : CLASS name '(' paramsList ')' inherits blocs IS '{' declList '}'		 { addClass($2, $4, $5); }
;

paramsList :															{ $$ = NULL; }
| paramsMultiples 														{ $$ = $1; }
;


paramsMultiples : paramName												{ $$ = $1; }
| paramName ',' paramsMultiples											{ $$ = concatVarDecl( $1, $3); }
; 

paramName : var ':' name												{ $$ = newVarDecl( $1, $3);}
;

var : ID																{ $$ = yyval.S; }
;

name : ID																{ $$ = yyval.S; }
;

inherits :																{ $$ = NULL; }
| EXTENDS name '(' argumentsList ')'  									{ $$ = addMere($2); }
;

argumentsList : 														/*Faut-ils utiliser des arbres ?? */
| argumentsListAux
;

argumentsListAux : expression ',' argumentsListAux						

| expression
;

blocs :																	{ $$ = NULL; }
| '{' blocInstructions '}'												{ $$ = $2; }
;

blocInstructions :														{ $$ = NULL; }
| listInstructions														/*TODO*/
| listDeclarationVariables IS listInstructions							/*TODO*/
;

declList :																/*TODO*/
| declChamp declList													/*TODO*/
| declMethod declList													/*TODO*/
;


/* [static] var nom : type [:= expression]; */
/* II/ */
declChamp : isStatic VAR var ':' type exprInitVar ';'   { addChamp($1,$3,$5,$6)}      /*|||||||||||*/
;

<<<<<<< HEAD
isStatic : {$$ = 0}
| STATIC   {$$ = 1}
=======
isStatic :
| STATIC 	{  }
>>>>>>> 83d41e4cc4716da08ea96df715eb33f940a41878
;

exprInitVar :  {$$ = NULL}
| AFF expression   {$$ = $2}
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
| RETURNS name
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

selection : expression '.' var
;

instenciation : NEW type '(' argumentsList ')'
;

envoiMsg : expression '.' ID '(' listAttributs ')'
;

listAttributs :
| expression
| expression',' listAttributs
;

/* Ce qui en commentaire à remettre après */
exprWithOperator : var				/* { $$=makeLeaf(ADD, 3); /* Attention valeur BIDON */ /* } */
| expression ADD expression  		/* { $$=makeTree(ADD,2,$1,$3); } */
| expression SUB expression			/* { $$=makeTree(SUB,2,$1,$3); } */
| expression MUL expression  		/* { $$=makeTree(MUL,2,$1,$3); } */
| expression DIV expression  		/* { $$=makeTree(DIV,2,$1,$3); } */
| expression RELOP expression  		/* { $$=makeTree($2,2,$1,$3); } */
| SUB expression %prec UNARY  		/* { $$=makeTree(UMIN,1,$2); } */
| ADD expression %prec UNARY		/* { $$=makeTree(UPLUS,1,$2); } */
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
