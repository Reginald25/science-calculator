#ifndef MYHEAD_H_INCLUDED
#define MYHEAD_H_INCLUDED

#define PI 3.1415926535897932
#define E 2.718281828459045
#define MAX_EXPR_LEN 200    //���ʽ����󳤶�
#define EXPR_INCREMENT 20   //���ʽ���ȵ�����
#define MAX_ELEM_LEN 50     //��������Ԫ�ص���󳤶�
#define MAX_OPTR_NUM 50     //���ʽ����������������
#define MAX_OPND_NUM 50     //���ʽ�в��������������
#define TRUE 1

struct Expression//expression, �����洢���ʽ
{
    char *str;
    int cur;//��Ƕ�ȡexpr�ĵ�ǰλ��
};

struct Element//�����洢һ���������������
{
    char str[MAX_ELEM_LEN];
    int type;           //������Ϊ0����Ϊ����������Ϊ1��Ϊ�����
};

typedef struct      //�����Ԫ��
{
    char optr[11];  //�����
    int flag;       //��Ϊ1����Ϊ��Ŀ���������Ϊ2����˫Ŀ�����
}OptrElem;

typedef struct optrstack    //�����ջ�����ڼ�¼��ɨ�赫δ����������
{
    OptrElem *data;
    int len;
}OptrStack;

typedef struct opndstack //������ջ�����ڼ�¼��ɨ�赫δ����Ĳ�����
{
    float *data;
    int len;
}OpndStack;

OptrStack OPTR;     //�����ջOPTR
OpndStack OPND;     //�����ջOPND
struct Expression expr;
struct Element elem;
int exprSize;       //���ʽ����

//��������
void InitOptrStack( OptrStack *Optr );          //��ʼ�������ջ
void DestroyOptrStack( OptrStack *Optr );       //���������ջ
void PushOptr( OptrStack *Optr, OptrElem e );   //�������ջ
void PopOptr( OptrStack *Optr, OptrElem *e );   //�������ջ
void InitOpndStack( OpndStack *S );             //��ʼ��������ջ
void DestroyOpndStack( OpndStack *S );          //���ٲ�����ջ
void PushOpnd( OpndStack *S, float e );         //��������ջ
void PopOpnd( OpndStack *S, float *e );         //��������ջ
void ReceiveExpr();                             //����������ʽ
int IsOpnd( char ch, int *dotNUm);              //�жϵ�ǰԪ���Ƿ������
void GetElement();                              //���һ������Ԫ�أ����������������
char Precede( OptrElem *optr1, OptrElem *optr2 );//�����������ȼ���ϵ
long Factorial( long n );                       //�׳�����
float Operate( float opnd1, OptrElem optr, float opnd2 );   //��������
int IsExpr();                                   //�����ʽ�Ƿ�Ϸ�
void Compute();                                 //������ʽ
#endif // MYHEAD_H_INCLUDED
