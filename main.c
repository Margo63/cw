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

//struct Text task3(struct Text text){
//    int len_of_txt=text.n;
//    wchar_t sym[]=L"aeyuioEUIOAуеёэоаыяиюЮИЯЫАОЭЁЕУ";
//
//    for(int i=0;i<len_of_txt;i++) {
//        int full_len = text.sentences[i]->len;
//        wchar_t* str = malloc(sizeof(wchar_t) * (full_len +2));
//        wcscpy(str, text.sentences[i]->words);
//
//        wchar_t *tr;
//        wchar_t *token = wcstok(str, L" .", &tr);
//
//        wchar_t **list_of_words = malloc(full_len * sizeof(wchar_t *));
//        int kol = 0;
//        while (token != NULL){
//            list_of_words[kol++] = token;
//            token = wcstok(NULL, L" .", &tr);
//        }
//        int lenght=0;
//
//
//        wchar_t *change= malloc(sizeof(wchar_t) * (full_len +2));
//        wcscpy(change,text.sentences[i]->words);
//        //wprintf(L"I %ls I",change);
//
//        for(int q=0;q<kol-1;q++){
//            wchar_t *vowels_a,*vowels_b;
//
//
//
//            wchar_t *word_a=list_of_words[q];
//            wchar_t *word_b=list_of_words[q+1];
//
//            // wchar_t arr_vowels_a[wcslen(word_a)];
//
//            vowels_a = wcspbrk (word_a, sym);
//            //wprintf(L"%ls:",word_a);
//            int kol_a=0,kol_b=0;
//
//            while (vowels_a != NULL)
//            {
//                kol_a++;
//                vowels_a = wcspbrk (vowels_a+1,sym);
//            }
//
//            wprintf(L"'%d'\n",kol_a);
///////////////\\\\\\\\\\\\\\
//
//            vowels_b = wcspbrk (word_b, sym);
//            // wprintf(L"%ls:",word_b);
//            while (vowels_b != NULL)
//            {
//                kol_b++;
//                vowels_b = wcspbrk (vowels_b+1,sym);
//            }
//
//            wprintf(L"'%d'\n",kol_b);
//
//            if(kol_b<kol_a){
//                list_of_words[q]=word_b;
//                list_of_words[q+1]=word_a;
//                if(word_a[kol_a]==','){
//                    puts('ok');
//                }
//
//                //memmove(&text.sentences[i]->words[],&change[],kol_b)
//            }
//
//
//        }
//
//
//       for(int h=0;h<kol;h++){
//            wprintf(L"%ls ",list_of_words[h]);
//        }
//        //wprintf(L"full=%d",full_len);
//
//        puts("\n");
//    }
//
//
//    return text;
//}

//struct Text task3(struct Text text){
//    int len_of_txt=text.n;
//    wchar_t sym[]=L"aeyuioEUIOAуеёэоаыяиюЮИЯЫАОЭЁЕУ";
//    wchar_t alp[]=L"qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNMйцукенгшщзхъёэждлорпавыфячсмитьбюЮБЬТИМСЧЯФЫВАПРОЛДЖЭЁЪХЗЩШГНЕКУЦЙ";
//
//
//    for(int i=0;i<len_of_txt;i++) {
//        int full_len = text.sentences[i]->len;
//        wchar_t str[full_len +2];// = malloc(sizeof(wchar_t) * ());
//        wchar_t new_str[full_len +2];// = malloc(sizeof(wchar_t) * (full_len +2));
//        wchar_t str_ras[full_len +2];
//        ////
//
//        wcscpy(str, text.sentences[i]->words);
//        wcscpy(str_ras, text.sentences[i]->words);
//
//        wchar_t *tr;
//        wchar_t *token = wcstok(str, L" ,.", &tr);
//
//        wchar_t *list_of_words[full_len];// = malloc(full_len * sizeof(wchar_t *));
//        //wchar_t *list_of_words2[full_len];// = malloc(full_len * sizeof(wchar_t *));
//
//        int kol = 0;
//        while (token != NULL){
//            //list_of_words[kol]= malloc((wcslen(token)+3)*sizeof(wchar_t));
//            //list_of_words2[kol]= malloc((wcslen(token)+1)*sizeof(wchar_t));
//
//            list_of_words[kol++] = token;
//
//            token = wcstok(NULL, L" ,.", &tr);
//        }
//
//        int kol_al=0;
//        wchar_t *token_a = wcstok(str_ras, alp, &tr);
//        while (token_a !=NULL){
//
//        }
//
//     /*   for(int y=0;y<kol;y++){
//            wchar_t *word = list_of_words[y];
//            wchar_t *word2=malloc((wcslen(word)+2)*sizeof (wchar_t));
//            wcscpy(word2,word);
//
//            word2[wcslen(word)]=' ';
//            word2[wcslen(word)+1]='\0';
//            //wprintf(L"'%ls'\n",word2);
//            list_of_words2[y]=word2;
//        }*/
//
////        puts("sec");
////
////        for(int y=0;y<kol;y++){
////            wchar_t *word = list_of_words2[y];
////            wprintf(L"'%ls'\n",word);
////
////
////        }
//        for(int q=0;q<kol-1;q++){
//            for(int w=q+1;w<kol;w++){
//                wchar_t *vowels_a,*vowels_b;
//
//
//                //wchar_t *w_a=list_of_words2[q];
//                //wchar_t *w_b=list_of_words2[w];
//
//                //int len_a= wcslen(w_a);
//                //int len_b= wcslen(w_b);
//
//                wchar_t *word_a= list_of_words[q];//malloc((len_a+3)*sizeof (wchar_t));
//                wchar_t *word_b= list_of_words[w];//malloc((len_b+3)*sizeof (wchar_t));
//                //wchar_t *word_b=list_of_words[w];
//                //wcscpy(word_a,w_a);
//                //wcscpy(word_b,w_b);
//
//                //wprintf(L"word_a=%ls word_b=%ls\n",word_a,word_b);
//
////                word_a[len_a]=' ';
////                word_a[len_a+1]='\0';
////                word_b[len_b]=' ';
////                word_b[len_b+1]='\0';
////
////
////
////
////                wprintf(L"a='%ls' b='%ls'\n",word_a,word_b);
////
//
//
//                // wchar_t arr_vowels_a[wcslen(word_a)];
//
//                vowels_a = wcspbrk (word_a, sym);
//                //wprintf(L"%ls:",word_a);
//                int kol_a=0,kol_b=0;
//
//                while (vowels_a != NULL)
//                {
//                    kol_a++;
//                    vowels_a = wcspbrk (vowels_a+1,sym);
//                }
//
//                //wprintf(L"'%d'\n",kol_a);
///////////////\\\\\\\\\\\\\\
//
//                vowels_b = wcspbrk (word_b, sym);
//                // wprintf(L"%ls:",word_b);
//                while (vowels_b != NULL)
//                {
//                    kol_b++;
//                    vowels_b = wcspbrk (vowels_b+1,sym);
//                }
//
//                // wprintf(L"'%d'\n",kol_b);
//
//
//
//                if(kol_b>kol_a){
//
//
//                    list_of_words[q]=word_b;
//                    list_of_words[w]=word_a;
//
//
//
//                    //memmove(&text.sentences[i]->words[],&change[],kol_b)
//                }
//
//
//            }
//
//        }
//
//        /*for(int h=0;h<kol;h++){
//           // wprintf(L"%ls",list_of_words2[h]);
//
//            wcscat(new_str,list_of_words2[h]);
//
//
//        }*/
//
//
//        //puts(" \n");
//
//        //new_str[full_len]='.';
//        for(int qw=0;qw<kol;qw++){
//            wprintf(L"%ls",list_of_words[qw]);
//        }
//
//        text.sentences[i]->words=new_str;
//
//
//    }
//
//
//    return text;
//}

struct Text task3(struct Text text){
    int len_of_txt=text.n;

        wchar_t sym[]=L"aeyuioEUIOAуеёэоаыяиюЮИЯЫАОЭЁЕУ";

        for(int i=0;i<len_of_txt;i++) {
            int full_len = text.sentences[i]->len;
            wchar_t* str = malloc(sizeof(wchar_t) * (full_len +2));
            wcscpy(str, text.sentences[i]->words);

            wchar_t *tr;
            wchar_t *token = wcstok(str, L" ,.", &tr);

            wchar_t **list_of_words = malloc(full_len * sizeof(wchar_t *));
            int kol = 0;
            while (token != NULL){
                list_of_words[kol++] = token;
                token = wcstok(NULL, L" ,.", &tr);
            }
            int lenght=0;


            wchar_t *change= malloc(sizeof(wchar_t) * (full_len +2));
            wcscpy(change,text.sentences[i]->words);
            //wprintf(L"I %ls I",change);

            for(int q=0;q<kol-1;q++){
                wchar_t *vowels_a,*vowels_b;



                wchar_t *word_a=list_of_words[q];
                wchar_t *word_b=list_of_words[q+1];

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
                    list_of_words[q+1]=word_a;


                    //memmove(&text.sentences[i]->words[],&change[],kol_b)
                }


            }


            for(int h=0;h<kol;h++){
                wprintf(L"%ls ",list_of_words[h]);
            }
            //wprintf(L"full=%d",full_len);

            puts("\n");
        }


        return text;
    }




    struct Mask{
    wchar_t *masks;
};



int main() {

    setlocale(LC_ALL,"");

    struct Text new_text = readText();
    /*struct Text */
    new_text = del_rep(new_text);

    for(int i=0;i<new_text.n;i++){
        wprintf(L"\nstring---->  %d : %ls %d %d\n",i,new_text.sentences[i]->words,new_text.sentences[i]->len,new_text.n);
    }



    ///////////////////////////////////////////////////////////////////////////////////

    int k=1;
while(k){
    int operation;
    printf("Для получения маски по каждому предложению введите 1.\nЧтобы удалить все предложения, в которых нет заглавных букв в начале слова введите 2."
           "\nЧтобы отсортировать слова в предложении по количеству гласных букв в слове введите 3."
           "\nЧтобы для каждого предложения получить количество одинаковых слов в строке введите 4."
           "\nЧтобы завершить программу введите 5.\n--->");
    scanf("%d",&operation);
    switch (operation) {
        case 1:{
            struct Mask *mask=task1(new_text);
            for(int i=0;i<new_text.n;i++){
                wprintf(L"%d: mask= %ls\n",i,mask[i].masks);
                }

            free(mask);
            }
            break;
        case 2:{
            new_text= task2(new_text);
            for(int i=0;i<new_text.n;i++){
                wprintf(L"%ls",new_text.sentences[i]->words);
            }
            puts("");
        }
            break;
        case 3:{

            new_text= task3(new_text);
            for(int i=0;i<new_text.n;i++){
                wprintf(L"%ls",new_text.sentences[i]->words);
            }
            puts("");
        }
            break;
        case 4:{
            struct Arr **arr= task4(new_text);

            for(int i=0;i<new_text.n;i++){
                int len=arr[i][0].len_rep;
                //printf("%d",len);
                for(int z=0;z<len;z++){
                    //printf("s ");
                    wprintf(L"'%ls':%d; ",arr[i][z].word,arr[i][z].kol_repeat);
                }
                puts("");



            }
            for(int f=0;f<new_text.n;f++){
                free(arr[f]);
            }
            free(arr);

        }
            break;
        case 5:{
            k=0;
        }break;
        default:
            puts("");


    }
}

    for(int i=0;i<new_text.n;i++){
        wprintf(L"\nstring---->  %d : %ls %d %d\n",i,new_text.sentences[i]->words,new_text.sentences[i]->len,new_text.n);
    }
    //wprintf(L"%d, %d",text.n,new_text.n);


//   for(int fr=0;fr<new_text.n;fr++){
//        free(new_text.sentences[fr]->words);
//    }
//    free(&new_text);



    for(int i=0;i<new_text.n;i++){
        free(new_text.sentences[i]->words);
    }
    for(int i=0;i<new_text.n;i++){
        free(new_text.sentences[i]);
    }







    ///////////////////////////////////////////////////////////////////////////////////////



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
