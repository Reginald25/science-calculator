#ifndef STACKFUN_H_INCLUDED
#define STACKFUN_H_INCLUDED

void InitOptrStack( OptrStack *Optr )//初始化运算符栈
{
    Optr->data = ( OptrElem* ) malloc ( sizeof ( OptrElem ) *MAX_OPTR_NUM );
    if( ! (Optr->data))
    {
        printf( "动态申请内存失败\n" );
        exit(0);
    }
    Optr->len = 0;
}

void DestroyOptrStack( OptrStack *Optr )//销毁运算符栈
{
    free( Optr->data );
    Optr->len = 0;
}

void PushOptr( OptrStack *Optr, OptrElem e )//运算符入栈
{
    if( Optr->len == MAX_OPTR_NUM )
    {
        printf( "运算符已满，无法入栈\n" );
        exit(0);
    }
    strcpy( (Optr->data[ Optr->len ] ).optr, e.optr );
    Optr->data[Optr->len].flag = e.flag;
    Optr->len = Optr->len + 1;
}

void PopOptr( OptrStack *Optr, OptrElem *e )    //运算符出栈
{
    if( Optr->len == 0)
    {
        printf("运算符栈已空，无法出栈\n");
        exit(0);
    }
    Optr->len = Optr->len - 1;
    strcpy( e->optr, (Optr->data[Optr->len]).optr );
    e->flag = Optr->data[Optr->len].flag;
}

void InitOpndStack( OpndStack *S )//初始化操作数栈
{
    S->data = (float * )malloc( sizeof( float ) * MAX_OPND_NUM );
    if( ! ( S->data ) )
    {
        printf( "动态申请内存失败\n" );
        exit(0);
    }
    S->len = 0;
}

void DestroyOpndStack( OpndStack *S )//销毁操作数栈
{
    free( S->data );
    S->len = 0;
}

void PushOpnd( OpndStack *S, float e )//操作数入栈
{
    if( S->len == MAX_OPND_NUM )
    {
        printf( "运算符栈已满，无法入栈\n" );
        exit(0);
    }
    S->data[S->len] = e;
    S->len = S->len + 1;
}

void PopOpnd( OpndStack *S, float *e )//操作数出栈
{
    if( S->len == 0 )
    {
        printf( "运算符栈已空，无法出栈\n" );
        exit(0);
    }
    S->len = S->len - 1;
    *e = S->data[S->len];
}
#endif // STACKFUN_H_INCLUDED
