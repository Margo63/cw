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


    wchar_t *check_words;
    wchar_t *word_now;
    struct Text new_text=text;
    int len_of_txt=new_text.n;


    for(int i=0;i<len_of_txt;i++){
        check_words=new_text.sentences[i]->words;
        int len_w_check=new_text.sentences[i]->len;
        for(int j=i+1;j<len_of_txt;j++){
            word_now=new_text.sentences[j]->words;
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
            if(check_symb){

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
        wcscpy(str,text.sentences[i]->words);
        //wprintf(L"sent=%ls\ncopy= %ls\n\n",text.sentences[i]->words,str);

        wchar_t *tr;
        wchar_t *token=wcstok(str,L" ,.",&tr);

        wchar_t **list_of_words= malloc(full_len*sizeof (wchar_t*));
        int kol=0;
        do {
            list_of_words[kol++]=token;
            token = wcstok(NULL, L" ,.", &tr);
        } while (token!=NULL);



        int kol_up=0;
        // printf("kol of words=%d\n",kol_of_words);
        for(int q=0;q<kol;q++){
            if(list_of_words[q][0]== towupper(list_of_words[q][0])){
                kol_up++;
            }
            // wprintf(L"%ls %d\n", list_of_words[q],kol_up);
        }
        if(kol_up!=kol){
            // wprintf(L"not rigntT_T->%ls \n", text.sentences[i]->words);

            free(text.sentences[len_of_txt]);
            memmove(&text.sentences[i],&text.sentences[i+1],(len_of_txt-i)*sizeof(struct Sentence*));
            len_of_txt--;
            i--;
        }

        free(list_of_words);

    }
    text.n=len_of_txt;
    return text;


}

