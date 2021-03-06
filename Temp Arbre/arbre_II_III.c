#include "arbre_II_III.h"

VarDecl *declaration = NULL;
extern ClassP lClass;
extern int yyparse();
extern int yylineno;
extern bool verbose = FALSE;
extern bool noEval = FALSE;
extern int errorCode;
extern FILE *fd = NIL(FILE);

void ajoutVar(char *name, TreeP tree) {	
	VarDecl *temp = malloc(sizeof(VarDecl));
	temp->name = name;
	temp->val = eval(tree, declaration);
	if (declaration != NULL); { /*cas initial*/
		temp->next = declaration;
	}
	declaration = temp;
}

VarDeclP concatVarDeclP ( VarDeclP v1, VarDeclP v2) {
	v1->next = v2;
	return v1;
}

VarDeclP newVarDeclP(char* nom, char* classe) {
	VarDeclP result = NEW(1,VarDecl);
	result->name = nom;
	result->val.c = classe;
	return result;
}

VarDeclP varSansNom(TreeP valeur) {
	VarDeclP result = NEW(1,VarDecl);
	result->exprInit = valeur;
	return result;
}

void createMethodFrom(char* nom,VarDeclP inputs, VarDeclP output, TreeP instrs )
{
	
	MethodeP tmp = NEW (1, Methode);
	/* On sette les champs de la méthode */
	tmp->nom = nom;
	tmp->varSortie = output;
        tmp->code = instrs;
	tmp->varEntree = inputs;

	/* On ajoute la méthode à la classe */
	tmp->next = lClass->methode
	lClass->methode = tmp; 
}

/** Renvoie 1 (TRUE) ou 0 (FALSE) si la class contient l'attribut
 * (pour le moment pas de méthode ou de private/public) 
 */
/* TODO : n'a pas l'air d'etre utilisé quelque aprt, à supprimer si besoin */
bool isVarDecl (ClassP classe, char* var) {
	VarDeclP temp = classe->attribut;
	while (temp != NULL) {
		if (strcmp(var, temp->name) == 0 ) {
			return TRUE;
		}
	}
	temp = classe->attributStatic;
	while (temp != NULL) {
		if (strcmp(var, temp->name) == 0 ) {
			return TRUE;
		}
	}
	
	return 0;
}

void addChamp(int isStatic,char* nom ,char* class , TreeP exprInit) {     
/* TODO static */
	VarDeclP var = NEW(1,VarDecl);
	ClassP theClass = findClass(class);
	var->name = nom;
	var->val.c = theClass;
	var->next = lClass->attribut;
	lClass->attribut = var;
	var->exprInit = exprInit;
}

}


