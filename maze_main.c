#include <stdio.h>
//#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define MAXSTACKSIZE 400 //�����С
#define MAXR 20//�������20******************************
#define MAXC 20//�������20******************************
#define STACK_INIT_SIZE 1000  //ջ�Ŀռ����
#define STACKINCREMENT 100
struct Pos{  //����λ��
    int x,y;
};
struct Node{
    struct Node *prev;//��ν�������������������Ԫ���а���ָ������Ԫ�ص�ָ��
    struct Pos p;
    int dir;//1-east,2-south,3-West,4-north �Ӵ�ͨ��������һ��ķ���
};
struct Maze{//��ʼ���Թ���
    int r,c;
    char m[MAXR+2][MAXC+2];//0-road,1-wall,2-footprint
    struct Pos s;//start
    struct Pos e;//end
};
void showmaze(struct Maze *pm){
	int i,j;
	for(i = 0; i != MAXC+2;i ++){
		printf("%2d: ",i);
		for(j = 0;j != MAXC +2;j ++)
			printf("%c",pm->m[i][j]);
		printf("\n");
	}
}
struct Stack{//����ջ��    
    struct Node *top;
    int size;//ջ��Ԫ������
};

void showStackfuzhu(struct Node *nd){
	if(!nd)
		return ;
	showStackfuzhu(nd->prev);
	printf("(%2d, %2d, %2d)\n", nd->p.x,nd->p.y, nd->dir); 
}
void showStack(struct Stack *s){
	showStackfuzhu(s->top);
}

int  InitStack(struct Stack *S){//����һ����ջS
    S->top = NULL;
    S->size = 0;
    return OK;
}

int Push(struct Stack *S,struct Node e){  //��ջ+e
    struct Node *pn = (struct Node*)malloc(sizeof(struct Node));
	if(!pn){
		printf("����ռ�ʧ����\n");
		exit(3);
	}
	pn->dir = e.dir;
	pn->p   = e.p;
	pn->prev = S->top;

    S->top  = pn;
    S->size ++;
    return OK;
}
struct Node 
	Pop(struct Stack *S){//��ջ-e
	struct Node nd;
	struct Node *tpd;//��ʱָ�룬ָ�����ǰ��ջ��
    if (S->size == 0){
		printf("ջ�ǿ���\n");
		exit(4);
	}
	nd.dir = S->top->dir;
	nd.p   = S->top->p;
	nd.prev = NULL;
    tpd = S->top;
	S->top = S->top->prev;//ջ���½�
	free(tpd);
    S->size --;
    return nd;
}

void freeStack(struct Stack *s){
	while(s->size){
		Pop(s);
	}
}
int inputmaze(struct Maze *a_maze,char *mazefile){//�Թ�������
    int i,j;
    char row[MAXC+10];//bufsize can be big
    FILE *fp;
    int s,e;//���������յ�
    s=0,e=0;
    fopen_s(&fp, mazefile,"r");
    if(!fp){
        printf("Can not find file: %s\n",mazefile);
        //getch();
        return ERROR;
    }
    fscanf_s(fp,"%d,%d",&(a_maze->r),&(a_maze->c),4*sizeof(char));
    //��ǽ
	fgetc(fp);//deal with "/n"
    for(i=0;i < MAXR+2;i++)
        for(j=0;j < MAXC+2;j++)
            a_maze->m[i][j] = '1';//������ǽ,0-road,1-wall,2-footprint
    //showmaze(a_maze);
	//printf("%d, %d \n",a_maze->r,a_maze->c);
    for(i = 1;i <= a_maze->r;i ++){
        fgets(row, MAXC+10, fp);
		//printf("%s\n",row);
        for(j = 1;j <= a_maze->c;j ++){
            a_maze->m[i][j] = row[j-1];
            if(a_maze->m[i][j] == 'S'){
                a_maze->s.x = i;
                a_maze->s.y = j;
                s = 1;
            }else if(a_maze->m[i][j] == 'E'){
                a_maze->e.x = i;
                a_maze->e.y = j;
                e = 1;
            }
        }
    }
    fclose(fp);
	//showmaze(a_maze);
    if (s+e < 2) {
        printf("%s can not find where to start or end\n",mazefile);
        //getch();
        return ERROR;
    }
    return OK;
}

void dispmaze(struct Maze *a_maze){//���ͨ·
    int i,j;
    printf("��·  ��ǽ  �����  ���յ�  �w��ͨ\n");
    for(i=0;i<=a_maze->r+1;i++){
        for(j=0;j<=a_maze->c+1;j++){//3-east��,4-south��,5-west��,6-north��
            if (i==a_maze->s.x && j==a_maze->s.y){//Ѱ�����
                printf("��");
            }
            else if (i==a_maze->e.x && j==a_maze->e.y){
                printf("��");
            }
            else{
                if(a_maze->m[i][j]=='0') printf("��");
                if(a_maze->m[i][j]=='1') printf("��");
                if(a_maze->m[i][j]=='2') printf("�w");
                if(a_maze->m[i][j]=='A') printf("��");
                if(a_maze->m[i][j]=='B') printf("��");
                if(a_maze->m[i][j]=='C') printf("��");
                if(a_maze->m[i][j]=='D') printf("��");
                if(a_maze->m[i][j]=='I') printf("��");
                if(a_maze->m[i][j]=='F') printf("��");
                if(a_maze->m[i][j]=='G') printf("��");
                if(a_maze->m[i][j]=='H') printf("��");
            }
        }
        printf("\n");
    }
    return;
}
void drawpath(struct Maze *a_maze){   //ָʾ·����
    struct Stack st;
    int x, y, dir, lastdir;
    int px, py;
    struct Node n;
    struct Stack *s = &st;
	x = a_maze->s.x;
    y = a_maze->s.y;
    dir = 1;
    InitStack(s);
    while(a_maze->m[x][y] != 'E'){
        if(a_maze->m[x][y+1] == '0' || a_maze->m[x][y+1] == 'E'){
			if (a_maze->m[x][y+1] == '0')
				a_maze->m[x][y+1] = '2';
            n.dir = 1;
            n.p.x = x;
            n.p.y = y;
			y += 1;
            Push(s,n);
        }else if(a_maze->m[x+1][y] == '0' || a_maze->m[x+1][y] == 'E'){
            if(a_maze->m[x+1][y] = '0')
				a_maze->m[x+1][y] = '2';
            n.dir = 2;
            n.p.x = x;
            n.p.y = y;
			x += 1;
            Push(s,n);
        }else if(a_maze->m[x][y-1] == '0' || a_maze->m[x][y-1] == 'E'){
            if(a_maze->m[x][y-1] = '0')
				a_maze->m[x][y-1] = '2';
            n.dir = 3;
            n.p.x = x;
            n.p.y = y;
			y -= 1;
            Push(s,n);
        }else if(a_maze->m[x-1][y] == '0' || a_maze->m[x+1][y] == 'E'){
			if(a_maze->m[x-1][y] = '0')
				a_maze->m[x-1][y] = '2';
            n.dir = 4;
            n.p.x = x;
            n.p.y = y;
			x -= 1;
            Push(s,n);
        }else{
            n = Pop(s);
            x = n.p.x;
            y = n.p.y;
        }
		//showmaze(a_maze);
    }
	//showmaze(a_maze);
	showStack(s);
    n = Pop(s);
    x = n.p.x;
    y = n.p.y;
    dir = n.dir;
    while(s->size != 0){
        n = Pop(s);
        px = n.p.x;
        py = n.p.y;
        lastdir = n.dir;
        //lastdir = n.dir;

        if (lastdir == 1 && dir == 1)a_maze->m[x][y] = 'A';
        else if (lastdir == 2 && dir == 2)a_maze->m[x][y] = 'B';
        else if (lastdir == 3 && dir == 3)a_maze->m[x][y] = 'C';
        else if (lastdir == 4 && dir == 4)a_maze->m[x][y] = 'D';
        //��
        else if (lastdir == 3 && dir == 2)a_maze->m[x][y] = 'I';
        else if (lastdir == 4 && dir == 1)a_maze->m[x][y] = 'I';
        //��
        else if (lastdir == 1 && dir == 2)a_maze->m[x][y] = 'F';
        else if (lastdir == 4 && dir == 3)a_maze->m[x][y] = 'F';
        //��
        else if (lastdir == 1 && dir == 4)a_maze->m[x][y] = 'G';
        else if (lastdir == 2 && dir == 3)a_maze->m[x][y] = 'G';
        //��
        else if (lastdir == 2 && dir == 1)a_maze->m[x][y] = 'H';
        else if (lastdir == 3 && dir == 4)a_maze->m[x][y] = 'H';

        x = px;
        y = py;
        dir = lastdir;
    }

    a_maze->m[a_maze->s.x][a_maze->s.y] = 'S';
    a_maze->m[a_maze->e.x][a_maze->e.y] = 'E';
	//showmaze(a_maze);
    freeStack(s);
    return;

}

int main(){
    struct Maze *maze1 = (struct Maze *)malloc(MAXSTACKSIZE*sizeof(struct Maze));
    if (inputmaze(maze1, "maze1.txt") == OK){
        printf("maze:\n");

        drawpath(maze1);//ָʾ·��
        dispmaze(maze1); //���ͨ·

        //getch();
    }
	getchar();
    return 0;
}



