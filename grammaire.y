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
%token ID VAR CST MET
%token RELOP
%token UMIN UPLUS
%token TREE

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
  *
*/

/* %type <S> NOM_VAR */
%type <D> paramsList
%type <D> paramsMultiples
%type <D> paramName
%type <I> isStatic
/*%type <T> exprInitVar*/
/* %type <T> instenciation */
/* %type <T> selection */
%type <M> inherits
/* %type <T> expression */
/* %type <T> exprWithOperator */
/* %type <Inst> instruction */
/* %type <Inst> listInstruction */
%type <S> isReturn
%type <I> isStaticOrOverride
%type <S> type
%type <S> var
%type <S> name

%{
#include "arbre.h"     /* les definition des types et les etiquettes des noeuds */

extern int yylex();	/* fournie par Flex */
extern void yyerror();  /* definie dans arbre.c */
%}

%%

/* class Nom (param, ...) [extends nom (args, ...)] [bloc]  is  { decl,  ... } */
/* I/ */
declarationClasse : CLASS name '(' paramsList ')' inherits blocs IS '{' declList '}'		 /*void*/ { addClass($2, $4, $6); }
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
| EXTENDS name '(' argumentsList ')'  									{ $$ = addMere($2 /*,$4*/ ); } /*Ajout des arguments d'initialisation à faire*/
;

argumentsList : 														/*TODO : { $$ = NULL; }*/
| argumentsListAux														/*TODO : { $$ = $1; }*/
;

argumentsListAux : expression ',' argumentsListAux						/*TODO*/
| expression															/*TODO*/
;

/************** DIFFERENCE ???!???
listAttributs :
| listAttributAux
;

listAttributsAux : expression
| expression',' listAttributs
;
*************/


blocs :																	/*TODO : { $$ = NULL; } */
| '{' blocInstructions '}'												/*TODO : { $$ = $2; } */
;

blocInstructions :														/*TODO : { $$ = NULL; } */
| listInstructions														/*TODO : { $$ = $1; } */
| listDeclarationVariables IS listInstructions							/*TODO : { $$ = $3; } */
;

declList : declChamp 													/*void*/
| declMethod															
| declChamp declList													
| declMethod declList													
;


/* [static] var nom : type [:= expression]; */
/* II/ */
declChamp : isStatic VAR var ':' type exprInitVar ';'   				{ addChamp($1,$3,$5,NULL/*$6*/)} /*void*/
;

isStatic :																{$$ = 0}
| STATIC   																{$$ = 1}
;

type : ID																{ $$ =  yyval.S; }
;

exprInitVar :  															/*TODO : {$$ = NULL} */
| AFF expression   														/*TODO : {$$ = $2} */
;


/* [override | static] def nom (params, ...) [returns type] is bloc */
/* III/ */
declMethod : isStaticOrOverride DEF var '(' paramsList ')' isReturn IS '{' blocInstructions '}'		{ createMethodFrom( $3 ,$5, $7 , NULL /*$10*/ );  }
;

isStaticOrOverride : isStatic  											{ $$ = $1; }
| OVERRIDE																{ $$ = 2; }
;

isReturn : 																{ $$ = "VOID_RETURN"; }
| RETURNS type															{ $$ = $2; }
;

/* IV/ */
/* Sans ";", qui est défini ailleurs */
/* expression ne rajoute JAMAIS de { }  */
expression : selection 													/* { $$ = $1; } */
| CST																	/* { $$ = makeLeafInt(CST, yyval.I); } */
| '(' expression ')'													/* { $$ = $$=makeTree(TREE,1,$2);*/
| instenciation															/*TODO*/
| envoiMsg																/*TODO*/
| exprWithOperator														/* { $$ = $1;} */
/* | '(' AS type ':' expression ')'		//correspond au cast d'une expression, non aborde\\ */	
;

selection : expression '.' var											/* TODO : { $$ = makeLeafVar($1,$3); } */
;

instenciation : NEW type '(' argumentsList ')'							/*TODO : { $$ = */					
;

envoiMsg : expression '.' ID '(' listAttributs ')'						/* TODO : { $$ = makeLeafMet($1,$3 //,$4//); } */
;

listAttributs :															/* difference avec argumentList ?!? */
| listAttributsAux
;

listAttributsAux : expression											/*TODO*/
| expression',' listAttributs											/*TODO*/
;

exprWithOperator : var													/*TODO : { $$=makeLeafStr(ID, $1); } */
| expression ADD expression  	 									    /*TODO : { $$=makeTree(ADD,2,$1,$3); }  */
| expression SUB expression												/*TODO : { $$=makeTree(SUB,2,$1,$3); }  */
| expression MUL expression  											/*TODO : { $$=makeTree(MUL,2,$1,$3); }  */
| expression DIV expression  											/*TODO : { $$=makeTree(DIV,2,$1,$3); }  */
| expression RELOP expression  											/*TODO : { $$=makeTree(RELOP,2,$1,$3); }  */
| SUB expression %prec UMIN  											/*TODO : { $$=makeTree(UMIN,1,$2); }  */
| ADD expression %prec UPLUS											/*TODO : { $$=makeTree(UPLUS,1,$2); }  */
;

instructions : expression ';' 											/*TODO { $$ = $1; } */
| '{' blocInstructions '}'												/*TODO { $$ = $2; } */
| RETURNS ';'															/*TODO*/
| affectation															/*TODO*/
| ifThenElse															/*TODO*/
;

listInstructions : instructions											/*TODO : { $$ = $1; } */
| instructions listInstructions											/*TODO : { $$ = concatInstruction($1, $2); } */
;

listDeclarationVariables : VAR var ':' type exprInitVar ';'				/*TODO*/
;

affectation : selection AFF expression ';'								/*TODO*/
| ID AFF expression ';'													/*TODO*/
;

ifThenElse : IF expression THEN instructions ELSE instructions			/*TODO*/
;
