#include <unistd.h>
#include <stdarg.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "arbre.h"
#include "tp_y.h"

extern int yyparse();
extern int yylineno;

/*typedef struct _Decl
{ char *name;
  int val;
  struct _Decl *next;
} VarDecl, *VarDeclP;*/

/*La liste de toutes les classes*/
ClassP lClass;

VarDecl *declaration = NULL;

void ajoutVar(char *name, TreeP tree) {	
	VarDecl *temp = malloc(sizeof(VarDecl));
	temp->name = name;
	temp->val = eval(tree, declaration);
	if (declaration != NULL); { /*cas initial*/
		temp->next = declaration;
	}
	declaration = temp;
}

/* Niveau de 'verbosite'.
 * Par defaut, n'imprime que le resultat et les messages d'erreur
 */
bool verbose = FALSE;

/* Evaluation ou pas. Par defaut, on evalue les expressions (si on appelle
 * bien la fonction eval dont l'en-tete est ci-dessus et le squelette du
 * code en fin de fichier.
 */
bool noEval = FALSE;

/* code d'erreur a retourner */
int errorCode = NO_ERROR;

/* Descripteur de fichier pour la lecture des donnees par get */
FILE *fd = NIL(FILE);

/* Appel:
 *   tp [-option]* programme.txt donnees.dat
 * Le fichier de donnees est obligatoire si le programme execute la
 * construction GET (pas de lecture au clavier), facultatif sinon.
 * Les options doivent apparaitre avant le nom du fichier du programme.
 * Options: -[eE] -[vV] -[hH?]
 */
int main(int argc, char **argv) {
	int fi;
	int i, res;

	for(i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			switch (argv[i][1]) {
			case 'v': case 'V':
				verbose = TRUE; continue;
			case 'e': case 'E':
				noEval = TRUE; continue;
			case '?': case 'h': case 'H':
				fprintf(stderr, "Command: tp -e -v program.txt data.dat\n");
				exit(USAGE_ERROR);
			default:
				fprintf(stderr, "Error: Unknown option: %c\n", argv[i][1]);
				exit(USAGE_ERROR);
			}
		} else break;
	}

	if (i == argc) {
		fprintf(stderr, "Error: Missing program file\n");
		exit(USAGE_ERROR);
	}

	if ((fi = open(argv[i++], O_RDONLY)) == -1) {
		fprintf(stderr, "Error: Cannot open %s\n", argv[i-1]);
		exit(USAGE_ERROR);
	}

	/* redirige l'entree standard sur le fichier... */
	close(0); dup(fi); close(fi);

	if (i < argc) { /* fichier dans lequel lire les valeurs pour get() */
		if ((fd = fopen(argv[i], "r")) == NULL) {
			fprintf(stderr, "Error: Cannot open %s\n", argv[i]);
			exit(USAGE_ERROR);
		}
	}

	/* Lance l'analyse syntaxique de tout le source, en appelant yylex au fur
	 * et a mesure. Execute les actions semantiques en parallele avec les
	 * reductions.
	 * yyparse renvoie 0 si le source est syntaxiquement correct, une valeur
	 * differente de 0 en cas d'erreur syntaxique (eventuellement causee par des
	 * erreurs lexicales).
	 * Comme l'interpretation globale est automatiquement lancee par les actions
	 * associees aux reductions, une fois que yyparse a termine il n'y
	 * a plus rien a faire (sauf fermer les fichiers)
	 * Si le code du programme contient une erreur, on bloque l'evaluation.
	 * S'il n y a que des erreurs contextuelles on essaye de ne pas s'arreter
	 * a la premiere mais de continuer l'analyse pour en trouver d'autres, quand
	 * c'est possible.
	 */
	res = yyparse();
	if (fd != NIL(FILE)) fclose(fd);
	return res ? SYNTAX_ERROR : errorCode;
}


void setError(int code) {
  errorCode = code;
  if (code != NO_ERROR) { noEval = TRUE; }
}

/* yyerror:  fonction importee par Bison et a fournir explicitement. Elle
 * est appelee quand Bison detecte une erreur syntaxique.
 * Ici on se contente d'un message minimal.
 */
void yyerror(char *ignore) {
  printf("erreur de syntaxe: Ligne %d\n", yylineno);
}

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
		 * fonction "findclass"
		 */
		/* Si c'est effectivment le nom d'une classe, alors il faut
		 * chercher une static !!!
		 */
		bool continuer = TRUE;
		VarDeclP temp = laClass->attributStatic;
		while (temp != NULL && continuer) {
			if (strcmp(var, temp->name) == 0 ) {
				continuer = FALSE;
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

/** Renvoie 1 (TRUE) ou 0 (FALSE) si la class contient l'attribut
 * (pour le moment pas de méthode ou de private/public) 
 */
int isVarDecl (ClassP classe, char* var) {
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

/* retrouve la valeur d'une variable stockee dans 'tree' si elle apparait bien
 * bien dans la liste des couples (nom, valeur) de 'decls'. Sinon quitte
 * irremediablement le programme car on ne va pas lui donner une valeur
 * arbitraire. 
 */
int evalVar(TreeP tree, VarDeclP decls) {
	char *name = tree->u.str;
	while (decls != NIL(VarDecl)) {
		if (! strcmp(decls->name, name)) return(decls->val);
		decls = decls->next;
	}
	fprintf(stderr, "Error: Undeclared variable: %s\n", name);
	exit(UNEXPECTED);
}
/* ATTENTION : ce programme servira pour faire le parcours d'arbre, ne pas toucher pour le moment ! */
/* Evaluation par parcours recursif de l'arbre representant une expression. 
 * Les valeurs des identificateurs situes aux feuilles de l'arbre sont a
 * rechercher dans la liste 'decls'
 * Attention dans chaque a n'evaluer que ce qui doit l'etre et au bon moment
 * selon la semantique de l'operateur (cas du IF, and, or, etc.)
 */
int eval(TreeP tree, VarDeclP decls) {
	switch (tree->op) {
	case ID:
		return evalVar(tree, decls);
	case CST:
		return(tree->u.val);
	case EQ:
	case NE:
	case GT:
	case GE:
	case LT:
	case LE:
	case ADD:
	case SUB:
	default: 
		fprintf(stderr, "Error: Eval unexpected operator label: %d\n", tree->op);
		exit(UNEXPECTED);
	}
}

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

void addChamp(int isStatic,char* nom ,char* class , TreeP exprInit){               /* TODO static */
	VarDeclP var = NEW(1,VarDecl);
	ClassP theClass = findClass(class);
	var->name = nom;
	var->val.c = theClass;
	var->next = lClass->attribut;
	lClass->attribut = var;
	var->exprInit = exprInit;
}


typedef struct _Decl
{	char *name;
	union { 
		Class c;
		int i;
		char *str;
	} val;
	struct _Decl *next;
} VarDecl, *VarDeclP;

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


void createMethodFrom( VarDeclP output, VarDeclP inputs, TreeP instrs )
{
	Methode tmp = NEW (1, Methode);
	/* On sette les champs de la méthode */
	tmp->varSortie = output;

	while( inputs != NULL )
	{
		tmp->varEntree = inputs;
		tmp->next = NEW (1, VarDeclP);
		tmp = tmp->next;
		inputs=inputs->next;
	}

	/* On ajoute la méthode à la classe */
	myClass->

	lClass->methode
}
