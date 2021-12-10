#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include "text_processing.h"
#define STEP 5


struct Sentence{
    wchar_t *words;
    int size;
    int len;

};
struct Sentence* readSentence(){
    int size=STEP;

    wchar_t *buf = malloc(size* sizeof(wchar_t));
    wchar_t temp;
    int n=0;
    do{
        if(n<=size-2){
            wchar_t *t= realloc(buf,size+STEP);
            if(!t){puts("ERROR: закончилась память");}
            size+=STEP;
            buf=t;
            //printf("new size=%d\n",size);

        }
        temp=getwchar();
        buf[n]=temp;
        n++;
    }while(temp!='.' && temp!='\n');
    buf[n]='\0';
    struct Sentence *sentence= malloc(sizeof (struct Sentence));
    sentence->words=buf;
    sentence->size=size;
    sentence->len=n-1;

    return sentence;
}

struct Text{
    struct Sentence **sentences;
    int size;
    int n;
};

struct Text readText(){
    int size=STEP;
    struct Sentence** txt= malloc(size*sizeof(struct Sentence));
    int n=0;
    struct Sentence *temp;
    int null_kol=0;
    do{
        temp=readSentence();
        if(temp->words[0]=='\n'){
            null_kol++;
            free(temp->words);
            free(temp);
        }else{
            null_kol=0;
            txt[n]=temp;
            n++;
        }
    } while (null_kol<1);

    struct Text full_text;
    full_text.size=size;
    full_text.sentences=txt;
    full_text.n=n;

    return full_text;

}

/*struct Text del_rep(struct Text text){


    wchar_t *check_words;
    wchar_t *word_now;
    struct Text new_text=text;
    int len_of_txt=new_text.n;


    for(int i=0;i<len_of_txt;i++){
        check_words=new_text.sentences[i]->words;

        for(int j=i+1;j<len_of_txt;j++){
            word_now=new_text.sentences[j]->words;

            int len=new_text.sentences[j]->len;
            int check_symb=0;
            ////посимвольное сравнение
                for(int k=0;k<len;k++){
                    if(towupper(word_now[k])== towupper(check_words[k])){
                        check_symb=1;
                    }else{
                        check_symb=0;
                        break;
                    }
                }
            ////
            if(check_symb){

                memmove(&new_text.sentences[j],&new_text.sentences[j+1],(len_of_txt-j)*sizeof(struct Sentence*));
                len_of_txt--;
                j--;

            }


        }

    }

    new_text.n=len_of_txt;

    return new_text;

}*/


int* task4 (struct Text text){

    int arr_repeat[text.n];


    return arr_repeat;
}

int main() {

    setlocale(LC_ALL,"");

    struct Text text = readText();
    struct Text new_text = del_rep(text);


    ///////////////////////////////////////////////////////////////////////////////////
/*
    int operation;
    printf("Для получения маски по каждому предложению введите 1.\nЧтобы удалить все предложения, в которых нет заглавных букв в начале слова введите 2."
          "\nЧтобы отсортировать слова в предложении по количеству гласных букв в слове введите 3."
          "\nЧтобы для каждого предложения получить количество одинаковых слов в строке введите 4."
          "\nЧтобы завершить программу введите любое другое число.\n--->");
       scanf("%d",&operation);
    switch (operation) {
        case 1:
            break;
        case 2:{
                new_text= task2(new_text);
                for(int i=0;i<new_text.n;i++){
                    wprintf(L"%ls",new_text.sentences[i]->words);
                }
          }
            break;
        case 3:
            break;
        case 4:
            break;
        default:
            puts("");


    }
    */
    ///////////////////////////////////////////////////////////////////////////////////////

    for(int i=0;i<new_text.n;i++){
        wprintf(L"\nstring---->  %d : %ls %d %d\n",i,new_text.sentences[i]->words,new_text.sentences[i]->len,new_text.n);
    }



/*    int *txt= malloc(5*sizeof(int));
    for(int i=0;i<5;i++){
        struct Sentence *sent=readSentence();
        txt[i]=sent->len;
    }
    for(int i=0;i<5;i++){
        printf("%d ",txt[i]);
    }

    wprintf(L"%ls",sent->words);

    free(sent->words);
    free(sent);
    printf("Hello, He!\n");*/
//    for(int i=0;i<text.n;i++){
//        free(text.sentences[i]->words);
//    }
//    free(&text);

    return 0;
}
