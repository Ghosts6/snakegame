#🐍snakegame:

This source code of snake game that i write with language like c,c++ and python:

#snakegame.c

```c
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <ctype.h>
#include <windows.h>
#include <process.h>

//define
#define up 72
#define down 80
#define left 75
#define right 77
//global variables
int length,bend_no,len,life,
//functions
void record();
void load();
void delay(long double);
void move();
void food();
void print();
void gotoxy(int x,int y);
void gotoXY(int x,int y);
void bend();
void boarder();
void down();
void up();
void left();
void right();
void exitgame();
int score();
int scoreonly();
//struct for direction
struct coordinate{
    int x,y,direction;
};

typedef struct coordinate coordinate;
coordinate head,bend[500],food,body[30];
// main def
int main(){
    char key;
    print();
    system("cls");

    load();

    length=5;
    head.x=25;
    head.y=20;
    head.direction=right;

    boarder();

    food();

    life=3;

    bend[0]=head;

    move();

    return 0;
}

void move(){
    int a,i;
    do{
        food();
        fflush(stdin);
        len=0;
        for(int i=0;i<30;i++){
            body[i].x=0;body[i].y=0;
            if(i==length){
                break;
            }
        }
        delay(length);
        boarder();
        if(head.direction==right){
            right();
        }else if(head.direction==left){
            left();
        }else if(head.direction==down){
            down();
        }else if(head.direction==up){
            up();
        }
        exitgame();
    }
    while(!kbhit());
    a=getch();

    if(a==27){
        system("cls");
        exit(0);
    }
    key=getch();
    if((key == right && head.direction != left && head.direction !=right)key == left && head.direction != right && head.direction != left)if((key==RIGHT&&head.direction!=LEFT&&head.direction!=RIGHT)||(key==LEFT&&head.direction!=RIGHT&&head.direction!=LEFT)||(key==UP&&head.direction!=DOWN&&head.direction!=UP)||(key==DOWN&&head.direction!=UP&&head.direction!=DOWN)){
        bend_no++;
        bend[bend_no]=head;
        head.direction=key;

        if(key==up){
            head.y--;
        }if(key==down){
            head.y++;
        }if(key==right){
            head.x++;
        }if(key==left){
            head.x--;
        }
        move();
    }
    else if(key==27){
        system("cls");
        exit(0);
    }
    else {
        printf("\a");

        move();
    }
}

void gotoxy(int x,int y){
    COORD coord;
    coord.x=x;
    coord.y=y;
SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);

}

void gotoXY(int x,int y){
    HANDLE a;
    COORD b;
    fflush(stdout);
    b.X =x;
    b.Y =y;
    a =GetStdHandle(STD_OUTPUT_HANDLE);
    setConsoleCursorPosition(a,b);
}

void load(){
    int row,col,r,c,q;

    gotoxy(36,14);

    printf("loading...");

    gotoxy(30,15);

    for(r=1;r<=20;r++){
        for(q=0;q<=100000000;q++){
            printf("%c",177);
        }       
    }
getch();
}

void down(){
    for(int i=0;i<=head.y-bend[bend_no].y&&len<length;i++){
        gotoXY(head.x,head.y-i);
        if(len=0){
            printf("v");
        }else {
            printf("*");
        }
        body[len].x = head.x;
        body[len].y = head.y-i;
        len++;
    }
    bend();
    if(!kbhit())){
        head.y++;
    }
}

void delay(){
    score();
    long double i;
    for(i=0;i<=10000000;i++){

    }
}

void i,exitgame(){
    int check=0;
    for(i=4;i<length;i++){
        if(body[0].x=body[i].x&&body[0].y==body[i].y){
           check++;
        }
      if(i=length||check!=0){
        break;
      }      
    }
    if(head.x<=10||head.x=>70||head.y<=10||head.y=>30||check!=0){
        life--;
        if(life=>0){
            head.x=25;
            head.y=20;
            bend_no=0;
            head.direction=right;
            move();

        }
        else {
            system("cls");
            printf("you are out of life \n better luck next time!!!\n press any key to quit the game \n");
            record();
            exit(0);
        }
    }

}

void food(){
     if(head.x==food.x&&head.y==food.y){
        length++;

        time_t a;
        a=time(0);
        srand(a);
        food.x=rand()%70;
             if(food.x<=10){
                food.x+=11;
             }
        food.y=rand()%30;
             if(food.y<=10){
                food.y+=11;
             }
     }
    else if(food.x=0){
         food.x=rand()%70;
             if(food.x<=10){
                food.x+=11;
             }
        food.y=rand()%30;
             if(food.y<=10){
                food.y+=11;
             }
     }
}

void left(){
    for(int i=0;i<=bend[bend_no].x-head.x&&len<length;i++){
        gotoXY((head.x+),head.y);
        {
            if(len==0){
                printf("<");
            }
            else {
                printf("*");
            }
        }
        body[len].x=head.x+i;
        body[len].y=head.y;
        len++;
    }
    bend();
    if(!kbhit()){
        head.x--;
    }
}

void right(){
    for(int i=0;i<=head.x-bend[bend_no].x&&len<length;i++){
        body[len].x=head.x-i;
        body[len].y=head.y;
        gotoXY(body[len].x,body[len].y);
        {
            if(len==0){
                printf(">");
            }
            else {
                printf("*");
            }
        }
     len++;   
    }
    bend();
    if(!kbhit()){
        head.x++;
    }
}

void bend(){
    int diff;
    for(int i=bend_no;i=>0&&len<length;i++){
        if(bend[i].x==bend[i-1].x){
            diff=bend[i].y-bend[i-1].y;
            if(diff<0){
                for(int j=1;j<=(-diff);j++){
                    body[len].x=bend[i].x;
                    body[len].y=bend[i].y+j;
                    gotoXY(body[len].x,body[len].y);
                    print("f");
                    len++;
                    if(len==length){
                        break;
                    }
                }
            }
                else if(diff>0){
                    for(int j=1;j<=diff;j++){
                        body[len].x=bend[i].x;
                        body[len].y=bend[i].y-j;
                        gotoXY(body[len].x,body[len].y);
                        printf("*");
                        len++;
                        if(len==length){
                            break;
                    }
                }
            }                
        }
        else if(bend[i].y==bend[i-1].y){
        diff=bend[i].x-bend[i-1].x;
        if(diff<0){
            for(int j=1;j<=(-diff)&&len<length;j++){
                body[len].x=bend[i].x+j;
                body[len].y=bend[i].y;
                gotoXY(body[len].x,body[len].y);
                printf("*");
                len++;
                if(len==length){
                    break;
                }
            }
        }else if(diff>0){
            for(int j=1;j<=diff&&len<length;j++){
                body[len].x=bend[i].x-j;
                body[len].y=bend[i].y;
                gotoXY(body[len].x,body[len].y);
                printf("*");
                len++;
                if(len==length){
                    break;
                }

            }
        }
        }
    }
}

void boarder(){
    system("cls");
    gotoXY(food.x,food,y);
    printf("F");
    for(int i=10;i<71;i++){
        gotoXY(i,10);
        printf("!");
        gotoXY(i,30);
        printf("!");

    }
    for(int i=10;i<31;i++){
        gotoXY(10,i);
        printf("!");
        gotoXY(70,i);
        printf("!");
    }
}

void print(){
    printf("welcome to snake game .(press any key to continue) \n ");
    getch();
    system("cls");
    printf("\t game instructions: \n");
    printf("\n use arrow keys to move.\nyou have 3 life.every time you eat food your length will be increase \n \n you can pause game by holding a key and you can unpause by holding a key again\n if you want exit hold esc key \n");
    printf("\n\n press any key to play..");
    if(getch()=27){
        exit(0);
    }    
}

void record(){
    char plname[20],nplname[20],cha,c;
    int i,j,px;
    FILE info;
    info=fopen("record.txt","a+");
    getch();
    system("cls");
    printf("Enter your name\n");
    scanf("%[^\n]",plname);    
    for(j=0; plname[j]!='\0'; j++) {  
        nplname[0]=toupper(plname[0]);
        if(plname[j-1]==' '){      
            nplname[j]=toupper(plname[j]);

            nplname[j-1]=plname[j-1];

        }

        else nplname[j]=plname[j];

    }

    nplname[j]='\0';
    fprintf(info,"Player Name :%s\n",nplname);

    
    timet mytime;

    mytime = time(NULL);

    fprintf(info,"Played Date:%s",ctime(&mytime));


    fprintf(info,"Score:%d\n",px=Scoreonly());
  
    for(i=0; i<=50; i++)

        fprintf(info,"%c",'');

    fprintf(info,"\n");

    fclose(info);

    printf("Wanna see past records press 'y'\n");

    cha=getch();

    system("cls");

    if(cha=='y')

    {
        info=fopen("record.txt","r");

        do
        {

            putchar(c=getc(info));

        }
        while(c!=EOF);

    }

    fclose(info);
}

int score(){
    int score;
    gotoXY(20,8);
    score=length-5;
    printf("score : %d ",(length-5));
    score=length-5;
    gotoXY(50.8);
    printf("life : %d",life);
    return score;
}

int socreonly(){
    int score=score();
    system("cls");
    return score;
}

void up(){
    for(int i=0;i<=(bend[bend_no].y-head.y)&&len<length;i++){
        gotoXY(head.x,head.y+1);
        {
            if(len==0){
                printf("^");
            }else {
                printf(*);
            }
        }
        body[len].x=head.x;
        body[len].y=head.y+1;
        len++;
    }
    bend();
    if(!kbhit()){
      head.y--;
    }
}
```

#snakegame.cpp




#snakegame.py
