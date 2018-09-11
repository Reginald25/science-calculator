#ifndef MYHEAD_H_INCLUDED
#define MYHEAD_H_INCLUDED

#define PI 3.1415926535897932
#define E 2.718281828459045
#define MAX_EXPR_LEN 200    //表达式的最大长度
#define EXPR_INCREMENT 20   //表达式长度的增量
#define MAX_ELEM_LEN 50     //单个运算元素的最大长度
#define MAX_OPTR_NUM 50     //表达式中运算符的最大数量
#define MAX_OPND_NUM 50     //表达式中操作数的最大数量
#define TRUE 1

struct Expression//expression, 用来存储表达式
{
    char *str;
    int cur;//标记读取expr的当前位置
};

struct Element//用来存储一个操作数或运算符
{
    char str[MAX_ELEM_LEN];
    int type;           //类型若为0，则为操作数，若为1则为运算符
};

typedef struct      //运算符元素
{
    char optr[11];  //运算符
    int flag;       //若为1，则为单目运算符，若为2则是双目运算符
}OptrElem;

typedef struct optrstack    //运算符栈，用于记录已扫描但未处理的运算符
{
    OptrElem *data;
    int len;
}OptrStack;

typedef struct opndstack //操作数栈，用于记录已扫描但未处理的操作数
{
    float *data;
    int len;
}OpndStack;

OptrStack OPTR;     //运算符栈OPTR
OpndStack OPND;     //运算符栈OPND
struct Expression expr;
struct Element elem;
int exprSize;       //表达式长度

//函数声明
void InitOptrStack( OptrStack *Optr );          //初始化运算符栈
void DestroyOptrStack( OptrStack *Optr );       //销毁运算符栈
void PushOptr( OptrStack *Optr, OptrElem e );   //运算符入栈
void PopOptr( OptrStack *Optr, OptrElem *e );   //运算符出栈
void InitOpndStack( OpndStack *S );             //初始化操作数栈
void DestroyOpndStack( OpndStack *S );          //销毁操作数栈
void PushOpnd( OpndStack *S, float e );         //操作数入栈
void PopOpnd( OpndStack *S, float *e );         //操作数出栈
void ReceiveExpr();                             //接收运算表达式
int IsOpnd( char ch, int *dotNUm);              //判断当前元素是否操作数
void GetElement();                              //获得一个运算元素（操作数或运算符）
char Precede( OptrElem *optr1, OptrElem *optr2 );//返回运算优先级关系
long Factorial( long n );                       //阶乘运算
float Operate( float opnd1, OptrElem optr, float opnd2 );   //其他计算
int IsExpr();                                   //检查表达式是否合法
void Compute();                                 //计算表达式
#endif // MYHEAD_H_INCLUDED
