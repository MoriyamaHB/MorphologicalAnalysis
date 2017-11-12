#ifndef NODE_H_
#define NODE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CBYTE 2//Linuxでは3,Windowsでは2
#define STRLEN 48
#define MAXLINK 10
#define MAXPATH 30

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
	struct Node *prev[MAXLINK];
	struct Node *next[MAXLINK];
	int prevnum, nextnum;
	Word word;
	int p;
	int pnext;
	int pathnum;//このノード以下にある道の数
	int pathnum_branch[MAXLINK];//このノード以下にある道の数(枝用)
	int path[MAXPATH];//このノードを通るPATH番号
	int path_n;//PATH番号の格納数
	int has_number_path_n;//番号割り振り済みか
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
extern Node* findnextnullNode(Node* h, List *list);

//同じノード探索
extern Node* findNode(Word word, int p, Node *head, List *list);

//全てのノード表示
extern void printNodeTree(Node *t, List *list);

//全PATHの表示(1回のみ)
extern void printPath(List *list);

#endif
