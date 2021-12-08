//
// Created by Маргарита  on 07.12.2021.
//
//#include <stdio.h>
//#include <wchar.h>
//#include <stdlib.h>
//#include <locale.h>
//
//
//struct Text del_rep(struct Text text){
//
//    wchar_t *check_words;
//    wchar_t *word_now;
//    struct Text new_text;
//    new_text.sentences = malloc(text.n*(sizeof (struct Sentence*)));
//
//    int ch_kol=0;
//    int new_i=0;
//    for(int i=0;i<text.n;i++){
//        check_words=text.sentences[i]->words;
//        int c_len=text.sentences[i]->len;
//        ch_kol=0;
//
//        for(int j=i;j<text.n;j++){
//            word_now=text.sentences[j]->words;
//            int len=text.sentences[j]->len;
//            int check_symb=0;
//            //if(len==c_len)
//            for(int k=0;k<len;k++){
//                if(towupper(word_now[k])== towupper(check_words[k])){
//                    check_symb=1;
//                }else{
//                    check_symb=0;
//                    break;
//                }
//            }
//            if(check_symb){
//                ch_kol++;
//            }
//
//
//
//        }
//        //wprintf(L"%ls>>> \n",check_words);
//        if(ch_kol==1){
//            //wprintf(L"%ls \n",check_words);
//            new_text.sentences[new_i]=check_words;
//            new_i++;
//
//
//        }
//    }
//
//    new_text.n=new_i;
//
//    return new_text;
//}



