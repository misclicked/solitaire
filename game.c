#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<windows.h>
#include<conio.h>

#define BLACK 0,7
#define RED 4,7
#define YELLO 14,0
#define RESET 7,0

#define CARDS 52
#define FACES 13

//#define test
#define org
typedef struct{
	int x;
	int y;
}pos;
typedef struct{
	int face;	//牌點 
	int suit;	//花色 
}Card;
void setcolor(int textcolor,int bgcolor);
void hidesomething();
void showsomething();
struct stacknode{
	Card wCard;
	struct stacknode *next;
};
typedef struct{
	int from,to,n,hidden;
}Step;
struct stepnode{
	Step myStep;
	struct stepnode *next;
};
enum game{ini=0,F1,F2,F3,F4,F5,F6,F7,P1,P2,P3,P4,FD,FDD};
typedef struct stacknode stackNode;
typedef	stackNode *stackNodePtr;
typedef struct stepnode stepNode;
typedef stepNode *stepNodePtr;
void gotoxy(int xpos, int ypos);
void pushstep(stepNodePtr *topPtr,Step myStep);
Step popstep(stepNodePtr *topPtr);
int isEmptystep(stepNodePtr topPtr);
void push(stackNodePtr *topPtr,Card wCard);
Card pop(stackNodePtr *topPtr);
int stackSize(stackNodePtr currentPtr);
void printStack(stackNodePtr currentPtr);
int isEmpty(stackNodePtr topPtr);
void fillDeck(Card* const wDeck);
void shuffle(Card* const wDeck,int n);
void iniDeck(stackNodePtr *topPtr,Card* const wDeck);
void movesDeck(stackNodePtr *fromPtr,stackNodePtr *toPtr,int n,stackNodePtr *recycle);
void buildfield(stackNodePtr *iniPtr,int hidden[]);
void alwayswin(stackNodePtr *iniPtr,int hidden[],Card * const wDeck);
void printfield(stackNodePtr *iniPtr,int hidden[],int Color,char Cardback);
void printsDeck(stackNodePtr topPtr,int n);
int checkwin(stackNodePtr *Deck,int hidden[]);
Card nCard(stackNodePtr topPtr,int n);
int valid(stackNodePtr *fromPtr,stackNodePtr *toPtr, int n,int to);
void bubblesort(int *a,int n);
int maxstacksize(stackNodePtr *currentPtr);
int main(){
	srand(time(0));
	FILE *fw;
	char choise;
	char name[50],pass[50],cmp[50],score[50];
	int userscore[50];
	int scorecnt,j;
	int same;
	fw=fopen("userdata","a+");
	if(!fw){
		printf("fail\n");
		return 0;
	}
	printf("New user?(Y/N)");scanf("%c",&choise);getchar();
	switch(choise){
		case 'Y':
		case 'y':
			same=1;
			while(same){
				printf("Enter new username: ");
				fgets(name,50,stdin);
				same=0;
				while(fgets(cmp,50,fw)!=NULL){
					if(strcmp(cmp,"Username:\n")==0){
						fgets(cmp,50,fw);
						if(strcmp(name,cmp)==0){
							same=1;
							break;
						}
					}
				}
				if(same){
					printf("Username existed\n");
					rewind(fw);
					system("pause");
					system("cls");
				}else{
					printf("Enter a password: ");
					fgets(pass,50,stdin);
					same=0;
					fprintf(fw,"Username:\n");
					fprintf(fw,name);
					fprintf(fw,pass);
					fprintf(fw,"\n");
				}
			}
			break;
		case 'N':
		case 'n':
			printf("Username: ");
			fgets(name,50,stdin);
			same=0;
			while(fgets(cmp,50,fw)!=NULL){
				if(strcmp(cmp,"Username:\n")==0){
					fgets(cmp,50,fw);
					if(strcmp(name,cmp)==0){
						same=1;
						break;
					}
				}
			}
			if(same){
				printf("Password: ");
				fgets(pass,50,stdin);
				fgets(cmp,50,fw);
				if(strcmp(cmp,pass)==0){
					printf("Welcome %s",name);
				}else{
					printf("密碼錯誤\n");
					system("pause");
					fclose(fw);
					return 0;
				}
			}else{
				printf("使用者不存在\n");
				system("pause");
				fclose(fw);
				return 0;
			}
			break;
	}
	printf("開始遊戲:1 顯示歷史分數:2\t");scanf("%c",&choise);getchar();
	if(choise=='2'){
		scorecnt=0;
		memset(userscore,0,sizeof(userscore));
		rewind(fw);
		while(fgets(cmp,50,fw)!=NULL){
			if(strcmp(cmp,"Score:\n")==0){
				fgets(cmp,50,fw);
				if(strcmp(name,cmp)==0){
					fgets(score,50,fw);
					userscore[scorecnt++]=atoi(score);
				}
			}
		}
		bubblesort(userscore,scorecnt);
		for(j=1;j<=10;j++){
			if(j>scorecnt){
				printf("%d.\t0\n",j);
			}else
				printf("%d.\t%d\n",j,userscore[j-1]);
		}
		system("pause");
	}
	hidesomething();
	Card deck[CARDS];
	int hf;
	int hidden[8]={0};
	char d;
	int n,from,to,i,z,t1;
	char c;
	float timeused;
	int Color=rand()%6+1;
	char Cardback=rand()%(48-14)+14;
	gamestart:
	system("cls");
	stackNodePtr sDeck[14]={NULL};
	stepNodePtr sStep=NULL;
	Step now,back;
	z=0;
	t1=clock();
	fillDeck(deck);
	#ifdef org
	shuffle(deck,CARDS);
	//ini-deck 0 playing field 1~7 poped field 8~11 flip-deck 12 fliped-deck 13
	iniDeck(sDeck+ini,deck);
	buildfield(sDeck,hidden);
	#endif
	#ifdef test
	alwayswin(sDeck,hidden,deck);
	#endif
	printfield(sDeck,hidden,Color,Cardback);
	int x=0,y=0;
	int prex=0,prey=0;
	gotoxy(x,y);
	putchar(16);
	gotoxy(x+5,y);
	putchar(17);
	int loaded=0,loadx,loady;
	int Move=0;
	while(1){
		if(kbhit()){
			c=getch();
			switch((int)c){
				case 72:
					if(y-1<0)
						y=maxstacksize(sDeck)+2;
					if(!loaded||loaded&&!(prex==loadx&&prey==loady)){
						gotoxy(prex,prey);
						putchar(' ');
						gotoxy(prex+5,prey);
						putchar(' ');
					}
					if(x==loadx&&y-1==loady&&loaded)
						setcolor(YELLO);
					gotoxy(x,y-1);
					prex=x;prey=y-1;
					y--;
					putchar(16);
					gotoxy(x+5,y);
					putchar(17);
					setcolor(RESET);
					break;
				case 80:
					if((y+1)>=2+maxstacksize(sDeck))
						y=-1;
					if(!loaded||loaded&&!(prex==loadx&&prey==loady)){
						gotoxy(prex,prey);
						putchar(' ');
						gotoxy(prex+5,prey);
						putchar(' ');
					}
					if(x==loadx&&y+1==loady&&loaded)
						setcolor(YELLO);
					gotoxy(x,y+1);
					prex=x;prey=y+1;
					y++;
					putchar(16);
					gotoxy(x+5,y);
					putchar(17);
					setcolor(RESET);
					break;
				case 75:
					if(x-6<0)
						x=42;
					if(!loaded||loaded&&!(prex==loadx&&prey==loady)){
						gotoxy(prex,prey);
						putchar(' ');
						gotoxy(prex+5,prey);
						putchar(' ');
					}
					if(x-6==loadx&&y==loady&&loaded)
						setcolor(YELLO);
					gotoxy(x-6,y);
					prex=x-6;prey=y;
					x-=6;
					putchar(16);
					gotoxy(x+5,y);
					putchar(17);
					setcolor(RESET);
					break;
				case 77:
					if(x+6>36)
						x=-6;
					if(!loaded||loaded&&!(prex==loadx&&prey==loady)){
						gotoxy(prex,prey);
						putchar(' ');
						gotoxy(prex+5,prey);
						putchar(' ');
					}
					if(x+6==loadx&&y==loady&&loaded)
						setcolor(YELLO);
					gotoxy(x+6,y);
					prex=x+6;prey=y;
					x+=6;
					putchar(16);
					gotoxy(x+5,y);
					putchar(17);
					setcolor(RESET);
					break;
				case ' ':
					if(!loaded){
						if(prex==12&&prey==0){
							system("cls");
							printf("Sure?(Y/N):");
							scanf("%c",&d);
							getchar();
							if(d=='Y'){
								fclose(fw);
								return 0;
							}else{
								system("cls");
								printfield(sDeck,hidden,Color,Cardback);
								gotoxy(prex,prey);
								putchar(16);
								gotoxy(prex+5,prey);
								putchar(17);
								continue;
							}
						}
						if(prex==0&&prey==0){
							if(!isEmpty(*(sDeck+FD))){
								movesDeck(sDeck+FD,sDeck+FDD,1,sDeck+ini);
								now.from=FD;now.to=FDD;now.n=1;now.hidden=0;
								pushstep(&sStep,now);
							}
							else{
								int Now=stackSize(*(sDeck+FDD));
								now.from=FDD;now.to=FD;now.n=Now;now.hidden=0;
								pushstep(&sStep,now);
								while(Now--)
									push(sDeck+FD,pop(sDeck+FDD));
							}
							gotoxy(0,0);
							system("cls");
							printfield(sDeck,hidden,Color,Cardback);
							gotoxy(prex,prey);
							putchar(16);
							gotoxy(prex+5,prey);
							putchar(17);
						}else{
							if(hidden[prex/6+1]<0)
								hf=0;
							else
								hf=hidden[prex/6+1];
							if(prey>=1+hf&&prey<=stackSize(*(sDeck+(prex/6+1)))){
								gotoxy(prex,prey);
								setcolor(YELLO);
								putchar(16);
								gotoxy(prex+5,prey);
								putchar(17);
								setcolor(RESET);
								loaded=1;loadx=prex;loady=prey;
								from=prex/6+1;n=stackSize(*(sDeck+(prex/6+1)))-prey+1;
							}else if(prey==0&&(prex>=18&&prex<=36||prex==6&&!isEmpty(*(sDeck+FDD)))){
								gotoxy(prex,prey);
								setcolor(YELLO);
								putchar(16);
								gotoxy(prex+5,prey);
								putchar(17);
								setcolor(RESET);
								loaded=1;loadx=prex;loady=prey;
								if(prex/6==1)
									from=FDD,n=1;
								else
									from=P1+(prex-18)/6,n=1;
							}
						}
					}else{
						if(prey==stackSize(*(sDeck+(prex/6+1)))+1)
							to=prex/6+1,Move=1;
						else if(prex>=18&&prex<=36&&prey==0)
							to=P1+(prex-18)/6,Move=1;
						if(Move){
							Move=0;
							if(valid(sDeck+from,sDeck+to,n,to)){
								if(from>=1&&from<=7){
									if(stackSize(*(sDeck+from))-n==hidden[from]){
										hidden[from]--;
										movesDeck(sDeck+from,sDeck+to,n,sDeck+ini);
										now.from=from;now.to=to;now.n=n;now.hidden=1;
										pushstep(&sStep,now);
									}else if(stackSize(*(sDeck+from))-n>hidden[from]){
										movesDeck(sDeck+from,sDeck+to,n,sDeck+ini);
										now.from=from;now.to=to;now.n=n;now.hidden=0;										
										pushstep(&sStep,now);
									}
								}
								else{
									movesDeck(sDeck+from,sDeck+to,n,sDeck+ini);
									now.from=from;now.to=to;now.n=n;now.hidden=0;
									pushstep(&sStep,now);
								}
							}
							gotoxy(0,0);
							system("cls");
							printfield(sDeck,hidden,Color,Cardback);
						}
						loaded=0;
						gotoxy(loadx,loady);
						putchar(' ');
						gotoxy(loadx+5,loady);
						putchar(' ');
						gotoxy(x,y);
						putchar(16);
						gotoxy(x+5,y);
						putchar(17);
					}
					break;
				case 'z':
					if(!loaded){
						if(!isEmptystep(sStep)){
							back=popstep(&sStep);
							if(back.from!=FDD){
								if(back.from>=F1&&back.from<=F7&&now.hidden)
									hidden[back.from]++;
								movesDeck(sDeck+back.to,sDeck+back.from,back.n,sDeck+ini);
							}else{
								while(back.n--)
									push(sDeck+FDD,pop(sDeck+FD));
							}
						}
						gotoxy(0,0);
						printfield(sDeck,hidden,Color,Cardback);
						gotoxy(x,y);
						putchar(16);
						gotoxy(x+5,y);
						putchar(17);
						z++;
					}
					break;
					case 'f':
						if(!isEmpty(*(sDeck+FD))){
							movesDeck(sDeck+FD,sDeck+FDD,1,sDeck+ini);
							now.from=FD;now.to=FDD;now.n=1;now.hidden=0;
							pushstep(&sStep,now);
						}
						else{
							int Now=stackSize(*(sDeck+FDD));
							now.from=FDD;now.to=FD;now.n=Now;now.hidden=0;
							pushstep(&sStep,now);
							while(Now--)
								push(sDeck+FD,pop(sDeck+FDD));
						}
						gotoxy(0,0);
						system("cls");
						printfield(sDeck,hidden,Color,Cardback);
						gotoxy(prex,prey);
						putchar(16);
						gotoxy(prex+5,prey);
						putchar(17);
						break;
					case 'n':
						goto gamestart;
					case 'r':
						while(!isEmptystep(sStep)){
							back=popstep(&sStep);
							if(back.from!=FDD){
								if(back.from>=F1&&back.from<=F7&&now.hidden)
									hidden[back.from]++;
								movesDeck(sDeck+back.to,sDeck+back.from,back.n,sDeck+ini);
							}else{
								while(back.n--)
									push(sDeck+FDD,pop(sDeck+FD));
							}
						}
						for(i=F1;i<=F7;i++)
							hidden[i]=i-1;
						t1=clock();
						z=0;
						system("cls");
						printfield(sDeck,hidden,Color,Cardback);
						prex=prey=x=y=0;
						gotoxy(0,0);
						putchar(16);
						gotoxy(5,0);
						putchar(17);
						break;
					case 'c':
						Color=rand()%6+1;
						Cardback=rand()%(48-14)+14;
						gotoxy(0,0);
						system("cls");
						printfield(sDeck,hidden,Color,Cardback);
						gotoxy(prex,prey);
						putchar(16);
						gotoxy(prex+5,prey);
						putchar(17);
						break;			
			}
			if(checkwin(sDeck,hidden)){
				system("cls");
				timeused=(float)(clock()-t1)/1000;
				printf("You win!!\n");
				printf("Used time:%.3f sec\n",timeused);
				printf("Score: %d\n",(int)(100000-(timeused*100)-z*100));
				fprintf(fw,"Score:\n");
				fprintf(fw,name);
				fprintf(fw,"%d\n",(int)(100000-(timeused*100)-z*100));
				fclose(fw);
				return 0;
			}
		}
	}

}
void gotoxy(int xpos, int ypos)
{
  COORD scrn;
  HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
  scrn.X = xpos; scrn.Y = ypos;
  SetConsoleCursorPosition(hOuput,scrn);
}
void hidesomething(){
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(hOut,&cci);
	cci.bVisible=0;
	SetConsoleCursorInfo(hOut,&cci);
}
void showsomething(){
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(hOut,&cci);
	cci.bVisible=1;
	SetConsoleCursorInfo(hOut,&cci);
}
void fillDeck(Card* const wDeck){
	int i;
	for(i=0;i<CARDS;i++){
		wDeck[i].face=i%FACES+1;
		wDeck[i].suit=i/FACES;
	}
}
void shuffle(Card* const wDeck, int n){
	int i,j;
	Card temp;
	for(i=0;i<n;i++){
		j=rand()%(n-i)+i;
		temp=wDeck[i];
		wDeck[i]=wDeck[j];
		wDeck[j]=temp;
	}//swap 1 with 1~52, 2 with 2~52......randomly
}
void pushstep(stepNodePtr *topPtr,Step myStep){
	stepNodePtr newPtr;
	newPtr=(stepNodePtr)malloc(sizeof(stepNode));
	if(newPtr!=NULL){
		newPtr->myStep=myStep;
		newPtr->next=*topPtr;
		*topPtr=newPtr;
	}else
		printf("No memory available.\n");
}
Step popstep(stepNodePtr *topPtr){
	stepNodePtr trashPtr;
	Step tStep;
	trashPtr=*topPtr;
	tStep=trashPtr->myStep;
	*topPtr=(*topPtr)->next;
	free(trashPtr);
	return tStep;
}
void push(stackNodePtr *topPtr,Card wCard){
	stackNodePtr newPtr;
	newPtr=(stackNodePtr)malloc(sizeof(stackNode));
	if(newPtr!=NULL){
		newPtr->wCard=wCard;
		newPtr->next=*topPtr;
		*topPtr=newPtr;
	}else
		printf("No memory available.\n");
}
Card pop(stackNodePtr *topPtr){
	stackNodePtr trashPtr;
	Card tCard;
	trashPtr=*topPtr;
	tCard=trashPtr->wCard;
	*topPtr=(*topPtr)->next;
	free(trashPtr);
	return tCard;
}
int stackSize(stackNodePtr currentPtr){
	int cnt=0;
	while(currentPtr!=NULL){
		cnt++;
		currentPtr=currentPtr->next;
	}
	return cnt;
}
int isEmpty(stackNodePtr topPtr){
	return topPtr==NULL;
}
int isEmptystep(stepNodePtr topPtr){
	return topPtr==NULL;
}
void printStack(stackNodePtr currentPtr){
	if(currentPtr==NULL)
		printf("The stack is empty\n");
	else{
		while(currentPtr!=NULL){
			printf("%d %d->\t",currentPtr->wCard.suit,currentPtr->wCard.face);
			currentPtr=currentPtr->next;
		}
	}
}
void iniDeck(stackNodePtr *topPtr,Card* const wDeck){
	int i;
	for(i=0;i<CARDS;i++)
		push(topPtr,wDeck[i]); 
}
void movesDeck(stackNodePtr *fromPtr,stackNodePtr *toPtr,int n,stackNodePtr *recycle){
	int i;
	for(i=0;i<n;i++)
		push(recycle,pop(fromPtr));
	for(i=0;i<n;i++)
		push(toPtr,pop(recycle));
}
void buildfield(stackNodePtr *iniPtr,int hidden[]){
	int i;
	for(i=F1;i<=F7;i++)
		hidden[i]=i-1;
	for(i=1;i<8;i++)
		movesDeck(iniPtr+0,iniPtr+i,i,iniPtr);
	movesDeck(iniPtr+0,iniPtr+12,24,iniPtr);
}
void alwayswin(stackNodePtr *iniPtr,int hidden[],Card * const wDeck){
	int i,table[13]={FD,F1,F2,F3,F4,F5,F6,F7,FD,FD,FD,FD,FD},maxn[8]={24,1,2,3,4,5,6,7},now,here;
	for(i=F1;i<=F7;i++)
		hidden[i]=i-1;
	for(i=0;i<CARDS;i++)
		push(iniPtr+(i/13)+P1,wDeck[i]);
	for(i=CARDS-1;i>=0;i--){
		now=rand()%13;
		if(now>7)
			now=0;
		if(maxn[now]){
			here=rand()%4;
			if(!isEmpty(*(iniPtr+P1+here))){
				push(iniPtr+table[now%8],pop(iniPtr+P1+here));
				maxn[now]--;
			}else{
				i++;
			}		
		}else
			i++;
	}
}
void printsDeck(stackNodePtr topPtr,int n){
	int i;
	char table[4]={(char)6,(char)3,(char)5,(char)4};
	//char table[4]={'B','H','T','D'};
	char table2[][4]={"0","A","2","3","4","5","6","7","8","9","10","J","Q","K"};
	if(isEmpty(topPtr)){
		printf("The stack is empty\n");
		return;
	}
	for(i=1;i<n;i++)
		topPtr=topPtr->next;
	if(topPtr->wCard.suit%2)
		setcolor(RED);
	else
		setcolor(BLACK);
	printf("%2c",table[topPtr->wCard.suit]);
	printf("%-2s",table2[topPtr->wCard.face]);
	setcolor(RESET);
}
Card nCard(stackNodePtr topPtr,int n){
	int i;
	Card noCard;
	noCard.face=-2;
	noCard.suit=-2;
	if(!isEmpty(topPtr)){
		for(i=1;i<n;i++)
			topPtr=topPtr->next;
		return topPtr->wCard;
	}else
		return noCard;
}
void printfield(stackNodePtr *iniPtr,int hidden[],int Color,char Cardback){
	int i,j,cursize;
	//print rule
	gotoxy(45,0);
	printf("上下左右移動");
	gotoxy(45,1);
	printf("空白鍵操作");
	gotoxy(45,2);
	printf("z:回復 c:改顏色");
	gotoxy(45,3);
	printf("f:翻牌(空白鍵也可以翻)");
	gotoxy(45,4); 
	printf("n:新遊戲 r:重新開始遊戲");
	gotoxy(45,5);
	printf("移動規則:");
	gotoxy(45,6);
	printf("空白鍵選取想要移動的牌");
	gotoxy(45,7);
	printf("然後移動至想放上位置按空白鍵");
	gotoxy(0,0);  
	//print flip-deck FD:
	printf("  %2d   ",stackSize(*(iniPtr+FD)));
	//print fliped-deck FDD:
	if(!isEmpty(*(iniPtr+FDD)))
		printsDeck(*(iniPtr+FDD),1);
	else
		printf("    ");
	printf("  EXIT  ");
	//print poped-field P1~4:
	for(i=P1;i<=P4;i++){
		if(!isEmpty(*(iniPtr+i)))
			printsDeck(*(iniPtr+i),1);
		else
			printf("    ");
		printf("  ");
	}
	printf("\n");
	//print playing-field:
	for(i=0;i<20;i++){
		for(j=F1;j<=F7;j++){
			if(j==F1)
				putchar(' ');
			cursize=stackSize(*(iniPtr+j));
			//printf("%d %d\n",j,visible[j]);
			if(cursize>i){
				if(i>=hidden[j])
					printsDeck(*(iniPtr+j),cursize-i);
				else{
					setcolor(0,Color);
					printf(" %c%c ",Cardback,Cardback);
					setcolor(RESET);
				}
					
			}
			else
				printf("    ");
			printf("  ");
		}
		printf("\n");
	}
}
int valid(stackNodePtr *fromPtr,stackNodePtr *toPtr, int n,int to){
	Card fCard=nCard(*fromPtr,n), tCard=nCard(*toPtr,1);
	if(isEmpty(*fromPtr))
		return 0;
	if(to<=P4&&to>=P1)
		if(isEmpty(*toPtr)&&fCard.face==1)
			return 1;
		else if(fCard.suit==tCard.suit&&fCard.face-1==tCard.face)
			return 1;
		else
			return 0;
	if(isEmpty(*toPtr)&&fCard.face==13)
		return 1;
	else if(fCard.suit%2!=tCard.suit%2&&fCard.face+1==tCard.face)
		return 1;
	else 
		return 0;
}
int checkwin(stackNodePtr *Deck,int hidden[]){
	int i;
	int flag=1;
	for(i=F1;i<=F7;i++){
		if(hidden[i]>0)
			flag=0;
	}	
	if(stackSize(*(Deck+FD))||stackSize(*(Deck+FDD))){
		flag=0;
	}
	if(flag)
		return 1;
	for(i=8;i<12;i++)
		if(stackSize(*(Deck+i))!=13)
			return 0;
	return 1;
}
int maxstacksize(stackNodePtr *currentPtr){
	int i;
	int max=stackSize(*(currentPtr+F1));
	for(i=F2;i<=F7;i++){
		if(stackSize(*(currentPtr+i))>max)
			max=stackSize(*(currentPtr+i));
	}
	return max;
}
void setcolor(int textcolor,int bgcolor){
	//setcolor(7,0); RESET
	//setcolor(0,7); BLACK
	//setcolor(4,7); RED
	unsigned short ForeColor=textcolor+16*bgcolor;
	HANDLE hCon=GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hCon,ForeColor);
}
void bubblesort(int *a,int n){
	int i,j,flag=0,temp;
	for(i=0;i<n-1;i++){
		flag=1;
		for(j=0;j<n-1-i;j++){
			if(a[j]<a[j+1]){
				flag=0;
				temp=a[j];
				a[j]=a[j+1];
				a[j+1]=temp;
			}
		}
		if(flag)
			return;
	}
}
