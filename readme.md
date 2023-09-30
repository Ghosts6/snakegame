this is my source code of snake game which i create with help of c lnagugae and some common library
```c
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
