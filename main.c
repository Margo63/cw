#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include "del_rep.h"
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
struct Text task2(struct Text text){


    int len_of_txt=text.n;
    struct Text text_now;
    text_now.sentences= malloc(len_of_txt*sizeof (struct Sentence*));

//    for(int q=0;q<len_of_txt;q++){
//        puts("ok");
//        wprintf(L"%ls",text_now.sentences[q]->words);
//
//    }

    for(int q=0;q<len_of_txt;q++){
        //puts("okokokokok");
        text_now.sentences[q]->words=text.sentences[q]->words;

    }
    //text.sentences[len_of_txt-1]->words="change\0";
    //wprintf(L"%ls\n",text.sentences[len_of_txt-1]->words);
    for(int q=0;q<len_of_txt;q++){
       // puts("okqweqweqw");
        wprintf(L"%ls\n",text_now.sentences[q]->words);
        //wprintf(L"%ls",text.sentences[q]->words);

    }
    int new_len=0;
    for(int i=0;i<len_of_txt;i++){

        wchar_t *tr;
        wchar_t *str=text_now.sentences[i]->words;
        int full_len=text_now.sentences[i]->len;

        wchar_t **list_of_words= malloc(full_len*sizeof (wchar_t*));
        wchar_t *token=wcstok(str,L" ,.",&tr);
        int kol_of_words=0;
        //в цикле получаю слова из предложения
        do {
            //wprintf(L"%ls\n", token);
            list_of_words[kol_of_words++]=token;
            token = wcstok(NULL, L" ,.", &tr);

        } while (token!=NULL);
        int kol_up=0;
       // printf("kol of words=%d\n",kol_of_words);
        for(int q=0;q<kol_of_words;q++){
            if(list_of_words[q][0]!= towupper(list_of_words[q][0])){
                //break;
            }else{
                kol_up++;
            }
           // wprintf(L"%ls %d\n", list_of_words[q],kol_up);
        }
        if(kol_up!=kol_of_words){
           // wprintf(L"not rigntT_T->%ls \n", text.sentences[i]->words);

           /* free(text.sentences[len_of_txt]);
            memmove(&text.sentences[i],&text.sentences[i+1],(len_of_txt-i)*sizeof(struct Sentence*));
            len_of_txt--;
            i--;*/
        }else{

        }

    }
    //text_now.n=new_len;

    return text_now;


}


/*
struct Sentence* task2(struct Sentence sent){
    wprintf(L"%ls",sent.words);
    wchar_t *str=sent.words;
    str="change";
    sent.words=str;
    return &sent;
}*/
int main() {

    setlocale(LC_ALL,"");

    struct Text text = readText();
    struct Text new_text = del_rep(text);
   // new_text.sentences[0]= task2(*new_text.sentences[0]);
    new_text= task2(new_text);
    ///////////////////////////////////////////////////////////////////////////////////
    /*
    int operation;
    printf("Для получения маски по каждому предложению введите 1.\nЧтобы удалить все предложения, в которых нет заглавных букв в начале слова введите 2."
          "\nЧтобы отсортировать слова в предложении по количеству гласных букв в слове введите 3."
          "\nЧтобы для каждого предложения получить количество одинаковых слов в строке введите 4."
          "\nЧтобы завершить программу введите любое число.\n--->");
       scanf("%d",&operation);
    switch (operation) {
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        default:
            puts("");


    }*/
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
