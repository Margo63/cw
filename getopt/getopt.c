#include <stdio.h>
//#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include<string.h>
// -triangle -(x,y) -(x2,y2) -(x3,y3) -full_color -thinkness
// -rectangle -color_find -color_full
// -collage -N -M
// -line -(x,y) -(x2,y2) -thinkness -color
void printHelp(){
    //для рисования линии введите --line и необходимые аргументы: координаты начала и конца --point1 <x,y> --point2 <x,y> толщина линии --thinkness n
    // цвет линии --color_line color (*)
    // для рисования треугольника введите --triangle и необходимые аргументы: координаты точек треугольника --point1 <x,y> --point2 <x,y> --point3 <x,y>
    // цвет заливки --color_full <color> (если треугольник без заливки введите none) --thinkness n цвет линии --color_line color (*)
    // для создания коллажа введите --collage и необходимые аргументы: изображение для коллажа -img name.png; количества по оси X --x_axis и по оси Y --y_axis
    // для находения самого большого треугольника введите  --rectangle и необходимые аргументы: его цвет --color_find <color>; цвет для перекраски --color_full <color>

    printf("для рисования линии введите --line и необходимые аргументы: \nкоординаты начала и конца --point1 <x,y> --point2 <x,y> \nтолщина линии --thinkness <n> \nцвет линии --color_line <color> (*)");
    puts("");
    printf("\nдля рисования треугольника введите --triangle и необходимые аргументы: \nкоординаты точек треугольника --point1 <x,y> --point2 <x,y> --point3 <x,y>"
           "\nцвет заливки --color_full <color> (если треугольник без заливки введите none) \nтолщина линии --thinkness <n> \nцвет линии --color_line color (*)");
    puts("");
    printf("\nдля создания коллажа введите --collage и необходимые аргументы: \nизображение для коллажа -img <name.png> \nколичества по оси X --x_axis и по оси Y --y_axis");
    puts("");
    printf("\nдля находения самого большого треугольника введите  --rectangle и необходимые аргументы: \nего цвет --color_find <color> \nцвет для перекраски --color_full <color>\n");
    puts("");
    //    printf("getopt example\n");
//    printf("-f <value> - final value\n");
//    printf("-r - reverse\n");
//    printf("-v --verbose - verbose\n");
//    printf("-h -? - help\n");
}
struct Point{
    int x;
    int y;
}typedef Point;
struct Configs{
    int final;
    int reverse; // 0 - false
    int verbose; // 0 - false
    char color_line[100];
    int color_find;
    int color_full;
    //int full_color;
    Point point1;
    Point point2;
    Point point3;
    int thinkness;
    int func;
};

int main(int argc, char* argv[]){
    if(argc==1){
        printHelp();
        return 0;
    }

    struct Configs config = {0, 0, 0};
    //char *opts = "f:lutpcrvha";
    char *opts = "ltrcqabpsufxyh";
    struct option longOpts[]={
            {"line", no_argument, NULL, 'l'},
            {"triangle",no_argument,NULL,'t'},
            {"rectangle",no_argument,NULL,'r'},
            {"collage",no_argument,NULL,'c'},

            {"color_line",1,NULL,'q'},
            {"point1",1,NULL,'a'},
            {"point2",1,NULL,'b'},
            {"point3",1,NULL,'p'},
            {"thinkness",1,NULL,'s'},
            {"color_full",1,NULL,'u'},

            {"color_find",1,NULL,'f'},
            {"x_axis",1,NULL,'x'},
            {"y_axis",1,NULL,'y'},
            {"help",no_argument,NULL,'h'},

            { NULL, 0, NULL, 0}
    };
    int opt;
    int longIndex;
    opt = getopt_long(argc, argv, opts, longOpts, &longIndex);
    char point[100];
    char* istr;
    struct Point p;
    while(opt!=-1){
        switch(opt){
            case 'l':
//				printf("get f with value: %d\n", atoi(optarg));
                config.func = 4;
                break;
            case 'a':
                strcpy(point,optarg);
                istr= strtok(point,",\n ");
                p.x=atoi(istr);
                istr=strtok(NULL,",\n ");
                p.y= atoi(istr);
                config.point1=p ;
                //printf("'%d %d'",config.point1.x,config.point1.y);
                break;
            case 'b':
                strcpy(point,optarg);
                istr= strtok(point,",\n ");
                p.x=atoi(istr);
                istr=strtok(NULL,",\n ");
                p.y= atoi(istr);
                config.point2=p ;
                break;
            case 'p':
                strcpy(point,optarg);
                istr= strtok(point,",\n ");
                p.x=atoi(istr);
                istr=strtok(NULL,",\n ");
                p.y= atoi(istr);
                config.point3=p ;
                break;

            case 's':
                config.thinkness= atoi(optarg);
                break;
            case 'q':
                strcpy(config.color_line,optarg);
                break;
            case 'h':

                printHelp();
                return 0;
            case 0:
                printf("->%s\n",longOpts[longIndex].name);
        }
        opt = getopt_long(argc, argv, opts, longOpts, &longIndex);
    }


   // printf("%s\n",argv[1]);
    if(strcmp(argv[1],"--line")==0 || strcmp(argv[1],"--rectangle")==0 || strcmp(argv[1],"--collage")==0 ||strcmp(argv[1],"--triangle")==0||
            strcmp(argv[1],"-l")==0 || strcmp(argv[1],"-r")==0 || strcmp(argv[1],"-c")==0 ||strcmp(argv[1],"-t")==0){
            int kol=optind;
            //line 10
            //
           if(kol==10) printf("p=%d,%d p=%d,%d t=%d c=%s\n",config.point1.x,config.point1.y,config.point2.x,config.point2.y,config.thinkness,config.color_line);

    }else{
        printHelp();
        return 0;
    }

    argc -= optind;
    argv += optind;
    for(int i=0; i<argc; i++)
        printf(">>%s\n", argv[i]);
    return 0;
}
