extern int yyparse();
extern int yylineno;


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


