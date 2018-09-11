#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "myhead.h"
#include "stackfun.h"

void ReceiveExpr()//获取expr字符串
{
    char *p;
    int size;
    expr.cur = 0;
    exprSize = MAX_EXPR_LEN;
    expr.str = (char*)malloc(exprSize * sizeof( char ) );
    if( !expr.str)
    {
        printf( "内存分配失败\n" );
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
                *p = *p + 32;//将大写转换为小写
            }
            p++;
            size++;
            if( size == exprSize - 1 )//此时expr.str已满，将其扩大
            {
                exprSize += EXPR_INCREMENT;
                expr.str = ( char* )realloc( expr.str, exprSize * sizeof( char ) );
                if( !expr.str )
                    {
                        printf( "内存分配失败\n" );
                        exit(0);
                    }
                    p = &expr.str[size];//*realloc后需重新指定p
            }
        }
    }
    *p++ = '$';
    *p++ = '\0';
}

int IsOpnd( char ch, int *dotNum )//判断ch是否是操作数的一部分
{
    if(( ch >= '0')&&( ch <= '9')||( ch == '.'))
    {
        if( ch == '.' )
            *dotNum++;
        return 1;
    }
    if( ( ch == '-' )||( ch == '+' ) )//如若+-前面是'$'或者'(',则为正负号
    {
        if(( expr.cur == 0) || (expr.str[expr.cur - 1] == '(' ))
            {
                return 1;
            }
    }
    return 0;
}

void GetElement()//获取一个运算元素，即操作数或运算符
{
    char *p = elem.str;
    int dotNum = 0;
    *p = expr.str[expr.cur];
    if(IsOpnd(*p, &dotNum))
    {
        while(IsOpnd((*++p = expr.str[ ++expr.cur ]), &dotNum))
            ;//同一个操作数含两个以上小数点的问题可以在此判断并处理
        *p = '\0';
        elem.type = 0;//将运算元素类型设定为操作数
        return;
    }
    if((*p >= 'a') && (*p <= 'z'))//接收sin, tan, ln之类的函数运算符或操作数
    {
        while((expr.str[expr.cur+1] >= 'a') && (expr.str[expr.cur+1] <= 'z'))
        {
            *++p = expr.str[++expr.cur];
        }
    }
    ++expr.cur;
    *++p = '\0';
    if(! strcmp(elem.str, "e"))//*e为自然对数的底数
    {
        sprintf(elem.str, "%.16g", E);//库函数springf将显示在屏幕上的内容储存在字符串中
        elem.type = 0;
        return;
    }
    if(! strcmp(elem.str, "pi"))//pi为圆周率
    {
        sprintf(elem.str, "%.16g", PI);
        elem.type = 0;
        return;
    }
    elem.type = 1;//将元素类型设定为运算符
}

char Precede(OptrElem *optr1, OptrElem *optr2)//返回运算符的优先关系
{
    char *firstOptr, *lastOptr;
    firstOptr = optr1->optr;
    lastOptr = optr2->optr;
    if( !strcmp(firstOptr, "ln") || !strcmp(firstOptr, "lg") || !strcmp(firstOptr, "sin") || !strcmp(firstOptr, "cos") || !strcmp(firstOptr, "tan"))
    {
        optr1->flag = 1;//以上ln等均为单目运算符
        return(! strcmp(lastOptr, "(")|| ! strcmp(lastOptr, "^")|| !strcmp(lastOptr, "!")) ? '<' : '>' ;
    }
    if(!strcmp(firstOptr, "!"))
    {
        optr1->flag = 1;//阶乘也为单目运算符
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

long Factorial(long n)//阶乘
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
    switch(optr.optr[0])//双目运算符的处理需要两个操作数
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

int Rules1(struct Expression exprCur)//检验表达式中括号是否匹配
{
    int bracketsNum = 0;    //记录当前已扫描过但未匹配的括号数
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

int Rules2()//待增加的表达式检测规则
{
    //这里可以编写检验表达式合格的规则
    return 1;
}

int IsExpr()
{
    if( !Rules1(expr))
        return 0;
    if( !Rules2(expr))
        return 0;
    //判定表达式是否合格的规则可不断增加
    return 1;
}

void Compute()//计算表达式
{
    OptrElem optr;
    float opnd1, opnd2;
    InitOptrStack(&OPTR);
    InitOpndStack(&OPND);
    strcpy(optr.optr, "$");//用$判断运算符是否处理完成，即在表达式两端加上特别的$运算符
    PushOptr(&OPTR, optr);
    GetElement();//从exprCur.str中获取一个元素（操作数或运算符
    while(strcmp(elem.str, "$") || strcmp(OPTR.data[OPTR.len - 1].optr, "$"))
    {
        if(elem.type)//说明当前扫描到的元素elem是运算符
        {
            strcpy(optr.optr, elem.str);//optr记录当前的运算符
            switch(Precede(&(OPTR.data[OPTR.len - 1]), &optr))//当前运算符与上一个扫描但未处理的运算符比较优先级
            {
            case '<':   //栈顶元素优先级低
                PushOptr(&OPTR, optr);
                GetElement();
                break;
            case '=': //优先级相同，表示为括号匹配，栈顶元素出栈且接收下一个运算元素
                PopOptr(&OPTR, &optr);
                GetElement();
                break;
            case '>'://前一个运算符的优先级高，则出栈运算并将结果入栈
                PopOptr(&OPTR, &optr);
                PopOpnd(&OPND, &opnd2);
                if(optr.flag == 2)//是双目运算符才需另一个操作符
                    PopOpnd(&OPND, &opnd1);
                PushOpnd(&OPND, Operate(opnd1, optr, opnd2));
            }
        }
        else//说明elem存储的是操作数
        {
            opnd1 = atof(elem.str);
            PushOpnd(&OPND, opnd1);//atof将elem.str转换为双精度数
            GetElement();//获取下一个元素
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
    printf("\n******************************欢迎使用************************************\n\n");
    printf("自然对数的底数请输入e，圆周率请输入pi \n");
    printf("本计算器处理的运算符及其优先级别为 :\n  1、括号，2、阶乘，3、求指数，4、对数运算与三角运算，5、乘与除，6、加与减\n");
    printf("\n请输入计算表达式：   ");
    ReceiveExpr();
    while(TRUE)
    {
        if(IsExpr())
        {
            Compute();
            printf("\n是否还需更多计算，是输入'Y'或者'y', 输入其他字符将退出系统：");
            isQuit = getchar();
            if(isQuit == 'Y' || isQuit == 'y')
            {
                getchar();
                printf("\n请输入计算表达式:  ");
                ReceiveExpr();
            }
            else
                break;
        }
        else
        {
            printf("\n表达格式不正确，请再次输入计算表达式:   ");
            ReceiveExpr();
        }
    }
    free(expr.str);
    return 0;
}
