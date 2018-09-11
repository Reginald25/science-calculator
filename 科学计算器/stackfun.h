#ifndef STACKFUN_H_INCLUDED
#define STACKFUN_H_INCLUDED

void InitOptrStack( OptrStack *Optr )//��ʼ�������ջ
{
    Optr->data = ( OptrElem* ) malloc ( sizeof ( OptrElem ) *MAX_OPTR_NUM );
    if( ! (Optr->data))
    {
        printf( "��̬�����ڴ�ʧ��\n" );
        exit(0);
    }
    Optr->len = 0;
}

void DestroyOptrStack( OptrStack *Optr )//���������ջ
{
    free( Optr->data );
    Optr->len = 0;
}

void PushOptr( OptrStack *Optr, OptrElem e )//�������ջ
{
    if( Optr->len == MAX_OPTR_NUM )
    {
        printf( "������������޷���ջ\n" );
        exit(0);
    }
    strcpy( (Optr->data[ Optr->len ] ).optr, e.optr );
    Optr->data[Optr->len].flag = e.flag;
    Optr->len = Optr->len + 1;
}

void PopOptr( OptrStack *Optr, OptrElem *e )    //�������ջ
{
    if( Optr->len == 0)
    {
        printf("�����ջ�ѿգ��޷���ջ\n");
        exit(0);
    }
    Optr->len = Optr->len - 1;
    strcpy( e->optr, (Optr->data[Optr->len]).optr );
    e->flag = Optr->data[Optr->len].flag;
}

void InitOpndStack( OpndStack *S )//��ʼ��������ջ
{
    S->data = (float * )malloc( sizeof( float ) * MAX_OPND_NUM );
    if( ! ( S->data ) )
    {
        printf( "��̬�����ڴ�ʧ��\n" );
        exit(0);
    }
    S->len = 0;
}

void DestroyOpndStack( OpndStack *S )//���ٲ�����ջ
{
    free( S->data );
    S->len = 0;
}

void PushOpnd( OpndStack *S, float e )//��������ջ
{
    if( S->len == MAX_OPND_NUM )
    {
        printf( "�����ջ�������޷���ջ\n" );
        exit(0);
    }
    S->data[S->len] = e;
    S->len = S->len + 1;
}

void PopOpnd( OpndStack *S, float *e )//��������ջ
{
    if( S->len == 0 )
    {
        printf( "�����ջ�ѿգ��޷���ջ\n" );
        exit(0);
    }
    S->len = S->len - 1;
    *e = S->data[S->len];
}
#endif // STACKFUN_H_INCLUDED
