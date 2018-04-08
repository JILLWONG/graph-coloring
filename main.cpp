#include <iostream>
#include <fstream>
#include <string.h>
#include <sstream>
#include <time.h>

using namespace std;

string url_125_1="C:\\Users\\Lenovo\\Desktop\\instances\\DSJC125.1.col";
string url_250_1="C:\\Users\\Lenovo\\Desktop\\instances\\DSJC250.1.col";
string url_250_5="C:\\Users\\Lenovo\\Desktop\\instances\\DSJC250.5.col";
string url_250_9="C:\\Users\\Lenovo\\Desktop\\instances\\DSJC250.9.col";
string url_500_1="C:\\Users\\Lenovo\\Desktop\\instances\\DSJC500.1.col";
string url_500_5="C:\\Users\\Lenovo\\Desktop\\instances\\DSJC500.5.col";
string url_500_9="C:\\Users\\Lenovo\\Desktop\\instances\\DSJC500.9.col";
string url_1000_1="C:\\Users\\Lenovo\\Desktop\\instances\\DSJC1000.1.col";
string url_1000_5="C:\\Users\\Lenovo\\Desktop\\instances\\DSJC1000.5.col";
string url_1000_9="C:\\Users\\Lenovo\\Desktop\\instances\\DSJC1000.9.col";
//连接集合
int conjSet[2][3300]= {0};
//总点数
int point=0;
//总边数
int edge=0;
//点对应颜色列表
int pColor[300]= {0};
//总冲突数
int allConfli=0;
//冲突边列表
int pConjList[300][300]= {-1};
//总颜色数（编号0-9）
int colorNum=8;
//冲突表
int adjTbl[300][10]= {0};
//禁忌表
int tabuTbl[300][10]= {0};
//选中点
int curPoint=0;
//当前颜色
int fromColor=0;
//即将变成的颜色
int toColor=0;
//迭代次数
int iter=0;
//减少的冲突
int delt=0;
//冲突相同的迭代次数
int cfliEqNum=0;

void readFile(string url) //读文件，构建连接集合
{
    ifstream in(url,ios::in);
    char buffer[256];
    if(!in.is_open())
    {
        cout<<"error opening file!";
        exit(1);
    }
    int j=0;
    while (!in.eof())
    {
        in.getline(buffer,100);
        while(buffer[0]=='c')
        {
            in.getline(buffer,100);
        }
        char *split=" ";
        char *p;
        string str[4];
        int i=0;
        p=strtok(buffer,split);
        while (p!=NULL)
        {
            str[i++]=p;
            p=strtok(NULL,split);
        }
        if(buffer[0]=='p')
        {
            stringstream ss,aa;
            int num=0;
            ss << str[2];
            ss >> num;
            point=num;
            aa << str[3];
            aa >> num;
            edge=num;
        }
        if(buffer[0]=='e')
        {
            int num=0;
            stringstream ss2,aa2;
            ss2 << str[1];
            ss2 >> num;
            conjSet[0][j]=num;
            aa2 << str[2];
            aa2 >> num;
            conjSet[1][j]=num;
            j++;
        }
    }
    in.close();
}

void initial() //初始化解
{
    srand((int)time(0));
    for(int i=0; i<point; i++)
    {
        pColor[i]=rand()%colorNum;
    }

    allConfli=0;
    //pConjList[150][150]={-1};
    for(int a=0; a<300; a++)
    {
        for(int b=0; b<300; b++)
        {
            pConjList[a][b]=-1;
            if(b<10)
            {
                adjTbl[a][b]=0;
                tabuTbl[a][b]=0;
            }
        }
    }
    //adjTbl[150][10]={0};
    //tabuTbl[150][10]={0};
    curPoint=0;
    fromColor=0;
    toColor=0;
    iter=0;
    delt=0;
}

void buildAdjTbl() //构建仇人表、冲突边列表
{
    for(int i=0; i<edge; i++)
    {
        adjTbl[conjSet[0][i]-1][pColor[conjSet[1][i]-1]]++;
        adjTbl[conjSet[1][i]-1][pColor[conjSet[0][i]-1]]++;
        for(int j=0; j<point; j++)
        {
            if(pConjList[j][conjSet[0][i]-1]==-1)
            {
                pConjList[j][conjSet[0][i]-1]=conjSet[1][i]-1;
                break;
            }
        }
        for(int k=0; k<point; k++)
        {
            if(pConjList[k][conjSet[1][i]-1]==-1)
            {
                pConjList[k][conjSet[1][i]-1]=conjSet[0][i]-1;
                break;
            }
        }
    }
}

void countAllConli()
{
    for(int i=0; i<point; i++)
    {
        allConfli+=adjTbl[i][pColor[i]];
    }
    allConfli/=2;
}

void findMove()
{
    int curDeltNtb=1000;
    int curPNtb=0;
    int curFromCNtb=0;
    int curToCNtb=0;
    int sameNtb=0;

    int curDeltTb=1000;
    int curPTb=0;
    int curFromCTb=0;
    int curToCTb=0;
    int sameTb=0;
    for(int i=0; i<point; i++)
    {
        if(adjTbl[i][pColor[i]]>0)
        {
            for(int j=0; j<colorNum; j++)
            {
                if(j!=pColor[i])
                {
                    if(tabuTbl[i][j]>=iter)
                    {
                        if(curDeltTb>adjTbl[i][j]-adjTbl[i][pColor[i]])
                        {
                            sameTb=0;
                            curDeltTb=adjTbl[i][j]-adjTbl[i][pColor[i]];
                            curPTb=i;
                            curFromCTb=pColor[i];
                            curToCTb=j;
                        }
                        if(curDeltTb==adjTbl[i][j]-adjTbl[i][pColor[i]])
                        {
                            int p=100/(sameTb+1);
                            if(rand()%100+1<=p)
                            {
                                curDeltTb=adjTbl[i][j]-adjTbl[i][pColor[i]];
                                curPTb=i;
                                curFromCTb=pColor[i];
                                curToCTb=j;
                            }
                            sameTb++;
                        }
                    }
                    else
                    {

                        if(curDeltNtb>adjTbl[i][j]-adjTbl[i][pColor[i]])
                        {
                            sameNtb=0;
                            curDeltNtb=adjTbl[i][j]-adjTbl[i][pColor[i]];
                            curPNtb=i;
                            curFromCNtb=pColor[i];
                            curToCNtb=j;
                        }
                        if(curDeltNtb==adjTbl[i][j]-adjTbl[i][pColor[i]])
                        {
                            int p=100/(sameNtb+1);
                            if(rand()%100+1<=p)
                            {
                                curDeltNtb=adjTbl[i][j]-adjTbl[i][pColor[i]];
                                curPNtb=i;
                                curFromCNtb=pColor[i];
                                curToCNtb=j;
                            }
                            sameNtb++;
                        }
                    }
                }
            }
        }
    }
    if(curDeltTb<curDeltNtb&&curDeltTb<0)
    {
        delt=curDeltTb;
        curPoint=curPTb;
        fromColor=curFromCTb;
        toColor=curToCTb;
    }
    else
    {
        delt=curDeltNtb;
        curPoint=curPNtb;
        fromColor=curFromCNtb;
        toColor=curToCNtb;
    }
}

void makeMove()
{
    pColor[curPoint]=toColor; //更新着色方案
    allConfli+=delt; //更新冲突总数
    for(int i=0; i<point; i++) //更新仇人表
    {
        if(pConjList[i][curPoint]==-1)
        {
            break;
        }
        else
        {
            adjTbl[pConjList[i][curPoint]][fromColor]--;
            adjTbl[pConjList[i][curPoint]][toColor]++;
        }
    }
    for(int j=0; j<point; j++) //更新禁忌表
    {
        tabuTbl[curPoint][fromColor]=iter+allConfli+rand()%10;
    }

}

int main()
{
    readFile(url_250_1);
    for(int i=0; i<1; i++)
    {
        initial();
        buildAdjTbl();
        countAllConli();
        int cfliBfr=allConfli;
        for(iter=0; iter<300000; iter++)
        {
            //cout<<allConfli<<"\n";
            cfliBfr=allConfli;
            findMove();
            makeMove();
            if(allConfli==0)
            {
                cout<<"find!\niter:"<<iter;
                break;
            }
            /*
            if(cfliBfr==allConfli)
            {
                cfliEqNum++;
            }
            else
            {
                cfliEqNum=0;
            }
            cout<<cfliEqNum<<"!\n";
            */
        }
        if(allConfli==0)
        {
            break;
        }
    }

    return 0;
}
