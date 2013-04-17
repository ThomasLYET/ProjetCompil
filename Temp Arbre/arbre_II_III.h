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
<<<<<<< HEAD
	VarDeclP varEntree; /*N'est pas initialisé !!*/
	VarDeclP varSortie; /* ATTENTION : il n'y a qu'une seul variable de sortie !!! N'est pas initialisé et n'a pas à priori de nom !!!! */
	ListArbreP first;
	//ListArbreP last;
	struct _Methode *next;
=======
	char* nom;
	VarDeclP varEntree; 
	VarDeclP varSortie; /* ATTENTION : il n'y a qu'une seul variable de sortie !!! */
	TreeP code;
	TreeP *next;
>>>>>>> 58f1e04895f1d4d4103fb41a60f6099ac6f376b1
} Methode, *MethodeP;

/* VarDeclP */
void ajoutVar(char *name, TreeP tree);
VarDeclP concatVarDeclP ( VarDeclP v1, VarDeclP v2);
VarDeclP newVarDeclP(char* nom, char* classe); /*TODO*/
int isVarDecl (ClassP classe, char* var)
void addChamp(int isStatic,char* nom ,char* class , TreeP exprInit);

/* Création des méthodes */
void createMethodFrom( VarDeclP output, VarDeclP inputs, TreeP instrs );

#endif
