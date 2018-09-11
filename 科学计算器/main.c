#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "myhead.h"
#include "stackfun.h"

void ReceiveExpr()//��ȡexpr�ַ���
{
    char *p;
    int size;
    expr.cur = 0;
    exprSize = MAX_EXPR_LEN;
    expr.str = (char*)malloc(exprSize * sizeof( char ) );
    if( !expr.str)
    {
        printf( "�ڴ����ʧ��\n" );
        exit(0);
    }
    size = 0;
    p = expr.str;
    while(( *p = getchar()) != '\n' )
    {
        if( *p != ' ')
        {
            if(( *p >= 'A') && ( *p <= 'Z'))
            {
                *p = *p + 32;//����дת��ΪСд
            }
            p++;
            size++;
            if( size == exprSize - 1 )//��ʱexpr.str��������������
            {
                exprSize += EXPR_INCREMENT;
                expr.str = ( char* )realloc( expr.str, exprSize * sizeof( char ) );
                if( !expr.str )
                    {
                        printf( "�ڴ����ʧ��\n" );
                        exit(0);
                    }
                    p = &expr.str[size];//*realloc��������ָ��p
            }
        }
    }
    *p++ = '$';
    *p++ = '\0';
}

int IsOpnd( char ch, int *dotNum )//�ж�ch�Ƿ��ǲ�������һ����
{
    if(( ch >= '0')&&( ch <= '9')||( ch == '.'))
    {
        if( ch == '.' )
            *dotNum++;
        return 1;
    }
    if( ( ch == '-' )||( ch == '+' ) )//����+-ǰ����'$'����'(',��Ϊ������
    {
        if(( expr.cur == 0) || (expr.str[expr.cur - 1] == '(' ))
            {
                return 1;
            }
    }
    return 0;
}

void GetElement()//��ȡһ������Ԫ�أ����������������
{
    char *p = elem.str;
    int dotNum = 0;
    *p = expr.str[expr.cur];
    if(IsOpnd(*p, &dotNum))
    {
        while(IsOpnd((*++p = expr.str[ ++expr.cur ]), &dotNum))
            ;//ͬһ������������������С�������������ڴ��жϲ�����
        *p = '\0';
        elem.type = 0;//������Ԫ�������趨Ϊ������
        return;
    }
    if((*p >= 'a') && (*p <= 'z'))//����sin, tan, ln֮��ĺ���������������
    {
        while((expr.str[expr.cur+1] >= 'a') && (expr.str[expr.cur+1] <= 'z'))
        {
            *++p = expr.str[++expr.cur];
        }
    }
    ++expr.cur;
    *++p = '\0';
    if(! strcmp(elem.str, "e"))//*eΪ��Ȼ�����ĵ���
    {
        sprintf(elem.str, "%.16g", E);//�⺯��springf����ʾ����Ļ�ϵ����ݴ������ַ�����
        elem.type = 0;
        return;
    }
    if(! strcmp(elem.str, "pi"))//piΪԲ����
    {
        sprintf(elem.str, "%.16g", PI);
        elem.type = 0;
        return;
    }
    elem.type = 1;//��Ԫ�������趨Ϊ�����
}

char Precede(OptrElem *optr1, OptrElem *optr2)//��������������ȹ�ϵ
{
    char *firstOptr, *lastOptr;
    firstOptr = optr1->optr;
    lastOptr = optr2->optr;
    if( !strcmp(firstOptr, "ln") || !strcmp(firstOptr, "lg") || !strcmp(firstOptr, "sin") || !strcmp(firstOptr, "cos") || !strcmp(firstOptr, "tan"))
    {
        optr1->flag = 1;//����ln�Ⱦ�Ϊ��Ŀ�����
        return(! strcmp(lastOptr, "(")|| ! strcmp(lastOptr, "^")|| !strcmp(lastOptr, "!")) ? '<' : '>' ;
    }
    if(!strcmp(firstOptr, "!"))
    {
        optr1->flag = 1;//�׳�ҲΪ��Ŀ�����
        return '>';
    }
    optr1->flag = 2;
    switch(firstOptr[0])
    {
    case '+':
    case '-':
        return(!strcmp(lastOptr, "+") || !strcmp(lastOptr, "-") || !strcmp(lastOptr, ")") || !strcmp(lastOptr, "$")) ? '>' : '<';
    case '*':
    case '/':
        return(!strcmp(lastOptr, "+") || !strcmp(lastOptr, "-") || !strcmp(lastOptr, "*") || !strcmp(lastOptr, "/0") || !strcmp(lastOptr, ")") || !strcmp(lastOptr, "$")) ? '>' : '<';
    case '(':
        return !strcmp(lastOptr, ")") ? '=' : '<';
    case ')':
        return '>';
    case '^':
        return( !strcmp(lastOptr, "(") || !strcmp(lastOptr, "!") || !strcmp(lastOptr, "^")) ? '<' : '>';
    case '$':
        return !strcmp(lastOptr, "$") ? '=' : '<';
    }
}

long Factorial(long n)//�׳�
{
    return( n <= 1) ? 1 : n * Factorial(n - 1);
}

float Operate(float opnd1, OptrElem optr, float opnd2)
{
    float tmpResult;
    if(optr.flag == 1)
    {
        if(!strcmp(optr.optr, "!"))
            tmpResult = Factorial((long)opnd2);
        else if(!strcmp(optr.optr, "lg"))
            tmpResult = log10(opnd2);
        else if(!strcmp(optr.optr, "ln"))
            tmpResult = log(opnd2);
        else if(!strcmp(optr.optr, "sin"))
            tmpResult = sin(opnd2);
        else if(!strcmp(optr.optr, "cos"))
            tmpResult = cos(opnd2);
        else if(!strcmp(optr.optr, "tan"))
            tmpResult = tan(opnd2);
        return tmpResult;
    }
    switch(optr.optr[0])//˫Ŀ������Ĵ�����Ҫ����������
    {
    case '+':
        tmpResult = opnd1 + opnd2;
        break;
    case '-':
        tmpResult = opnd1 - opnd2;
    case '*':
        tmpResult = opnd1 * opnd2;
        break;
    case '/':
        tmpResult = opnd1 / opnd2;
        break;
    case '^':
        tmpResult = pow(opnd1, opnd2);
    }
    return tmpResult;
}

int Rules1(struct Expression exprCur)//������ʽ�������Ƿ�ƥ��
{
    int bracketsNum = 0;    //��¼��ǰ��ɨ�����δƥ���������
    char *p = expr.str;
    char brackets[100];
    while( *p != '\0' )
    {
        if(( *p == '|' ) || ( *p == '[' ) || (*p == '(' ))
            brackets[bracketsNum++] = *p;
        else if (*p == '|')
        {
            if(( bracketsNum != 0 ) && ( brackets[bracketsNum - 1] == '{' ))
                bracketsNum--;
            else
                return 0;
        }
        else if(*p == ']')
        {
            if(( bracketsNum != 0 ) && ( brackets[bracketsNum - 1] == '[' ))
                bracketsNum--;
            else
                return 0;
        }
        else if(*p == ')')
        {
            if((bracketsNum != 0) && (brackets[bracketsNum - 1] == '(' ))
                bracketsNum--;
            else
                return 0;
        }
        p++;
    }
    return 1;
}

int Rules2()//�����ӵı��ʽ������
{
    //������Ա�д������ʽ�ϸ�Ĺ���
    return 1;
}

int IsExpr()
{
    if( !Rules1(expr))
        return 0;
    if( !Rules2(expr))
        return 0;
    //�ж����ʽ�Ƿ�ϸ�Ĺ���ɲ�������
    return 1;
}

void Compute()//������ʽ
{
    OptrElem optr;
    float opnd1, opnd2;
    InitOptrStack(&OPTR);
    InitOpndStack(&OPND);
    strcpy(optr.optr, "$");//��$�ж�������Ƿ�����ɣ����ڱ��ʽ���˼����ر��$�����
    PushOptr(&OPTR, optr);
    GetElement();//��exprCur.str�л�ȡһ��Ԫ�أ��������������
    while(strcmp(elem.str, "$") || strcmp(OPTR.data[OPTR.len - 1].optr, "$"))
    {
        if(elem.type)//˵����ǰɨ�赽��Ԫ��elem�������
        {
            strcpy(optr.optr, elem.str);//optr��¼��ǰ�������
            switch(Precede(&(OPTR.data[OPTR.len - 1]), &optr))//��ǰ���������һ��ɨ�赫δ�����������Ƚ����ȼ�
            {
            case '<':   //ջ��Ԫ�����ȼ���
                PushOptr(&OPTR, optr);
                GetElement();
                break;
            case '=': //���ȼ���ͬ����ʾΪ����ƥ�䣬ջ��Ԫ�س�ջ�ҽ�����һ������Ԫ��
                PopOptr(&OPTR, &optr);
                GetElement();
                break;
            case '>'://ǰһ������������ȼ��ߣ����ջ���㲢�������ջ
                PopOptr(&OPTR, &optr);
                PopOpnd(&OPND, &opnd2);
                if(optr.flag == 2)//��˫Ŀ�����������һ��������
                    PopOpnd(&OPND, &opnd1);
                PushOpnd(&OPND, Operate(opnd1, optr, opnd2));
            }
        }
        else//˵��elem�洢���ǲ�����
        {
            opnd1 = atof(elem.str);
            PushOpnd(&OPND, opnd1);//atof��elem.strת��Ϊ˫������
            GetElement();//��ȡ��һ��Ԫ��
        }
    }
    printf("%.16g\n\n", OPND.data[OPND.len - 1]);
    free(expr.str);
    DestroyOptrStack(&OPTR);
    DestroyOpndStack(&OPND);
}

int main()
{
    char isQuit;
    printf("\n******************************��ӭʹ��************************************\n\n");
    printf("��Ȼ�����ĵ���������e��Բ����������pi \n");
    printf("�������������������������ȼ���Ϊ :\n  1�����ţ�2���׳ˣ�3����ָ����4�������������������㣬5���������6�������\n");
    printf("\n�����������ʽ��   ");
    ReceiveExpr();
    while(TRUE)
    {
        if(IsExpr())
        {
            Compute();
            printf("\n�Ƿ��������㣬������'Y'����'y', ���������ַ����˳�ϵͳ��");
            isQuit = getchar();
            if(isQuit == 'Y' || isQuit == 'y')
            {
                getchar();
                printf("\n�����������ʽ:  ");
                ReceiveExpr();
            }
            else
                break;
        }
        else
        {
            printf("\n����ʽ����ȷ�����ٴ����������ʽ:   ");
            ReceiveExpr();
        }
    }
    free(expr.str);
    return 0;
}
