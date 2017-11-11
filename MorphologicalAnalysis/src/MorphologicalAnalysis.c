/*
 ============================================================================
 Name        : MorphologicalAnalysis.c
 Author      :
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Node.h"

 //連結可能性辞書
int connection_array[ClassNum][ClassNum] = { { 0, 1, 0, 1, 0, 1, 0 }, { 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 1 },
		{ 0, 0, 1, 1, 1, 1, 1 }, { 0, 0, 0, 1, 0, 0, 1 }, { 0, 0, 1, 1, 1, 0, 1 }, { 0, 0, 0, 0, 0, 0, 0 } };

//連結可能性を返す
int connection(Word a, Word b) {
	return connection_array[a.class][b.class];
}

//単語辞書配列
Word dictionary_array[] = { { "こ", SETSUBI }, { "この", RENTAI }, { "ひ", MEI }, { "と", JO }, { "ひと", MEI }, { "こと", MEI },
		{ "ひとこと", MEI }, { "で", DOU }, { "で", MEI }, { "元気", MEI }, { "に", JO }, { "なった", DOU }, { "になった", DOU } };

//単語辞書(一致したものをwaに格納,一致数を返す)
int dictionary(char str[], Word wa[]) {
	int i, num = 0;
	for (i = 0; i < sizeof(dictionary_array) / sizeof(Word); i++) {
		if (strcmp(str, dictionary_array[i].str) == 0) {
			wa[num++] = dictionary_array[i];
		}
	}
	return num;
}

//ポインタの位置から検索される単語の配列を返す
//一致したものをwaに格納,一致数を返す
int findWord(char string[], int p, Word wa[]) {
	int string_num = strlen(string + (p * CBYTE)) / CBYTE;//pの位置からの日本語文字数
	char string_search[STRLEN];//探索したい文字列
	int num = 0;//一致数

	int i;
	Word wp[MAXLINK];
	for (i = 1; i < string_num; i++) {//(1:こ,2:この,3このひ…)
		//探索する文字列を作成
		strncpy(string_search, string + (p * CBYTE), (i*CBYTE));
		string_search[i*CBYTE] = '\0';
		//探索結果を配列arrayに連結
		int search_num;
		if ((search_num = dictionary(string_search, wp)) != 0) {
			int j;
			for (j = 0; j < search_num; j++) {
				wa[num++] = wp[j];
			}
		}
	}
	return num;
}

int main(void) {
	char main_str[] = "このひとことで元気になった";
	List list;
	Node *ptr;

	InitFrameNode(&list);
	while ((ptr = findnextnullNode(list.head, &list)) != NULL) {
		//pの位置から始まる単語検索
		Word w[MAXLINK];
		int num;
		if ((num = findWord(main_str, ptr->pnext, w)) != 0) {
			//単語をノードとして追加
			int i;
			for (i = 0; i < num; i++) {
				if (connection(ptr->word, w[i])) { //ptrとwの単語が連結可能なら
					makeNode(ptr, w[i], ptr->pnext, &list); //ノード作成
				}
			}
		}
	}
	return 0;
}

