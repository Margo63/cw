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

int main() {

    setlocale(LC_ALL,"");

    struct Text text = readText();
    struct Text new_text = del_rep(text);


    //task4(text);
    //arr = task4(text);


    ///////////////////////////////////////////////////////////////////////////////////

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
        case 4:{
            struct Arr **arr= task4(text);

            for(int i=0;i<new_text.n;i++){
                int len=arr[i][0].len_rep;
                //printf("%d",len);
                for(int z=0;z<len;z++){
                    //printf("s ");
                    wprintf(L"%ls:%d; ",arr[i][z].word,arr[i][z].kol_repeat);
                }
                puts("");

            }

        }
            break;
        default:
            puts("");


    }

    ///////////////////////////////////////////////////////////////////////////////////////

    for(int i=0;i<new_text.n;i++){
        wprintf(L"\nstring---->  %d : %ls %d %d\n",i,new_text.sentences[i]->words,new_text.sentences[i]->len,new_text.n);
    }

//    for(int i=0;i<new_text.n;i++){
//        printf("'%d' ",arr[i]);
//    }



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
