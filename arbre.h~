#include <stdlib.h>

/* deux macros pratiques, utilisees dans les allocations */
#define NEW(howmany, type) (type *) calloc((unsigned) howmany, sizeof(type))
#define NIL(type) (type *) 0

#define TRUE 1
#define FALSE 0 

typedef int bool;

/* la structure d'un arbre (noeud ou feuille) */
typedef struct _Tree {
	short op;         /* etiquette de l'operateur courant */
	short nbChildren; /* nombre de sous-arbres */
	union {
		char *str;      /* valeur de la feuille si op = ID ou STR */
		int val;        /* valeur de la feuille si op = CST */
		struct _Tree **children; /* tableau des sous-arbres */
	} u;
} Tree, *TreeP;

typedef struct _Methode {
	VarDeclP varEntree; 
	VarDeclP varSorti; /* ATTENTION : il n'y a qu'une seul variable de sorti !!! */
	TreeP code;
} Methode, *MethodeP;

/* Structure qui permet de mémoriser la liste de toutes les classes défini
 * dans le fichier
 */
typedef struct _Class
{   char *name;
	VarDeclP varConstruction; /* Les variables nécessaires à la construction de la classe */
	VarDeclP attribut;
	VarDeclP attributStatic;
	MethodeP methode;
	MethodeP methodeStatic; /* ATTENTION : ne peut pas utiliser des attributs NON-static ! */
	TreeP constructeur;
    struct _Class *next;
} Class, *ClassP;

/* la structure ci-dessous permet de memoriser des listes variable/valeur
 * (entiere). On va construire des listes de la forme { (x 5), (y, 27) } au fur
 * et a mesure qu'on interprete les declarations dans le programme.
 */
 /* TODO : Trouver commer stocker la valeur de la variable ! */
typedef struct _Decl
{	char *name;
	union { /* Normalement c'est une "Class" SAUF dans le cas des classes prédéfini "Integer" et "String" */
		Class c;
		int i;
		char *str;
	} val;
	struct _Decl *next;
} VarDecl, *VarDeclP;

typedef struct _Fichier {
	ClassP classe;
	TreeP main;
}

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
{ char C;
  char *S;   
  int I;    
  VarDeclP D;
  TreeP T;
} YYSTYPE;

int eval(TreeP tree, VarDeclP decls);

/* construction et accesseur de liste pour les classes */
ClassP addClass( char* nom);
ClassP findClass( char* nom );
void addConstructeur(char* nomVar, char* class);

/* construction et accesseur pour les arbres */
TreeP makeLeafStr(short op, char *str);
TreeP makeLeafInt(short op, int val);
TreeP makeTree(short op, int nbChildren, ...);
TreeP getChild(TreeP tree, int rank);

/* evaluateur de l'expression principale */
int eval(TreeP tree, VarDeclP decls);

/* ecriture formatee */
void pprintVar(VarDeclP decl, TreeP tree);
void pprintValueVar(VarDeclP decl);
void pprint(TreeP tree);
void pprintMain(TreeP);

#define YYSTYPE YYSTYPE
