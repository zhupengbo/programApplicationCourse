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
#define MAXSIZE 40000000
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
        cout<<"数据已满"<<endl;exit(0);
        return ERROR;
    }
    if(row>M->mu||col>M->nu||row<0||col<0){
        bool b1 = row > M->mu;bool b2 = col > M->nu;
        cout<<M->mu<<" "<<M->nu<<" "<<"1行列索引错误"<<endl;exit(0);
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
    for(int i=0;i<N;i++) //{max=v[i]>max?v[i]:max;}
        max += v[i];
    //cout<<max<<"::::"<<v[0]<<endl;
    return max;
}
//稀疏矩阵的幂法
void powerInter(const TSMatrix *M,double X[N],double Y[N]){
    //for(int i = 0 ; i <N;i++)cout<<X[i]<<"::"<<endl;
    //double Y[];
    while(1){

        int slo = slove(X);
       for(int p = 1 ; p <= M->tu ; p++){
            Triple temp = M->data[p];
            Y[temp.i-1] += (temp.e * X[temp.j-1])/slo;
       }
       for(int q = 0 ; q < N ;q++){
            Y[q] += X[q]*0.15/N/slo;
       }//exit(0);
       //cout<<slo<<" "<<slove(X)<<" "<<slove(Y)<<" "<<fabs(slove(X)-slove(Y))<<endl;
       if(fabs(slove(X)-slove(Y))<DELT){break;}
       for(int i = 0 ; i<N ; i++){
            X[i] = Y[i];
            Y[i] = 0;
        }
    }
}
//
void initX(TSMatrix M , double X[]){
    for(int p = 1 ; p <= M.tu ; p++){
        int temp = M.data[p].j-1;
        //if(X[temp] == 0){
        X[temp]  = 1;
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
        int end1 = line.find(']');
        const string rows = line.substr(start+1,end1);
        vector<string> tokens;
        split(rows,",",tokens);
        for(int i = 0 ; i < tokens.size() ;i++){
                cout<<atoi(tokens[i].c_str())<<":"<<col<<endl;
            insertTriple(M,atoi(tokens[i].c_str()),col,0.85*1.0/tokens.size());
        }

        //char key[1024]={0};
        //set<int> value;

       // in.getline(line,sizeof(line));
        //in>>key;
        //in.read((set)value,sizeof(value));//>>value;
        //resultMap.insert(make_pair(key,value));
        //cout<<col<<"\t@"<<tokens.size()<<"&&&"<<line<<endl;
    }
}

int main()
{
/*
    char* filePath = "./object2int-v3.txt";
    TSMatrix* M = (TSMatrix*)malloc(sizeof(TSMatrix));
    newMatrix(M,N,N);
    readDataFromFile(filePath , M);
    cout<<M<<endl;
/*
    ofstream out("./res3.txt");
    //初始化X
    double X[N],Y[N]={0};
    for(int i =0 ; i<N;i++)X[i] = 1;
    powerInter(M,X,Y);
    for(int i = 0; i < N;i++){
        out<<Y[i]<<endl;
    }
    for(int i = 0 ; i<10;i++){
        double max  = Y[i] ; int index = i ;
        for(int j = 0 ; j <N;j++){
            if(max < Y[j]){
                max = Y[j];
                index = j;
            }
        }
        cout<<index<<endl;
        Y[index] = 0;
    }
    //cout<<"\nM:";
    //PrintSMatrix(&M);
*/

    double X[N];
    ifstream in("./res3.txt");
    for(int i = 0 ; i< N;i++){
        in>>X[i];
        //cout<<X[i]<<endl;//exit(0);
    }

    ifstream inVisited("./visitedUrl-v3.txt");
    string url[N];int h  = 0;
    while(getline(inVisited , url[h++])){

    }

    int index[10];
    //double X[10] = {0,1,2,3,4,5,9,7,6,8};
    for(int i = 0 ; i<10;i++){
        double max  = X[i] ; //int index = i ;
        for(int j = 0 ; j <N;j++){
            if(max < X[j]){
                max = X[j];
                index[i] = j;
            }
        }
        cout<<url[index[i]]<<" : "<<max<<endl;
        X[index[i]] = 0;
    }

}
