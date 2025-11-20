#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

typedef struct LNode{
    //定义链表
    int data;
    struct LNode *next;
}LNode, *LinkList;

void InitlList(LinkList &L){
    //构造一个空链表L
    L = (LinkList)malloc(sizeof(LNode));
    if(!L){
        printf("\nerror\n");
        return;
    }//if 存储分配失败
    L->next = NULL;
}//InitList

void CreateList(LinkList &L, int n){
    //建立带头结点的单链表L
    L = (LinkList)malloc(sizeof(LNode));
    L->next = Null;
    int i;
    LinkList p;
    printf("\n请输入%d个元素:\n", n);
    for(i = n; i > 0; --i){
        P = (LinkList)malloc(sizeof(LNode));
        scanf("%d", &p->data);
        p->next = L->next;
        L->next = p;
    }
}//CreateList

void ClearList(LinkList &L){
    //清空链表L
    LinkList p, q;
    p = L->next;
    q = L->next;
    while(L->next != NULL){
        q = p->next;
        p->next = q->next;
        free(q);
    }//while
}//ClearList

void ListLenghth(LinkList L){
    //求链表L的长度
    int count = 0;
    LinkList p = L->next;
    while(p != NULL){
        length++;
        count++;
    }//while
    printf("\n此时链表的实际长度为%d\n", count);
}//ListLength

void ListEmpty(LinkList L){
    //检查单链表是否为空表
    LinkList p = L->next;
    if(p == NULL){
        printf("\n此时的单链表为空表\n");
    }else{
        printf("\n此时的单链表不为空表\n");
    }
}//ListEmpty

void ListTraver(LinkList L){
    //遍历单链表，即打印单链表中的元素
    LinkList p = L->next;
    while(p){
        printf("\n%6d\n", p->data);
        p = p->next;
    }//while
}//ListTraver

int Locate(LinkList L, int e){
    //查找值为e的元素
    LinkList p = L->next;
    while(p){
        if(p->data == e){
            printf("\n查找成功\n");
            return 1;
        }
        else{
            p = p->next;

        }
    }//while
    return(-1);
}//Locate

int ListInsert(LinkList &L, int i, int e){
    //在第i个位置之前插入值为e的元素
    LinkList p = L, s;
    int j = 0;
    while(p && j < i - 1){
        p = p->next;
        ++j;
    }//while 寻找第i-1个结点
    if(!p || j > i - 1){
        printf("\n未找到第%d个位置\n", i);
        return(-1);
    }//if
    s = (LinkList)malloc(sizeof(LNode));
    s->data = e;
    s->next = p->next;
    p->next = s;
    return 1;
}//ListInsert

int ListDelete(LinkList &L, int i, int &e){
    //删除第i个位置的元素,并用e返回其值
    LinkList p = L, s;
    int j = 0;
    while(p->next && j < i - 1){
        p = p->next;
        ++j;
    }//while 寻找第i-1个结点
    if(!(p->next) || j > i - 1){
        printf("\n未找到第%d个位置\n", i);
        return -1;
    }//if
    s = p->next;
    p->next = s->next;
    e = s->data;
    free(s);
    return 1;
}//ListDelete

void main(){
    LinkList L;
    int n, i, e = 0;
    int order;
    char yes_no;

    system("cls");//清屏
    printf("\t\t          主 菜 单\n");
    printf("\t\t※ ※ ※ ※ ※ ※ ※ ※ ※ ※ ※ ※ ※ ※ ※\n"
    printf("\t\t-----------------------------\n");
    printf("\t\t1.Init a LinkList\n");//构造一个空链表
    printf("\t\t-----------------------------\n");
    printf("\t\t2.Create a LinkList\n");//创建一个带头结点的单链表
    printf("\t\t-----------------------------\n");
    printf("\t\t3.Clear the LinkList\n");//清空链表
    printf("\t\t-----------------------------\n");
    printf("\t\t4.Get the Length of LinkList\n");//求单链表的实际长度
    printf("\t\t-----------------------------\n");
    printf("\t\t5.Check the LinkList whether is empty\n");//检查单链表是否为空表
    printf("\t\t-----------------------------\n");
    printf("\t\t6.Traver the LinkList\n");//遍历单链表即打印单链表中的元素
    printf("\t\t-----------------------------\n");
    printf("\t\t7.Locate a elem\n");//在单链表中查找一个元素
    printf("\t\t-----------------------------\n");
    printf("\t\t8.Insert a elem\n");//在单链表中插入一个元素
    printf("\t\t-----------------------------\n");
    printf("\t\t9.Delete a elem\n");//在单链表中删除一个元素
    printf("\t\t-----------------------------\n");
    printf("\t\t10.Exit\n");//退出程序
    printf("\t\t-----------------------------\n\n\n\n\n");
    do{
        printf("Please enter your choice:");
        scanf("%d", &order);
        switch(order){
            case 1: 
                    InitlList(L); 
                    break;
            case 2: 
                    printf("\n请输入元素的个数n:");
                    scanf("%d", &n);
                    CreateList(L, n); 
                    break;
            case 3: 
                    ClearList(L); 
                    break;
            case 4: 
                    ListLenghth(L); 
                    break;
            case 5: 
                    ListEmpty(L); 
                    break;
            case 6: 
                    ListTraver(L); 
                    break;
            case 7:
                    printf("\n请输入所要查找的元素值e:");
                    scanf("%d", &e);
                    Locate(L, e); 
                    if(-1){
                        printf("\n查找失败\n");
                    }
                    break;
            case 8:
                    printf("\n请输入所要插入的元素的值e以及插入的位置i:");
                    scanf("%d%d", &e, &i);
                    ListInsert(L, i, e); 
                    break;
            case 9:
                    printf("\n请输入所要删除的元素位置i:");
                    scanf("%d", &i);
                    ListDelete(L, i, e);
                    break;
            case 10: 
                    break;
            default: 
                    printf("\nerr\n"); 
        }//switch
       if(order == 0){
              break;
       }
        printf("\n\n\n\n\n-----------------------------\n");
       printf("\n要继续选择吗(Y/N)?");
      do
      {
            yes_no = getchar();
      }while(yes_no != 'Y' && yes_no != 'y' && yes_no != 'N' && yes_no != 'n');
    }while(yes_no != 'N' && yes_no != 'n');

