


/* findTree ne prend en entré soit le nom d'une classe, dans ce cas 
 * l'attribut recherché est forcément static.
 * soit une variable dans ce cas l'attribut est static ou non.
 */

TreeP makeLeafVarAUX(int op, VarDeclP var) {
	TreeP tree = makeNode(0, VAR);
	tree->u.var = var;
	return(tree);
}






void addChamp(int isStatic,char* nom ,char* class , TreeP exprInit {               /* TODO static */
	VarDeclP var = NEW(1,VarDecl);
	ClassP theClass = findClass(class);
	var->name = nom;
	var->val.c = theClass;
	var->next = lClass->attribut;
	lClass->attribut = var;
	var->exprInit = exprInit;
}


