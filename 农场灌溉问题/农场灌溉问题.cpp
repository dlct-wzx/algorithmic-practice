#include<iostream>
#include<string>
#include<fstream>
#include <sys/timeb.h>
#include <iomanip>
#include <ctime>
#include <sstream>
#define MAXSIZE 1000
using namespace std;

//获取毫秒时间
long long getCurrentTimeMillis()
{

    timeb now;
    ftime(&now);
    std::stringstream milliStream;
    // 由于毫秒数不一定是三位数，故设置宽度为3，前面补0
    milliStream << setw(3) << setfill('0') << right << now.millitm;

    stringstream secStream;
    secStream << now.time;
    string timeStr(secStream.str());
    timeStr.append(milliStream.str());

    long long timeLong;
    stringstream transStream(timeStr);
    transStream >> timeLong;

    return timeLong;
}

//定义方块信息
class point{
    public:
    int top;        //顶部水渠
    int bottom;     //底部水渠
    int left;       //左边水渠
    int right;      //右边水渠
    int visit;      //记录该节点是否被访问
    int gid;        //点的组id
    point(int itop, int ibottom, int ileft, int iright) : top(itop), bottom(ibottom), left(ileft), right(iright) {
        visit = 0;
    }
    point(){}
};

ifstream input("input.txt");        //输入重定向为文件
int wh[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};  //转向信息
point map[MAXSIZE][MAXSIZE];        //存放每个点信息
int m,n;    //存放农场宽高


bool init_by_txt();             //从文件中读取数据
bool init_by_terminal();        //从命令行读取数据
point get_new_point(char ch);   //根据字符获取结点
void solve();                   //问题解决
void dfs(int x, int y, int label);         //深度优先搜索，回溯
unsigned long long start_time, end_time;

void solve(){
    system("cls"); //清屏
    int type;
    cout<<"\n\n\t\t*********************************************\t\t\n";
    printf("\t\t\t\t农场灌溉问题\n");
    printf("\t\t*********************************************\t\t\n");
    printf("\n\n\n\t\t\t1.手动输入数据.");
    printf("\n\t\t\t2.从文件读取数据.");
    printf("\n\n\t\t\t\t选择所要操作:");
    cin>>type;
    while( !(type == 1 || type ==2) ){
        cout<<"\n\n\t\t\t\terror:模式选取错误，请从新选择：";
        cin>>type;
    }
    start_time =  getCurrentTimeMillis();
    
    int item = 1;
    system("cls"); //清屏
    while(1){
        cout<<"********************正在读取第 "<<item<<" 农场的数据********************"<<endl;

        switch(type){
            case 1:
                if(!init_by_terminal()) return;
                break;
            case 2:
                if(!init_by_txt()) return;
                break;
            default:
                cout<<"模式选择错误";
                return ;
                break;
        }
        int sum = 0;
        //遍历所有结点
        for(int i = 0;i < n;i++){
            for(int j = 0;j < m;j++){
                if(map[i][j].visit == 1) //结点已被访问
                    continue;
                dfs(i, j, sum+1);
                sum++;
            }
        }
        item++;
        cout<<endl;
        cout<<"该农场的特征矩阵为: "<<endl;
        for(int i = 0;i < n;i++){
            for(int j = 0;j < m;j++){
                if(j != 0)  cout<<" ";
                cout<<map[i][j].gid;
            }
            cout<<endl;
        }
        cout<<endl;
        cout<<"因此该农场应修建 "<<sum<<" 个水井"<<endl;
        cout<<endl;
    }
    input.close();
}

bool init_by_terminal(){
    cout<<"请输入农场高和宽: ";
    cin>>n>>m;
    char ch;
    if(n == -1 && m == -1) {
        cout<<"**************************程序已退出***************************"<<endl;
        end_time = getCurrentTimeMillis();
        cout<<end_time<<endl;
        cout<<end_time-start_time<<endl;
        return false;
    }
    for(int i = 0;i < n;i++){
        cout<<"请输入农场第"<<i+1<<"行: ";
        for(int j = 0;j < n;j++){
            cin>>ch;
            map[i][j] = get_new_point(ch);
        }
    }
    return true;
}

bool init_by_txt(){
     if(!input){
        cout<<"error:文件打开失败"<<endl;
        return false;
    }
    input>>n>>m;
    cout<<"农场的高和宽分别为: "<<n<<" "<<m<<endl;
    char ch;
    if(n == -1 && m == -1) {
        cout<<"**************************程序已退出***************************"<<endl;
        end_time = getCurrentTimeMillis();
        cout<<"程序运行时间为："<<end_time-start_time<<endl;
        return false;
    }
    for(int i = 0;i < n;i++){
        cout<<"农场第一行为:";
        for(int j = 0;j < n;j++){
            input>>ch;
            cout<<" "<<ch;
            map[i][j] = get_new_point(ch);
        }
        cout<<endl;
    }
    return true;
}

void dfs(int x, int y, int label){
    map[x][y].visit = 1;    //结点被访问
    map[x][y].gid = label;
    for(int i = 0;i < 4;i++){
        if(x + wh[i][0] >= n || x + wh[i][0] < 0 || y + wh[i][1] >= m || y + wh[i][1] < 0)  //约束条件，不可走到农场外
            continue;
        if(map[x + wh[i][0]][y + wh[i][1]].visit == 1)          //约束条件，不可访问已经访问过的结点
            continue;
        switch (i)
        {
        case 0:
            if(map[x][y].top & map[x + wh[i][0]][y + wh[i][1]].bottom == 1) //顶部对底部
                dfs(x + wh[i][0], y + wh[i][1], label);
            break;
         case 1:
            if(map[x][y].bottom & map[x + wh[i][0]][y + wh[i][1]].top == 1) //底部对顶部
                dfs(x + wh[i][0], y + wh[i][1], label);
            break;
         case 2:
            if(map[x][y].left & map[x + wh[i][0]][y + wh[i][1]].right == 1) //左边对右边
                dfs(x + wh[i][0], y + wh[i][1], label);
            break;
         case 3:
            if(map[x][y].right & map[x + wh[i][0]][y + wh[i][1]].left == 1) //右边对左边
                dfs(x + wh[i][0], y + wh[i][1], label);
            break;
        default:
            break;
        }
    }
}

point get_new_point(char ch){
    switch (ch)
    {
    case 'A':
        return point(1, 0, 1, 0);
        break;
    case 'B':
        return point(1, 0, 0, 1);
        break;
    case 'C':
        return point(0, 1, 1, 0);
        break;
    case 'D':
        return point(0, 1, 0, 1);
        break;
    case 'E':
        return point(1, 1, 0, 0);
        break;
    case 'F':
        return point(0, 0, 1, 1);
        break;
    case 'G':
        return point(1, 0, 1, 1);
        break;
    case 'H':
        return point(1, 1, 1, 0);
        break;
    case 'I':
        return point(0, 1, 1, 1);
        break;
    case 'J':
        return point(1, 1, 0, 1);
        break;
    case 'K':
        return point(1, 1, 1, 1);
        break;
    default:
        return point(0, 0, 0, 0);
        break;
    }
}

int main()
{
    solve();
    return 0;
}