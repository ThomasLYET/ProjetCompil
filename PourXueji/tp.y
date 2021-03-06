/* les tokens ici sont ceux supposes etre renvoyes par l'analyseur lexical
 * A adapter par chacun en fonction de ce qu'il a ecrit dans tp.l
 *
 * Bison ecrase le contenu de tp_y.h a partir de la description de la ligne
 * suivante. Ce sont donc ces lignes qu'il faut adapter si besoin, pas tp_y.h !
 */
%token BEG END IF THEN ELSE GET PUT
%token AFF
%token ID STR
%token CST
%token AND NOT OR
%token RELOP
%token urg

/* indications de precedence (en ordre croissant) et d'associativite. Les
 * operateurs sur une meme ligne (separes par un espace) ont la meme priorite.
 * On peut mettre soit un token, soit directement un caractere comme '*'
 */
%nonassoc ELSE
%left ADD SUB
%left MUL DIV
%left urg
%left OR
%left AND
%left NOT

 /* On declare que la valeur associe a NOM_VAR utilise la variante 'S' du
  * type YYSTYPE. Il s'agira donc d'une chaine de caracteres...
  * Ajouter les indications similaires pour les autres nom-terminaux qui ont
  * aussi une notion de valeur associee.
  */
%type <S> NOM_VAR
%type <T> expr
%type <T> boolExpr
%type <S> Strr

%{
#include "tp.h"     /* les definition des types et les etiquettes des noeuds */

extern int yylex();	/* fournie par Flex */
extern void yyerror();  /* definie dans tp.c */
%}

%% 
programme : declLO BEG expr END
;



/* l'environnemt variable/valeur etant memorise directement dans une variable
 * globale, les productions qui suivent ne retournent pas de valeur et n'ont
 * pas d'action associee.
 *
 * declL0: liste optionnelle de 'declarations de variable'
 */
declLO :
| declL
;

declL : decl
| declL decl
;


/* une declaration de variable ou de fonction, terminee par un ';'.
 * L'action associee devrait correspondre a
 * - evaluer l'arbre correspondant a 'expr', en utilisant la liste courante
 * des couple variable/valeur. Une fois la valeur obtenue ajouter le nouveau
 * couple forme du nom de la variable et de sa valeur, en verifiant au
 * prealable que la variable n'existait pas deja (une seule declaration de
 * chaque variable d'apres l'enonce.
 *
 * Pour l'evaluation, voir l'ebauche de la fonction 'eval' dans tp.c
 */
decl : NOM_VAR AFF expr ';' { ajoutVar($1, $3); }
;

/* construire et renvoyer un arbre representant l'expression, en fonction des
 * arbres (deja construits) de ses sous-expressions.
 *
 * Voir les fonctions mises a disposition dans tp.c et en ajouter si besoin.
 */
expr : expr ADD expr { $$ = makeTree( ADD, 2, $1, $3); }
| expr SUB expr { $$ = makeTree( SUB, 2,$1, $3); }
| expr MUL expr { $$ = makeTree( MUL, 2,$1, $3); }
| expr DIV expr { $$ = makeTree( DIV, 2,$1, $3); }
| GET'('')' { $$ = makeLeafInt (GET,0); }
| PUT'('Strr',' expr')' { $$ = makeTree (PUT, 2, $3, $5) ; }
| IF boolExpr THEN expr ELSE expr { $$ = makeTree(IF, 3, $2, $4, $6);}
| SUB expr %prec urg // { $$ = makeTree(SUB, 1, $2);}
| ADD expr %prec urg // { $$ = $2;}
| CST/* cst yy contient la variable*/ { $$ = makeLeafInt(CST,0); }
| NOM_VAR { $$ = makeLeafStr(ID, $1); }
;

boolExpr : expr { $$ = $1;}
| expr RELOP expr {$$ = makeTree( RELOP, 2, $1, $3);}
| NOT boolExpr {$$ = makeTree(NOT, 1, $2);}
| boolExpr AND boolExpr {$$ = makeTree(AND, 2, $1, $3);}
| boolExpr OR boolExpr {$$ = makeTree(OR, 2, $1, $3);}
;

Strr : STR { $$ = yyval.S;}

/* Passage de valeur entre analyseur lexical et analyseur syntaxique via la
 * variable yylval (de type YYSTYPE)
 */
NOM_VAR : ID { $$ = yylval.S; }
;
