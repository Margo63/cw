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

struct Mask{
    wchar_t *masks;
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


struct Mask* task1(struct Text text){
    int len_of_txt=text.n;
    struct Mask *mask= malloc(sizeof (struct Mask)*len_of_txt);

    //wchar_t *list_of_words;
    for(int i=0;i<len_of_txt;i++) {
        int max=0,min;
        int full_len = text.sentences[i]->len;
        wchar_t str[full_len + 2];// = malloc(sizeof(wchar_t) * (full_len + 2));
        wcscpy(str, text.sentences[i]->words);

        wchar_t *tr;
        wchar_t *token = wcstok(str, L" ,.", &tr);

        wchar_t *list_of_words[full_len];// = malloc(full_len * sizeof(wchar_t *));
        int kol = 0;

        while (token != NULL) {
            list_of_words[kol++] = token;
            //if(wcslen(list_of_words[kol])>max)max=wcslen(list_of_words[kol]);
            token = wcstok(NULL, L" ,.", &tr);
        }
        //puts("list of word");
        //for(int y=0;y<kol;y++){
          //  wprintf(L"%ls ",list_of_words[y]);
        //}
        //puts("");

        min= wcslen(list_of_words[0]);
        for(int h=0;h<kol;h++ ){

            if(max < wcslen(list_of_words[h])){
                max= wcslen(list_of_words[h]);

            }
            if(min>wcslen(list_of_words[h])){
                min=wcslen(list_of_words[h]);
            }

        }


        wchar_t *mask_first= malloc(sizeof (wchar_t)*(max+2));//максимальной длины

        if(kol>1){
            int z;
            int min_len;
            if(wcslen(list_of_words[0])<wcslen(list_of_words[1])){
                min_len=wcslen(list_of_words[0]);
            }else{
                min_len=wcslen(list_of_words[1]);
            }

            for(z=0;z< min_len;z++){
                if(list_of_words[0][z]==list_of_words[1][z]){
                    mask_first[z]=list_of_words[0][z];
                }else{
                    mask_first[z]='?';
                }
            }
            if(wcslen(list_of_words[0])!= wcslen(list_of_words[1])){
                mask_first[z]='*';
            }
        }else{
            wcscpy(mask_first,list_of_words[0]);
            mask[i].masks=mask_first;
            continue;
        }

        //wprintf(L"%d fir=%ls\n",i,mask_first);


        for(int q=1;q<kol-1;q++){

            wchar_t* word_now=list_of_words[q];
            wchar_t* word_next=list_of_words[q+1];

//            int min_len;
//
//            if(wcslen(word_now)<wcslen(word_next)){
//                min_len=wcslen(word_now);
//            }else{
//                min_len=wcslen(word_next);
//            }

            //wprintf(L"%d ",min_len);
            int index;

            for(index=0;index< min;index++){
                //wprintf(L"%c %c %c\n",word_now[index],word_next[index],mask_first[index]);
                if((word_now[index]==word_next[index]) && (word_next[index]==mask_first[index]) && (word_now[index]==mask_first[index])){
                    //wprintf(L"равны \n");
//                    mask_first[index]=word_now[index];


                }else{
                    // wprintf(L"ne равны \n");
                    mask_first[index]='?';
                }
            }

            if(wcslen(word_now)!= wcslen(word_next)){
                if(mask_first[index-1]!='*'){
                    mask_first[index]=L'*';
                    mask_first[index+1]=L'\0';
                }else{
                    mask_first[index]= L'\0';
                }

            }
            //wprintf(L"%d %ls\n",i,mask_first);

        }
        mask[i].masks=mask_first;

        //free(str);


        //free(list_of_words);
        //free(mask_first);
    }

    //free(list_of_words);

    return mask;
}



struct Text task2(struct Text text){


    int len_of_txt=text.n;

    for(int i=0;i<len_of_txt;i++){
        int full_len=text.sentences[i]->len;
        wchar_t str[full_len+2];//= malloc(sizeof (wchar_t)*());
        wcscpy(str,text.sentences[i]->words);//копируем полученное предложение в str


        wchar_t *tr;
        wchar_t *token=wcstok(str,L" ,.",&tr);


        wchar_t *list_of_words[full_len];//= malloc(full_len*sizeof (wchar_t*));//массив из слов
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

    }
    text.n=len_of_txt;
    return text;


}

struct Text task3(struct Text text){
    int len_of_txt=text.n;

    wchar_t sym[]=L"aeyuioEUIOAуеёэоаыяиюЮИЯЫАОЭЁЕУ";
    wchar_t alphabet[]=L"qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNMйцукенгшщзхъфывапролджэёячсмитьбюЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЁЯЧСМИТЬБЮ";

    for(int i=0;i<len_of_txt;i++) {
        int full_len = text.sentences[i]->len;
        wchar_t str[full_len +2];// = malloc(sizeof(wchar_t) * (full_len +2));
        wchar_t str_a[full_len+2];
        wcscpy(str, text.sentences[i]->words);
        wcscpy(str_a, text.sentences[i]->words);

        wchar_t *tr;
        wchar_t *token = wcstok(str, L" ,.", &tr);

        wchar_t *list_of_words[full_len];// = malloc(full_len * sizeof(wchar_t *));
        int kol = 0;
        while (token != NULL){
            list_of_words[kol++] = token;
            token = wcstok(NULL, L" ,.", &tr);
        }

        wchar_t *token_a = wcstok(str_a, alphabet, &tr);
        wchar_t *list_of_sp[full_len];// = malloc(full_len * sizeof(wchar_t *));
        int kol_sp = 0;
        while (token_a != NULL){
            list_of_sp[kol_sp++] = token_a;
            token_a = wcstok(NULL, alphabet, &tr);
        }
//            for(int a=0;a<kol_sp;a++){
//                wprintf(L"'%ls'",list_of_sp[a]);
//            }



        wchar_t change[full_len +2];//= malloc(sizeof(wchar_t) * (full_len +2));
        //wcscpy(change,text.sentences[i]->words);
        //wprintf(L"I %ls I",change);

        for(int q=0;q<kol-1;q++){
            for(int w=q;w<kol;w++){
                wchar_t *vowels_a,*vowels_b;



                wchar_t *word_a=list_of_words[q];
                wchar_t *word_b=list_of_words[w];

                // wchar_t arr_vowels_a[wcslen(word_a)];

                vowels_a = wcspbrk (word_a, sym);
                //wprintf(L"%ls:",word_a);
                int kol_a=0,kol_b=0;

                while (vowels_a != NULL)
                {
                    kol_a++;
                    vowels_a = wcspbrk (vowels_a+1,sym);
                }
                //wprintf(L"'%d'\n",kol_a);
                /////////////\\\\\\\\\\\\\\

                vowels_b = wcspbrk (word_b, sym);
                // wprintf(L"%ls:",word_b);
                while (vowels_b != NULL)
                {
                    kol_b++;
                    vowels_b = wcspbrk (vowels_b+1,sym);
                }

                // wprintf(L"'%d'\n",kol_b);

                if(kol_b>kol_a){
                    list_of_words[q]=word_b;
                    list_of_words[w]=word_a;


                    //memmove(&text.sentences[i]->words[],&change[],kol_b)
                }


            }

        }


//            for(int h=0;h<kol;h++){
//                wprintf(L"%ls ",list_of_words[h]);
//            }
////            //wprintf(L"full=%d",full_len);
////
//            puts("\n");
        //wprintf(L"%d %d\n",kol,kol_sp);

        wchar_t change_now[full_len +2];
        if(kol==kol_sp && kol>1){
            wcscpy(change,list_of_words[0]);
            wcscat(change,list_of_sp[0]);

            for(int k=1;k<kol;k++){
                int len_sym= wcslen(list_of_words[k]), len_sep= wcslen(list_of_sp[k]);
                wchar_t pair[len_sym+len_sep+1];
                wcscpy(pair,list_of_words[k]);
                wcscat(pair,list_of_sp[k]);
                //wprintf(L"'%ls'\n",pair);
                wcscat(change,pair);
                //wprintf(L"%d %d\n",len_sym,len_sep);
                //wcscat(change,wcscat(list_of_words[k],list_of_sp[k]));
                //wprintf(L"STR=='%ls'\n",change);
            }

            //wprintf(L"%ls\n",change);
            wcscpy(text.sentences[i]->words,change);

        } else{
            wcscpy(change,list_of_words[0]);
            wcscat(change,list_of_sp[1]);

            for(int k=1;k<kol;k++){
                int len_sym= wcslen(list_of_words[k]), len_sep= wcslen(list_of_sp[k+1]);

                wchar_t pair[len_sym+len_sep+1];

                wcscpy(pair,list_of_words[k]);
                wcscat(pair,list_of_sp[k+1]);
                //wprintf(L"'%ls'\n",pair);
                wcscat(change,pair);
                //wprintf(L"%d %d\n",len_sym,len_sep);
                //wcscat(change,wcscat(list_of_words[k],list_of_sp[k]));
                //wprintf(L"STR=='%ls'\n",change);
            }

            wcscpy(change_now,list_of_sp[0]);
            wcscat(change_now,change);

            //wprintf(L"%ls\n",change_now);
            wcscpy(text.sentences[i]->words,change_now);

        }



    }


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



    for(int i=0;i<len_of_txt;i++) {
        int full_len = text.sentences[i]->len;


        wchar_t *str= malloc(sizeof (wchar_t)*(full_len+2));
        wcscpy(str,text.sentences[i]->words);//копируем полученное предложение в str


        wchar_t *tr;
        wchar_t *token = wcstok(str, L" ,.", &tr);

        wchar_t *list_of_words[full_len];// = malloc(full_len * sizeof(wchar_t *));
        int kol = 0;
        while (token != NULL) {
            list_of_words[kol++] = token;
            token = wcstok(NULL, L" ,.", &tr);
        }


        arr_get[i]= malloc(kol*sizeof (struct Arr));


        // int max=0;
        int kol_arr=0;
        for(int q=0;q<kol;q++){
            int kol_rep=0;
            wchar_t *word_check = list_of_words[q];

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
    }

    return arr_get;
}


