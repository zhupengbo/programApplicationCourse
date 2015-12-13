#include <iostream>
#include <string>
#include <cstdlib>
#include <math.h>
#include <fstream>
#include <map>
#include <set>
#include <vector>
using namespace std;
#define ERROR 0
#define OK 1
#define MAXSIZE 130000000
#define N 170000
#define DELT 1e-5
//typedef double ElemType;
typedef struct{//三元组结构
    int i,j;//非零元素行下标和列下标
    double e;//非零元素值
}Triple;
typedef struct{
    Triple data[MAXSIZE+1];//非零元三元组表，data[0]不用
    int mu,nu,tu;
}TSMatrix;
TSMatrix NewMatrix(int m,int n){
    //新建一个三元组表示的稀疏矩阵
    TSMatrix M;
    M.mu=m;
    M.nu=n;
    M.tu=0;
    return M;
}
void newMatrix(TSMatrix* M , int m , int n){
    M->mu=m;
    M->nu=n;
    M->tu=0;
    //return &M;
}
void split(const string& src, const string& separator, vector<string>& dest)
 {
     string str = src;
     string substring;
     string::size_type start = 0, index;
     do
     {
         index = str.find_first_of(separator,start);
         if (index != string::npos)
         {
             substring = str.substr(start,index-start);
             dest.push_back(substring);
            start = str.find_first_not_of(separator,index);
             if (start == string::npos) return;
         }
     }while(index != string::npos);

     //the last token
     substring = str.substr(start);
     dest.push_back(substring);
 }
int insertTriple(TSMatrix *M,int row,int col,double e){
    int i,t,p;
    if(M->tu>=MAXSIZE){
        cout<<"数据已满"<<endl;
        return ERROR;
    }
    if(row>M->mu||col>M->nu||row<1||col<1){
        cout<<"行列索引错误"<<endl;
    }
    p = 1;
    if(M->tu==0){//插入前矩阵M没有非零元素
        M->data[p].i=row;
        M->data[p].j=col;
        M->data[p].e=e;
        M->tu++;
        return OK;
    }
    for(t=1;t<=M->tu;t++)//寻找合适的插入位置
        if((row>=M->data[t].i)&&(col>=M->data[t].j))
                p++;
    if(row==M->data[t-1].i && col==M->data[t-1].j){//插入前，该元素已经存在
        M->data[t-1].e=e;
        return OK;
    }
    for(i=M->tu;i>=p;i--){//移动p之后的元素
        M->data[i+1].i=M->data[i].i;
        M->data[i+1].j=M->data[i].j;
        M->data[i+1].e=M->data[i].e;
    }
    //插入新元素
    M->data[p].i=row;
    M->data[p].j=col;
    M->data[p].e=e;
    M->tu++;
    return OK;
}
void PrintSMatrix(const TSMatrix *M){
    //打印稀疏矩阵所有元素
    int i,j,p=1;
    cout<<"\nsize:"<<M->mu <<" × "<< M->nu<<endl;
    if(!M->tu){//0矩阵
        cout<<0<<endl;
        return;
    }
    for(i=1;i<=M->mu;i++){
        for(j=1;j<=M->nu;j++){

            if(i==M->data[p].i && j==M->data[p].j){
                cout<<M->data[p].e<<"\t";
                p++;
            }else{
                cout<<0<<"\t";
            }
        }
        cout<<"\n";
    }
    cout<<endl;
}
double slove(double v[N])
{
    double max;
    for(int i=0;i<N-1;i++) max=v[i]>v[i+1]?v[i]:v[i+1];
    return max;
}
//稀疏矩阵的幂法
void powerInter(const TSMatrix M,double X[N],double Y[N]){
    //double Y[];
    while(1){

        for(int i = 0 ; i<N ; i++){
            X[i] = Y[i];
            Y[i] = 0;
        }
       for(int p = 1 ; p <= M.tu ; p++){
            Triple temp = M.data[p];
            Y[temp.i-1] += (temp.e * X[temp.j-1])/slove(X);
       }
       //<<fabs(slove(X)-slove(Y))<<endl;
       if(fabs(slove(X)-slove(Y))<DELT){break;}
    }
}
//
void initX(TSMatrix M , double X[] , double Y[]){
    for(int p = 1 ; p <= M.tu ; p++){
        int temp = M.data[p].j-1;
        //if(X[temp] == 0){
        X[temp] = Y[temp] = 1;
       // }
    }
}
void readDataFromFile(char* filePath ,TSMatrix* M){

    ifstream in(filePath);
    map<int,set<int> > resultMap ;
    string line;
    while(getline(in,line)){

        int pos = line.find(':');
        int col = atoi((line.substr(0,pos)).c_str());
        int start = line.find('[');
        int end = line.find(']');
        const string rows = line.substr(start,end);
        vector<string> tokens;
        split(rows,",",tokens);
        for(int i = 0 ; i < tokens.size() ;i++){
            //insertTriple(M,atoi(tokens[i].c_str()),col,1);
        }

        //char key[1024]={0};
        //set<int> value;

       // in.getline(line,sizeof(line));
        //in>>key;
        //in.read((set)value,sizeof(value));//>>value;
        //resultMap.insert(make_pair(key,value));
        cout<<col<<"\t@"<<tokens.size()<<"&&&"<<line<<endl;
    }
}
int main()
{
    //Triple data[130000000];
    cout<<sizeof(double)*160000/1024.0/1024<<endl;
    cout<<sizeof(TSMatrix)/1024.0/1024<<endl;
    //char* filePath = "f:\\object2int.txt";
    //TSMatrix M = NewMatrix(N,N);
    //readDataFromFile(filePath , &M);

    TSMatrix* M = (TSMatrix*)malloc(sizeof(TSMatrix));//=NewMatrix(N,N);
    newMatrix(M,N,N);
    cout<<M<<endl;
   // TSMatrix T=NewMatrix(N,N);
    insertTriple(M,1,1,1);
    insertTriple(M,1,2,1);
    insertTriple(M,1,3,0.5);
    insertTriple(M,2,1,1);
    insertTriple(M,2,2,1);
    insertTriple(M,2,3,0.25);
    insertTriple(M,3,1,0.5);
    insertTriple(M,3,2,0.25);
    insertTriple(M,3,3,2);

    //初始化X
    double X[N]={0},Y[N]={0},Y2[N]={0};
    initX(*M,X,Y);
    powerInter(*M,X,Y);
    initX(*M,X,Y2);
    powerInter(*M,X,Y2);
    //cout<<"X:\n";
    for(int i = 0; i < N;i++){
        cout<<Y[i]+Y2[i]<<endl;
    }
    //cout<<"\nM:";
    //PrintSMatrix(&M);*/

}
