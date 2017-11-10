#include "Node.h"

static char ClassStr[ClassNum][STRLEN] = { "|文頭|", "連体詞", "接尾詞", "名詞", "助動詞", "動詞", "|文末|", "|終了" };

void printWord(Word w) {
	printf("%s\n", ClassStr[w.class]);
	if (w.class == BUNTO || w.class == BUNMATSU)
		return;
	printf("%s\n", w.str);
}

void printWordArray(Word wa[], int n) {
	int i;
	for (i = 0; i < n; i++) {
		printWord(wa[i]);
	}
}

void printNode(Node *n) {
	printf("---------------\n");
	printWord(n->word);
	printf("p=%d,pnext=%d\n", n->p, n->pnext);
}

int strlenJP(char str[]) {
	return strlen(str) / CBYTE;
}

static Node* newNode(Node* prev) {
	Node *n = malloc(sizeof(Node));
	if (prev == NULL) {
		n->prev[0] = NULL;
		n->prevnum = 0;
	} else {
		n->prev[0] = prev;
		n->prevnum = 1;
	}
	n->next[0] = NULL;
	n->nextnum = 0;
	n->p = 0;
	n->pnext = 0;
	return n;
}

void InitFrameNode(List *list) {
	list->head = newNode(NULL);
	((Node*) list->head)->word.class = BUNTO;
	list->fin = newNode(NULL);
	((Node*) list->fin)->word.class = FIN;
	list->tail = newNode(NULL);
	((Node*) list->tail)->word.class = BUNMATSU;
}

void makeNode(Node* p, Word word, int n, List *list) {
	Node *ptr = findNode(word, n, list->head, list);
	if (ptr == NULL) {
		Node *new = newNode(p);
		new->p = n;
		new->pnext = n + strlenJP(word.str);
		new->word = word;
		p->next[p->nextnum++] = new;
	} else {
		p->next[p->nextnum++] = ptr;
		ptr->prev[ptr->prevnum++] = p;
	}
}

void makeTailNode(Node *p, List *list) {
	p->next[p->nextnum++] = list->tail;
	((Node*) list->tail)->prev[((Node*) list->tail)->prevnum++] = p;
}

void makeFinNode(Node *p, List *list) {
	p->next[p->nextnum++] = list->fin;
	((Node*) list->fin)->prev[((Node*) list->fin)->prevnum++] = p;
}

/*次にさすポインタがNULlのノードを返す*/
Node* findnextnullNode(Node *ptr, List *list) {

	if (ptr == list->fin || ptr == list->tail)
		return NULL;

	if (ptr->next[0] == NULL)
		return ptr;

	int i;
	Node* re;
	for (i = 0; i < ptr->nextnum; i++) {
		if ((re = findnextnullNode(ptr->next[i], list)) != NULL)
			return re;
	}
	return NULL;
}

/*単語探索*/
Node* findNode(Word word, int p, Node *ptr, List *list) {

	if (ptr == list->fin || ptr == list->tail)
		return NULL;

	if (strcmp(ptr->word.str, word.str) == 0 && ptr->p == p)
		return ptr;

	int i;
	Node* re;
	for (i = 0; i < ptr->nextnum; i++) {
		if ((re = findNode(word, p, ptr->next[i], list)) != NULL)
			return re;
	}
	return NULL;
}

#define MAXPATH 100
Node* node[MAXPATH][MAXLINK] = { 0 };
int ng = 0, mg = 0;

void printNodeTree(Node *ptr, List *list, int n, int m) {

//	int i;
//	for (i = 0; i != -1; i++) {
//		Node* n;
//		for (n = list->head;; n = n->next[i]) {
//			if (n->word.class == FIN || n->word.class == BUNMATSU) {
//				break;
//			}
//			printNode(n);
//		}
//	}

//	if (ptr == list->head) {
//		printNode(ptr);
//		return;
//	}
//	int i;
//	for (i = ptr->prevnum - 1; i >= 0; i--) {
//		printNodeTree(ptr->prev[i], list);
//	}
//	printNode(ptr);
//	return;

	if (ptr == list->head) {
		node[n][m] = ptr;
		return;
	}
	int i;
	for (i = ptr->prevnum - 1; i >= 0; i--) {
		memcpy(node[n + i], node[n], sizeof(node) * MAXLINK);
		ng++;
		printNodeTree(ptr->prev[i], list, n + i, m++);
	}
	node[n][m] = ptr;
	return;
}

void PPP(List *list) {
	int i, j;
	for (i = 0; i < ng; i++) {
		for (j = 0; node[i][j] != list->tail; j++) {
			printNode(node[i][j]);
		}
	}
}

