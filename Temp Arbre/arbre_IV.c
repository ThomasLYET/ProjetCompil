#include "arbre_IV.h"

extern ClassP lClass;
extern int yyparse();
extern int yylineno;
extern bool verbose = FALSE;
extern bool noEval = FALSE;
extern int errorCode;
extern FILE *fd = NIL(FILE);
extern VarDecl *declaration = NULL;

/* Tronc commun pour la construction d'arbre */
TreeP makeNode(int nbChildren, short op) {
	TreeP tree = NEW(1, Tree);
	tree->op = op;
	tree->nbChildren = nbChildren;
	tree->u.children = nbChildren > 0 ? NEW(nbChildren, TreeP) : NIL(TreeP);
	return(tree);
}


/* Construction d'un arbre a nbChildren branches, passees en parametres */
TreeP makeTree(short op, int nbChildren, ...) {
	va_list args;
	int i;
	TreeP tree = makeNode(nbChildren, op); 
	va_start(args, nbChildren);
	for (i = 0; i < nbChildren; i++) { 
		tree->u.children[i] = va_arg(args, TreeP);
	}
	va_end(args);
	return(tree);
}

/* Retourne le rankieme fils d'un arbre (de 0 a n-1) */
TreeP getChild(TreeP tree, int rank) {
	return tree->u.children[rank];
}

/* Constructeur de feuille dont la valeur est une chaine de caracteres
 * (un identificateur de variable ou parametre, la chaine d'un PUT).
 */
TreeP makeLeafStr(short op, char *str) {
	TreeP tree = makeNode(0, op);
	tree->u.str = str;
	return(tree);
}

/* Constructeur de feuille dont la valeur est un entier */
TreeP makeLeafInt(short op, int val) {
	TreeP tree = makeNode(0, op); 
	tree->u.val = val;
	return(tree);
}

/* makeLeafVar ne prend en entré soit le nom d'une classe, dans ce cas 
 * l'attribut recherché est forcément static.
 * soit une variable dans ce cas l'attribut est static ou non.
 */
TreeP makeLeafVar(TreeP classe, char* var) {
	
	/* REVOIR quel est le bon identifiant (ID ou STR) !!!! */
	if (classe->op == ID || classe->op == STR ) { 
		char* nomClasse = classe->u.str;
		ClassP laClass = findClass(classe);
		/* Si la classe n'existe pas, l'erreur a déjà été révélé par la
		 * fonction "findclass" */
		/* Si c'est effectivment le nom d'une classe, alors il faut
		 * chercher une variable static !!!
		 */
		bool continuer = TRUE;
		VarDeclP temp = laClass->attributStatic;
		while (temp != NULL && continuer) {
			if (strcmp(var, temp->name) == 0 ) {
				continuer = FALSE;
			}
			else {
				temp = temp->next;
			}
		}
		if ( continuer = FALSE ) {
			return makeLeafVarAux(VAR, temp);
		}
		fprintf(stderr, "The class \"%s\" doesn't have the static attribut \"%s\"",nomClasse,var);
		exit(CLASS_NOT_FOUND);
	}
	else if (classe->op == VAR ) {
		ClassP laClass = classe->u.var->val.c;
		VarDeclP temp = laClass->attribut;
		bool continuer = TRUE;
		while (temp != NULL && continuer) {
			if (strcmp(var, temp->name) == 0 ) {
				continuer = FALSE;
			}
		}
		if ( continuer = FALSE ) {
			return makeLeafVarAux(temp);
		}
		temp = laClass->attributStatic;
		continuer = TRUE;
		while (temp != NULL && continuer) {
			if (strcmp(var, temp->name) == 0 ) {
				continuer = FALSE;
			}
		}
		if ( continuer = FALSE ) {
			return makeLeafVarAux(temp);
		}
		fprintf(stderr, "The class \"%s\" doesn't have the ( static or not ) attribut \"%s\"",laClass->name,var);
		exit(CLASS_NOT_FOUND);
	}
	
	fprintf(stderr, " This error is not expected (in fonction makeLeafVar)" );
	exit(CLASS_NOT_FOUND);
}


TreeP makeLeafVarAUX(int op, VarDeclP var) {
	TreeP tree = makeNode(0, VAR);
	tree->u.var = var;
	return(tree);
}

TreeP makeLeafMet(TreeP classe, char* meth, 


InstructionsP concatInstruction(InstructionsP premier, InstructionsP suivants) {
	premier->next = suivants;
	return premier;
}
