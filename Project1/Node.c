#include "Node.h"

static char ClassStr[ClassNum][STRLEN] = { "|文頭|", "連体詞", "接尾詞", "名詞", "助動詞", "動詞", "|文末|", "|終了|" };

void printWord(Word w) {
	printf("%s\n", ClassStr[w.class]);
	if (w.class == BUNTO || w.class == BUNMATSU || w.class == FIN)
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
	printf("p=%d,pnext=%d,path_n=%d\n", n->p, n->pnext, n->path_n);
	int i;
	printf("path=");
	for (i = 0; i < n->path_n; i++) {
		printf("%d,", n->path[i]);
	}
	putchar('\n');
}

int strlenJP(char str[]) {
	return strlen(str) / CBYTE;
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
	n->pathnum = 0;
	n->path_n = 0;
	n->has_number_path = 0;
	return n;
}

void InitFrameNode(List *list) {
	list->head = newNode(NULL);
	((Node*)list->head)->word.class = BUNTO;
	list->fin = newNode(NULL);
	((Node*)list->fin)->word.class = FIN;
	list->tail = newNode(NULL);
	((Node*)list->tail)->word.class = BUNMATSU;
}

void makeNode(Node* p, Word word, int n, List *list) {
	Node *ptr = findNode(word, n, list->head, list);
	if (ptr == NULL) {
		Node *new = newNode(p);
		new->p = n;
		new->pnext = n + strlenJP(word.str);
		new->word = word;
		p->next[p->nextnum++] = new;
	}
	else {
		p->next[p->nextnum++] = ptr;
		ptr->prev[ptr->prevnum++] = p;
	}
}

void makeTailNode(Node *p, List *list) {
	p->next[p->nextnum++] = list->tail;
	((Node*)list->tail)->prev[((Node*)list->tail)->prevnum++] = p;
}

void makeFinNode(Node *p, List *list) {
	p->next[p->nextnum++] = list->fin;
	((Node*)list->fin)->prev[((Node*)list->fin)->prevnum++] = p;
}

/*次にさすポインタがNULlのノードを返す*/
Node* findnextnullNode(Node *h, List *list) {

	if (h == list->fin || h == list->tail)
		return NULL;

	if (h->next[0] == NULL)
		return h;

	int i;
	Node* re;
	for (i = 0; i < h->nextnum; i++) {
		if ((re = findnextnullNode(h->next[i], list)) != NULL)
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

//全ノードのPATH数を計算
//各ノード以下のPATHの数をメンバpathに記録
static int calcPathNum(Node* h, List *list) {
	if (h == list->tail) {
		h->pathnum = 1;
		return 1;
	}
	if (h == list->fin) {
		return 0;
	}
	int i, t, total = 0;
	for (i = 0; i < h->nextnum; i++) {
		t = calcPathNum(h->next[i], list);
		h->pathnum_branch[i] = t;
		total += t;
	}
	h->pathnum = total;
	return total;
}

//ノードを通るPATH番号をつける
static void numberNodePath(Node* t, List *list) {
	if (t->has_number_path == 1)
		return;
	if (t == list->head) {
		int i;
		for (i = 0; i < t->pathnum; i++) {
			t->path[t->path_n++] = i;
		}
		int j, cnt = 0;
		for (i = 0; i < t->nextnum; i++) {
			for (j = 0; j < (t->path_n * t->pathnum_branch[i]) / t->pathnum; j++) {
				((Node*)t->next[i])->path[((Node*)t->next[i])->path_n++] = t->path[cnt++];
			}
		}
		t->has_number_path = 1;//番号割り振り済み
		return;
	}
	int i;
	for (i = 0; i < t->prevnum; i++) {
		numberNodePath(t->prev[i], list);
	}
	int j, cnt = 0;
	for (i = 0; i < t->nextnum; i++) {
		for (j = 0; j < (t->path_n * t->pathnum_branch[i]) / t->pathnum; j++) {
			((Node*)t->next[i])->path[((Node*)t->next[i])->path_n++] = t->path[cnt++];
		}
	}
	t->has_number_path = 1;//番号割り振り済み
	return;
}

//Nodeがあるpath番号を持っているか
static int hasPathNumber(Node *node, int n) {
	int i;
	for (i = 0; i < node->path_n; i++) {
		if (node->path[i] == n)
			return 1;
	}
	return 0;
}

//Nodeからnのpath番号を取り除く
static void removePathNumber(Node *node, int n) {
	int i;
	for (i = 0; i < node->path_n; i++) {
		if (node->path[i] == n)
			node->path[i] = -1;
	}
	return;
}

//Path番号nのPath表示
static void printNumberPath(int num, Node *t, List *list) {
	if (hasPathNumber(t, num) == 0)
		return;
	if (t == list->head) {
		if (hasPathNumber(t, num) == 1) {
			removePathNumber(t, num);
			printNode(t);
		}
		return;
	}
	int i;
	for (i = t->prevnum - 1; i >= 0; i--) {
		printNumberPath(num, t->prev[i], list);
	}
	if (hasPathNumber(t, num) == 1) {
		removePathNumber(t, num);
		printNode(t);
	}
	return;
}

//全Pathの表示
static void printAllNumberPath(List *list) {
	int i;
	for (i = 0; i < ((Node*)list->head)->pathnum; i++) {
		printf("\n========== %d ============\n", i);
		printNumberPath(i, list->tail, list);
	}
	return;
}

//全PATHの表示(1回のみ)
void printPath(List *list) {
	printf("path=%d\n", calcPathNum(list->head, list));
	numberNodePath(list->tail, list);
	printAllNumberPath(list);
}

void printNodeTree(Node *t, List *list) {
	if (t == list->head) {
		printNode(t);
		return;
	}
	int i;
	for (i = t->prevnum - 1; i >= 0; i--) {
		printNodeTree(t->prev[i], list);
	}
	printNode(t);
	return;

}

