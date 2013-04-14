TreeP findTree(TreeP classe, char* var) {
	
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
			return makeLeafVar(VAR, temp);
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
			return makeLeafVar(VAR, temp);
		}
		temp = laClass->attributStatic;
		continuer = TRUE;
		while (temp != NULL && continuer) {
			if (strcmp(var, temp->name) == 0 ) {
				continuer = FALSE;
			}
		}
		if ( continuer = FALSE ) {
			return makeLeafVar(VAR, temp);
		}
		fprintf(stderr, "The class \"%s\" doesn't have the ( static or not ) attribut \"%s\"",laClass->name,var);
		exit(CLASS_NOT_FOUND);
	}
	
	fprintf(stderr, " This error is not expected (in fonction findTree)" );
	exit(CLASS_NOT_FOUND);
}

TreeP makeLeafVar(int op, VarDeclP var) {
	TreeP tree = makeNode(0, op);
	tree->u.var = var;
	return(tree);	
}

