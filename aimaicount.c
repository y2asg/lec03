// 曖昧さ回避のページ数を調べる

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#define SIZE1 52973671      //links.txtの総行数
//#define SIZE2 1483277       //pages.txtの総行数

#define WORD_LENGTH 1000     //wikipediaの見出し語の長さ

//曖昧さ回避ページをカウントする関数
int count(int num, char *filename){
  
  FILE *fp;
  char *word; //fgetsで読み込む用
  char *check; //strstrのNULL確認用
  word = (char *)malloc(WORD_LENGTH);
  num = 0;
  
  //fileを開く
  if((fp = fopen(filename, "r")) == NULL){
    fprintf(stderr,"ERROR: fail to open file\n");
    exit(EXIT_FAILURE);
  }

  //曖昧さ回避が見出し語にあるか調べる
  while(fgets(word, WORD_LENGTH, fp) != NULL){
    if((check = strstr(word,"曖昧さ回避")) != NULL){
      num++;
    }
  }

  free(word);

  return num;

}

  
int main(void){

  int counter;
  int a;

  counter = count(a,"pages.txt");

  printf("曖昧さ回避のためのページは%dページでした\n",counter);

}
