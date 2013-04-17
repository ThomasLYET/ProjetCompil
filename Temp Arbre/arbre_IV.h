#ifndef A_IV
#define A_IV

#include <arbre_header.h>

/* la structure d'un arbre (noeud ou feuille) */
typedef struct _Tree {
	short op;         /* etiquette de l'operateur courant */
	short nbChildren; /* nombre de sous-arbres */
	union {
		char *str;      /* valeur de la feuille si op = ID ou STR */
		int val;        /* valeur de la feuille si op = CST */
		VarDeclP var;	/* nécessaire pour a.b */
		ClassP classe;	/* nécessaire pour a.b() */
		struct _Tree **children; /* tableau des sous-arbres */
	} u;
} Tree, *TreeP;

/* construction et accesseur pour les arbres */
TreeP makeLeafVar(TreeP classe, char* var);
TreeP makeLeafVarAUX(int op, VarDeclP var);
TreeP makeLeafMet(TreeP classe, char* var);
TreeP makeLeafStr(short op, char *str);
TreeP makeLeafInt(short op, int val);
TreeP makeTree(short op, int nbChildren, ...);
TreeP getChild(TreeP tree, int rank);
TreeP findTree(char* classe, char* var);

#endif
