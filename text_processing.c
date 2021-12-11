//
// Created by Маргарита  on 07.12.2021.
//
#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include "text_processing.h"
struct Sentence{
    wchar_t *words;
    int size;
    int len;

};
struct Text{
    struct Sentence **sentences;
    int size;
    int n;
};

struct Text del_rep(struct Text text){


    wchar_t *check_words;//строчки для сравнения
    wchar_t *word_now;
    struct Text new_text=text;
    int len_of_txt=new_text.n;


    for(int i=0;i<len_of_txt;i++){
        check_words=new_text.sentences[i]->words;//check_words пробегается по всем предложениям
        int len_w_check=new_text.sentences[i]->len;
        for(int j=i+1;j<len_of_txt;j++){
            word_now=new_text.sentences[j]->words;//текущее предложение
            int len_w_now=new_text.sentences[j]->len;
            int len=new_text.sentences[j]->len;
            int check_symb=0;
            ////посимвольное сравнение
            if(len_w_check==len_w_now) {
                for (int k = 0; k < len; k++) {
                    if (towupper(word_now[k]) == towupper(check_words[k])) {

                        check_symb = 1;
                    } else {
                        check_symb = 0;
                        break;
                    }
                }
            }
            ////
            if(check_symb){//если предложения совпали сдвигаем массив так, чтобы удалить повторяющееся предложение

                //wprintf(L"%ls %ls\n",word_now,check_words);
                free(new_text.sentences[len_of_txt]);
                memmove(&new_text.sentences[j],&new_text.sentences[j+1],(len_of_txt-j)*sizeof(struct Sentence*));
                len_of_txt--;
                j--;

            }


        }

    }

    new_text.n=len_of_txt;

    return new_text;
}

struct Text task2(struct Text text){


    int len_of_txt=text.n;

    for(int i=0;i<len_of_txt;i++){
        int full_len=text.sentences[i]->len;
        wchar_t *str= malloc(sizeof (wchar_t)*(full_len+2));
        wcscpy(str,text.sentences[i]->words);//копируем полученное предложение в str


        wchar_t *tr;
        wchar_t *token=wcstok(str,L" ,.",&tr);

        free(str);
        wchar_t **list_of_words= malloc(full_len*sizeof (wchar_t*));//массив из слов
        int kol=0;
        do {
            list_of_words[kol++]=token;
            token = wcstok(NULL, L" ,.", &tr);
        } while (token!=NULL);



        int kol_up=0;

        for(int q=0;q<kol;q++){
            if(list_of_words[q][0]== towupper(list_of_words[q][0])){
                kol_up++;
            }

        }
        if(kol_up!=kol){//если не все слова с заглавной буквы

            free(text.sentences[len_of_txt]);
            memmove(&text.sentences[i],&text.sentences[i+1],(len_of_txt-i)*sizeof(struct Sentence*));
            len_of_txt--;
            i--;
        }
        for(int f=0;f<kol;f++){
           // free(list_of_words[f]);
        }
        free(list_of_words);

    }
    text.n=len_of_txt;
    return text;


}

struct Arr{
    int kol_repeat;
    wchar_t* word;
    int len_rep;
};

struct Arr** task4 (struct Text text){
    int len_of_txt=text.n;
    struct Arr** arr_get= malloc(len_of_txt*sizeof (struct Arr*));
//    for(int s=0;s<len_of_txt;s++){
//        arr_get[s]= malloc(20*sizeof (struct Arr));//решить проблему с памятью
//    }



    for(int i=0;i<len_of_txt;i++) {
        int full_len = text.sentences[i]->len;
        wchar_t *str = malloc(sizeof(wchar_t) * (full_len + 2));
        wcscpy(str, text.sentences[i]->words);
        //wprintf(L"sent=%ls\ncopy= %ls\n\n",text.sentences[i]->words,str);

        wchar_t *tr;
        wchar_t *token = wcstok(str, L" ,.", &tr);

        wchar_t **list_of_words = malloc(full_len * sizeof(wchar_t *));
        int kol = 0;
        do {
            list_of_words[kol++] = token;
            token = wcstok(NULL, L" ,.", &tr);
        } while (token != NULL);

        arr_get[i]= malloc(kol*sizeof (struct Arr));


        // int max=0;
        int kol_arr=0;
        for(int q=0;q<kol;q++){
            int kol_rep=0;
            wchar_t *word_check=list_of_words[q];

            for(int w=q+1;w<kol;w++){
                //  wprintf(L"%d:%ls  %d:%ls ",q,list_of_words[q],w,list_of_words[w]);



                if(wcscmp(word_check,list_of_words[w])==0){
                    kol_rep++;
                    memmove(&list_of_words[w],&list_of_words[w+1],(kol-w)*sizeof (wchar_t*));
                    w--;
                    kol--;
                }

            }

            // wprintf(L"%d %d %ls %d \n",i,kol_arr,word_now,kol_rep+1);

            arr_get[i][kol_arr].word=word_check;
            arr_get[i][kol_arr].kol_repeat=kol_rep;
            //wprintf(L"%d %d '%ls' ",i,kol_arr,arr_get[0][0].word);
            kol_arr++;






        }
        arr_get[i][0].len_rep=kol;
        //  wprintf(L"%ls\n",arr_get[0][0].word);
//        for(int y=0;y<kol;y++){
//            wprintf(L" %d: %ls \n",arr_get[i][y].kol_repeat,arr_get[i][y].word);
//        }

    }
    //puts("//////////////////////////////////////");
    /*
        for(int i=0;i<len_of_txt;i++){
            int len=arr_get[i][0].len_rep;
            //printf("%d",len);
            for(int z=0;z<len;z++){
                //printf("s ");
                wprintf(L"%d %ls:%d; ",i,arr_get[i][z].word,arr_get[i][z].kol_repeat);
            }
            puts("");

        }

    */
    return arr_get;
}


