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
#define MAXR 20//����20******************************
#define MAXC 20//����20******************************
#define STACK_INIT_SIZE 1000  //ջ�Ŀռ����
#define STACKINCREMENT 100
struct Pos{  //����λ��
    int x,y;
};
struct Node{
    int no;//���
    struct Pos p;
    int dir;//1-east,2-south,3-West,4-north �Ӵ�ͨ��������һ��ķ���
};
struct Maze{//��ʼ���Թ���
    int r,c;
    char m[MAXR+2][MAXC+2];//0-road,1-wall,2-footprint
    struct Pos s;//start
    struct Pos e;//end
};

struct Stack{//����ջ��
    struct Node *base;
    struct Node *top;
    int size;
};
int  InitStack(struct Stack *S){//����һ����ջS
    S->base = (struct Node *)malloc(STACK_INIT_SIZE * sizeof(struct Node));
    if (!S->base) exit(OVERFLOW);
    S->top = S->base;
    S->size = 0;
    return OK;
}

void cleanStack(struct Stack *s){
    free(s->base);
    s->size = 0;
    s->base = s->top = NULL;
}

int Push(struct Stack *S,struct Node e){  //��ջ+e
    if (S->top - S->base >= S->size){  //���ջ����׷�Ӵ洢�ռ�
        S->base = (struct Node *)realloc(S->base,
        (S->size + STACKINCREMENT)*sizeof(struct Node));
        if (!S->base)exit(OVERFLOW);//�洢����ʧ��
        S->top = S->base + S->size;
        S->size += STACKINCREMENT;
    }
    *S->top++ = e;
    S->size ++;
    return OK;
}
int Pop(struct Stack *S,struct  Node e){//��ջ-e
    if (S->size == 0) return ERROR;
    e = *--S->top;
    S->size --;
    return OK;
}

int inputmaze(struct Maze *a_maze,char *mazefile){//�Թ�������
    int i,j;
    char row[MAXC+10];//bufsize can be big
    FILE *fp;
    int s,e;//���������յ�
    s=0,e=0;
    fp = fopen(mazefile,"r");
    if(!fp){
        printf("Can not find file: %s\n",mazefile);
        //getch();
        return ERROR;
    }
    fscanf(fp,"%d,%d",&(a_maze->r),&(a_maze->c));
    //��ǽ
    for(i=0;i < MAXR+2;i++)
        for(j=0;j < MAXC+2;j++)
            a_maze->m[i][j] = '1';//������ǽ,0-road,1-wall,2-footprint
    //maze
    //fgetc(fp);//????????????????????

    for(i = 1;i <= a_maze->r;i ++){
        fgets(row, MAXC+10, fp);
        for(j = 1;j <= a_maze->c;j ++){
            a_maze->m[i][j] = row[j-1];
            if(a_maze->m[i][j]=='S'){
                a_maze->s.x = i;
                a_maze->s.y = j;
                s = 1;
            }
            if(a_maze->m[i][j]=='E'){
                a_maze->e.x = i;
                a_maze->e.y = j;
                e = 1;
            }
        }
    }
    fclose(fp);
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
    struct Stack *s;
    InitStack(s);

    int x, y, lastdir, dir;
    int px, py;
    struct Node n;
    x = a_maze->s.x;
    y = a_maze->s.y;
    dir = 1;
    lastdir = 0;

    while(x != a_maze->e.x && y != a_maze->e.y){
        if(a_maze->m[x+1][y] == 0 && lastdir == 0){
            a_maze->m[x+1][y] = 2;
            x += 1;
            n.dir = 1;
            n.p.x = x;
            n.p.y = y;
            Push(s,n);
        }else if(a_maze->m[x][y-1] ==0 && lastdir == 1){
            a_maze->m[x][y-1] = 2;
            y -= 1;
            n.dir = 2;
            n.p.x = x;
            n.p.y = y;
            Push(s,n);
        }else if(a_maze->m[x-1][y] ==0 && lastdir == 2){
            a_maze->m[x-1][y] = 2;
            x -= 1;
            n.dir = 3;
            n.p.x = x;
            n.p.y = y;
            Push(s,n);
        }else if(a_maze->m[x][y+1] ==0 && lastdir == 3){
            a_maze->m[x+1][y] = 2;
            y += 1;
            n.dir = 4;
            n.p.x = x;
            n.p.y = y;
            Push(s,n);
        }else{
            Pop(s, n);
            lastdir = n.dir;
            n.dir = 1;
            n.p.x = x+1;
            n.p.y = y;
        }
    }

    Pop(s, n);
    x = n.p.x;
    y = n.p.y;
    lastdir = n.dir;
    while(s->size != 0){
        Pop(s, n);
        px = n.p.x;
        py = n.p.y;
        dir = n.dir;
        //lastdir = n.dir;

        if (lastdir == 1 && dir == 1)a_maze->m[x][y] = 'A';
        if (lastdir == 2 && dir == 2)a_maze->m[x][y] = 'B';
        if (lastdir == 3 && dir == 3)a_maze->m[x][y] = 'C';
        if (lastdir == 4 && dir == 4)a_maze->m[x][y] = 'D';
        //��
        if (lastdir == 3 && dir == 2)a_maze->m[x][y] = 'I';
        if (lastdir == 4 && dir == 1)a_maze->m[x][y] = 'I';
        //��
        if (lastdir == 1 && dir == 2)a_maze->m[x][y] = 'F';
        if (lastdir == 4 && dir == 3)a_maze->m[x][y] = 'F';
        //��
        if (lastdir == 1 && dir == 4)a_maze->m[x][y] = 'G';
        if (lastdir == 2 && dir == 3)a_maze->m[x][y] = 'G';
        //��
        if (lastdir == 2 && dir == 1)a_maze->m[x][y] = 'H';
        if (lastdir == 3 && dir == 4)a_maze->m[x][y] = 'H';

        x = px;
        y = py;
        dir = lastdir;
    }

    a_maze->m[a_maze->s.x][a_maze->s.y] = 'S';
    a_maze->m[a_maze->e.x][a_maze->e.y] = 'E';

    cleanStack(s);
    return;

}

int main(){
    struct Maze *maze1 = (struct Maze *)malloc(MAXSTACKSIZE*sizeof(struct Maze));
    if (inputmaze(maze1, "maze_v2/maze1.txt") == OK){
        printf("maze:\n");

        drawpath(maze1);//ָʾ·��
        dispmaze(maze1); //���ͨ·

        //getch();
    }
    return 0;
}



