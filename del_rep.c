//
// Created by Маргарита  on 07.12.2021.
//
#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include "del_rep.h"
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

}

