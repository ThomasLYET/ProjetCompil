#include "arbre_I.h"

/*La liste de toutes les classes*/
ClassP lClass;

extern int yyparse();
extern int yylineno;
extern bool verbose = FALSE;
extern bool noEval = FALSE;
extern int errorCode;
extern FILE *fd = NIL(FILE);
extern VarDecl *declaration = NULL;

/** 
 * Créer une classe avec seulement le nom, les autres éléments devront
 * être ajouté manuellment après.
 */
ClassP addClass( char* nom ) {
	ClassP result = NEW(1,Class);
	result->name =nom;
	result->next =lClass;
	lClass = result;
	return result;
}

/**
 * Renvoie la Class ayant le nom demandé
 */
ClassP findClass( char* nom ) {
	ClassP temp = lClass;
	while (temp != NULL) {
		if (strcmp(nom,temp->name) == 0 ) {
			return temp;
		}
		temp = temp->next;
	}
	fprintf(stderr, "Error: Class name doesn't exist : %s\n", nom);
	exit(CLASS_NOT_FOUND);
}



/**
 * Pas besoin d'avoir la classe en variable, en effet, c'est lClass
 * directement qui est concerné.
 */
void addConstructeur(char* nomVar, char* class) {
	ClassP theClass = findClass(class);
	
	VarDeclP var = NEW(1,VarDecl);
	var->name = nomVar;
	var->val.c = theClass;
	var->next = lClass->constructeur;
	lClass->constructeur = var;
}

/**
 * Ajoute la classe mère
 */
void addMere(char* nom) {
	lClass.mere = findClass(nom);
}

