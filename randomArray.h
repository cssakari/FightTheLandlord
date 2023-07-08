
#ifndef RANDOMARRAY_H
#define RANDOMARRAY_H

#include"ctime"
#include"cstdlib"
#include <algorithm>
#define maxn 20000
int Myarray[maxn];
using namespace std;
struct element{     //用来排序的数据结构
        int data;  // 数据
        int index;  // 序号
};
int cmp(const void *a,const void *b); //升序排列
void rand_of_n(int a[],int n);  //产生 1-n 的随机排列并存到 a[] 中

bool in_left(int a,int b){
        int col1=(a-1)/13;
        int col2=(b-1)/13;
        int p1=(a-1)%13;
        int p2=(b-1)%13;
        if(col1==4||col2==4){
            return (col1==4&&col2!=4)||(col1==4&&col2==4&&p1==1);
        }
        else if(p1==1||p2==1){
            return (p1==1&&p2!=1);
        }
        else if(p1==0||p2==0){
            return (p1==0&&p2!=0);
        }
        else return p1>p2;
}

void generate_array(){

    int n=54;
    rand_of_n(Myarray,n);
    sort(Myarray,Myarray+17,in_left);
}

int cmp(const void *a,const void *b){   // 升序排序
    return((struct element*)a)->data - ((struct element*)b)->data;
}
void rand_of_n(int a[],int n){
    int i;
    struct element ele[maxn];
    srand((int)time(0));  // 初始化随机数种子
    for(i=0;i<n;i++){
        ele[i].data=rand();  // 随机生成一个数
        ele[i].index=i+1;
    }
    qsort(ele,n,sizeof(ele[0]),cmp);  //排序
    for(i=0;i<n;i++){
        a[i]=ele[i].index;
    }
}

#endif // RANDOMARRAY_H
