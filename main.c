#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
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
    sentence->len=n;

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

struct Text del_rep(struct Text text){


    wchar_t *check_words;
    wchar_t *word_now;
    struct Text new_text=text;


    //new_text.sentences = malloc(text.n*(sizeof (struct Sentence*)));

    int ch_kol=0;
    //int new_i=0;
    int len_of_txt=new_text.n;


    for(int i=0;i<len_of_txt;i++){
        check_words=new_text.sentences[i]->words;
        //int c_len=new_text.sentences[i]->len;
        ch_kol=0;

        for(int j=i+1;j<len_of_txt;j++){
            word_now=new_text.sentences[j]->words;
           // wprintf(L" сравнение %ls  %ls\n",check_words,word_now);
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
//                for (int q = 0; q < len_of_txt; q++) {
//
//                    wprintf(L" %ls   ",text.sentences[q]->words);
//
//                }
                ch_kol++;
            }

            //puts("");
            //printf("\n%d %d\n",len_of_txt,j);
        }

        if(ch_kol==0){
            //wprintf(L"%ls \n",check_words);
            //new_text.sentences[new_i]=check_words;
            //new_i++;


        }
    }

    new_text.n=len_of_txt;

    return new_text;

}

int main() {

    setlocale(LC_ALL,"");

    struct Text text = readText();
    struct Text new_text = del_rep(text);


//    for(int i=0;i<new_text.n;i++){
//        wprintf(L"%ls \n",new_text.sentences[i]->words);
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
    return 0;
}
