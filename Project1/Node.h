#ifndef NODE_H_
#define NODE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CBYTE 2//Linuxでは3,Windowsでは2
#define STRLEN 48
#define MAXLINK 10

//品詞
enum Class {
	BUNTO = 0, RENTAI, SETSUBI, MEI, JO, DOU, BUNMATSU, FIN, ClassNum
};

//単語
typedef struct {
	char str[STRLEN];
	enum Class class;
} Word;

//ノード
typedef struct {
	struct Node *prev[10];
	struct Node *next[10];
	int prevnum, nextnum;
	Word word;
	int p;
	int pnext;
} Node;

//リスト
typedef struct {
	struct Node* head;
	struct Node* tail;
	struct Node* fin;
} List;

//Word表示
extern void printWord(Word w);

//Word配列表示
extern void printWordArray(Word wa[], int n);

//Node表示
extern void printNode(Node *n);

//日本語文字数を返す
extern int strlenJP(char str[]);

//ノード構造を作成
extern void InitFrameNode(List *list);

//新しくノードを追加
extern void makeNode(Node* p, Word word, int n, List *list);

//文末ノードをつける
extern void makeTailNode(Node *p, List *list);

//終了ノードをつける
extern void makeFinNode(Node *p, List *list);

//後ろがNULLのノード探索
extern Node* findnextnullNode(Node* head, List *list);

//同じノード探索
extern Node* findNode(Word word, int p, Node *head, List *list);

//全てのノード表示
extern void printNodeTree(Node *ptr, List *list);

//全てのノード表示
extern void printNodeTree2(Node *ptr, List *list, int n, int m);
extern void PPP(List *list);

#endif
