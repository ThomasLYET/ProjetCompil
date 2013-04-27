#ifndef A_BASE
#define A_BASE

#include <arbre_header.h>

/* deux macros pratiques, utilisees dans les allocations */
#define NEW(howmany, type) (type *) calloc((unsigned) howmany, sizeof(type))
#define NIL(type) (type *) 0

#define TRUE 1
#define FALSE 0 

typedef int bool;

/*
typedef struct _Fichier {
	ClassP classe;
	TreeP main;
}*/

/* Etiquettes additionnelles pour les arbres de syntaxe abstraite. Les tokens
 * tels que ADD, SUB, etc. servent directement d'etiquette.
 * Attention donc a ne pas donner des valeurs identiques a celles des tokens.
 */
#define NE	1
#define EQ	2
#define LT	3
#define LE	4
#define GT	5
#define GE	6
#define UMIN	7

/* Codes d'erreurs */
#define NO_ERROR	0
#define USAGE_ERROR	1
#define LEXICAL_ERROR	2
#define SYNTAX_ERROR    3
#define CONTEXT_ERROR	4
#define EVAL_ERROR	5
#define CLASS_NOT_FOUND 6
#define UNEXPECTED	10

/* Type pour la valeur de retour de Flex et les actions de Bison
 * le premier champ est necessaire pour flex
 * les autres correspondent aux variantes utilisees dans les actions
 * associees aux productions de la grammaire. 
*/
typedef union /*Pour Thomas : c'est une UNION !!! */
{	char C;
	char *S;   
	int I;    
	VarDeclP D;
	TreeP T;
	SuperClassP M; /*M comme mère*/
} YYSTYPE;

/* evaluateur de l'expression principale */
int eval(TreeP tree, VarDeclP decls);

int evalVar(TreeP tree, VarDeclP decls);

/* ecriture formatee */
void pprintVar(VarDeclP decl, TreeP tree);
void pprintValueVar(VarDeclP decl);
void pprint(TreeP tree);
void pprintMain(TreeP);

#define YYSTYPE YYSTYPE

#endif
