
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE1 52973671      //links.txtの総行数
#define SIZE2 1483277       //pages.txtの総行数
#define SIZE_AIMAI 7289     //曖昧さ回避の総ページ数

#define WORD_LENGTH 1000     //wikipediaの見出し語の長さ

typedef struct data_t{
  int id;       //id
  int links;    //リンク数
  char *title;   //見出し語
} wiki_data;

//グローバル変数(あまり使いたくない…)
wiki_data aimai[SIZE_AIMAI];

int compare(const void *a, const void *b){

  wiki_data *w1,*w2;
  int links1,links2;

  w1 = (wiki_data *)a;
  w2 = (wiki_data *)b;

  links1 = w1->links;
  links2 = w2->links;

  return links2-links1;

}

//各語のリンク数を調べる
void linkscheck(int check_id, int num, FILE *fp_links){
  int r2, pageid, linksid;
  char* dam;
  dam = (char *)malloc(WORD_LENGTH);
  
  while((fgets(dam, WORD_LENGTH, fp_links)) != NULL){
    if((r2 = sscanf(dam, "%d\t%d\n", &pageid, &linksid)) != EOF){
      if(check_id == pageid){
	aimai[num].links++;
      }else if((check_id != pageid) && (aimai[num].links != 0)){
	//デバッグ用
	//printf("%d,%d\n",pageid,aimai[num].links);
	break;
	//links.txtのファイルの性質上これで数え終わる
      }//if
    } //if
  }//while
  if(r2 == EOF){
    printf("EOF id:%d,num:%d\n",check_id,num);
  } 
  return;
  
}

//aimai配列の中にidと見出しを格納
void make_aimai(char *filename1, char *filename2){
  
  FILE *fp1, *fp2;
  int read_id,r,i;
  char *read_word; //fgetsで読み込む用
  char *check; //strstrのNULL確認用
  read_word = (char *)malloc(WORD_LENGTH);
  i = 0;
  
  
  //fileを開く
  if((fp1 = fopen(filename1, "r")) == NULL){
    fprintf(stderr,"ERROR: fail to open file\n");
    exit(EXIT_FAILURE);
  }
  
  if((fp2 = fopen(filename2, "r")) == NULL){
    fprintf(stderr,"ERROR: fail to open file\n");
    exit(EXIT_FAILURE);
  }
  
  //曖昧さ回避ページ情報をaimaiに入れていく
  while((r = fscanf(fp1, "%d\t%s\n", &read_id, read_word)) != EOF){
    if((check = strstr(read_word,"曖昧さ回避")) != NULL){

      aimai[i].title = (char *)malloc(WORD_LENGTH);
      aimai[i].id = read_id;
      strcpy(aimai[i].title, read_word);
      aimai[i].links = 0;
      linkscheck(read_id, i, fp2);
      i++;
    }
  }
  
  free(read_word);
  fclose(fp1);
  fclose(fp2);
  
  return;
  
}

  
int main(void){

  int j,c,start,finish;

  make_aimai("pages.txt", "links.txt");
  qsort(aimai, SIZE_AIMAI, sizeof(wiki_data),compare);


  for(j=SIZE_AIMAI-20; j<SIZE_AIMAI; j++){
    printf("rank%d : %s, (links %d)\n", j+1, aimai[j].title, aimai[j].links);
  }

  /*
  while(1){

    printf("何位から何位まで見ますか？\n");
    printf("1位から10位まで見たい場合の入力例：1,10\n");
    printf("終了したい場合は0,0と入力してください\n");

    if((c = fscanf(stdin, "%d,%d", &start, &finish)) == EOF){
      printf("形式に従って入力してください\n");
    }else if((start == 0) && (finish == 0)){
      printf("またのご利用をお待ちしています！\n");
      break;
    }else{      
      for(j=start-1; j<finish; j++){
	printf("rank%d : %s, (links %d)\n", j+1, aimai[j].title, aimai[j].links);
      }//if
    }//if

  }//while
  */


}

