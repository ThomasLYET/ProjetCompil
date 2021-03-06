#ifndef A_II_III
#define A_II_III

#include <arbre_header.h>

/* la structure ci-dessous permet de memoriser des listes variable/valeur
 * (entiere). On va construire des listes de la forme { (x 5), (y, 27) } au fur
 * et a mesure qu'on interprete les declarations dans le programme.
 */
typedef struct _Decl
{	char *name;
	union { /* Normalement c'est une "Class" SAUF dans le cas des classes prédéfini "Integer" et "String" */
		ClassP c;
		int i;
		char *str;
	} val;
	TreeP exprInit;
	struct _Decl *next;
} VarDecl, *VarDeclP;

typedef struct _Methode {
	char* nom;
	VarDeclP varEntree; /* Possède des noms mais pas de code !*/
	VarDeclP varSortie; /* ATTENTION : il n'y a qu'une seul variable de sortie !!! */
	BlocP code;
	struct _Methode *next;
} Methode, *MethodeP;

/* VarDeclP */
void ajoutVar(char *name, TreeP tree);
VarDeclP concatVarDeclP ( VarDeclP v1, VarDeclP v2);
VarDeclP newVarDeclP(char* nom, char* classe); /*TODO*/
VarDeclP varSansNom(TreeP valeur);
int isVarDecl (ClassP classe, char* var);
void addChamp(int isStatic,char* nom ,char* class , TreeP exprInit);

/* Création des méthodes */
void createMethodFrom( VarDeclP output, VarDeclP inputs, TreeP instrs );

#endif
