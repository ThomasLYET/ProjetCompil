#ifndef A_I
#define A_I

#include <arbre_header.h>

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
	struct _Class *mere;
    struct _Class *next;
} Class, *ClassP;

typedef struct _SuperClass
{   char* name;
	VarDeclP constructeur; /* TODO A changer vraisemblablement en arbre */
} SuperClass, *SuperClassP;

/* SuperClassP */
SuperClassP newSuperClassP(char* nom /*, VarDeclP variable // Voir problème dans grammaire.y \\ */ );

/* construction et accesseur de liste pour les classes */
ClassP addClass( char* nom, VarDeclP varConst, SuperClassP mere, TreeP construct ); /*TODO*/
ClassP findClass( char* nom );
void addChamp(int isStatic,char* nom ,char* classe,TreeP exprInit);
void addConstructeur(char* nomVar, char* class);
void addMere(char* nom);

#endif
