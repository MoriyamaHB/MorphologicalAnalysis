#include "Node.h"

static char ClassStr[ClassNum][STRLEN] = {
	"|文頭|","連体詞","接尾詞","名詞","助動詞","動詞","|文末|"
};

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

static Node* newNode(Node* prev) {
	Node *n = malloc(sizeof(Node));
	if (prev == NULL) {
		n->prev[0] = NULL;
		n->prevnum = 0;
	}
	else {
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
	((Node*)list->head)->word.class = BUNTO;
	list->fin = newNode(NULL);
	list->tail = newNode(NULL);
}

void makeNode(Node* p, Word word, int n, List *list) {
	Node *ptr = findNode(word, n, list->head, list);
	if (ptr == NULL) {
		Node *new = newNode(p);
		new->p = n;
		new->pnext = n + strlen(word.str) / CBYTE;
		new->word = word;
		p->next[p->nextnum++] = new;
	}
	else {
		p->next[p->nextnum++] = ptr;
		ptr->prev[ptr->prevnum++] = p;
	}
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

