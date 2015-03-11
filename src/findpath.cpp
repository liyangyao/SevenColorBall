/****************************************************************************

Creator: liyangyao@gmail.com
Date: 2015/3/11

****************************************************************************/
#pragma execution_character_set("utf-8")

#include "findpath.h"
#include <QDebug>

FindPath::FindPath(int src, int dst, int *board):
    m_src(src),
    m_dst(dst),
    m_board(board),
    m_visit(121, false)
{

}

bool FindPath::start()
{
    qDebug()<<"src="<<m_src<<" dst="<<m_dst;
    return goFrom(m_src);
}


bool FindPath::goFrom(int sq)
{
    m_visit[sq] = true;
    m_sqList.append(QString::number(sq));
    //qDebug()<<m_sqList.join("->");
    //同一行

    enum Direct{Left, Right, Up, Down};
    Direct d[4];
    int x = sq%11;
    int y = sq/10;
    int xx = m_dst%11;
    int yy = m_dst/11;
    if (y==yy)//同一行
    {
        if (x<xx)
        {
            d[0] = Right;
            d[1] = Up;
            d[2] = Down;
            d[3] = Left;
        }
        else{
            d[0] = Left;
            d[1] = Up;
            d[2] = Down;
            d[3] = Right;
        }
    }
    else if (x==xx)//同一列
    {
        if (y<yy)
        {
            d[0] = Down;
            d[1] = Left;
            d[2] = Right;
            d[3] = Up;
        }
        else{
            d[0] = Up;
            d[1] = Left;
            d[2] = Right;
            d[3] = Down;
        }
    }
    else if (x<xx && y<yy)//右下
    {
        d[0] = Right;
        d[1] = Down;
        d[2] = Left;
        d[3] = Up;
    }
    else if (x>xx && y>yy)//左上
    {
        d[0] = Left;
        d[1] = Up;
        d[2] = Right;
        d[3] = Down;
    }
    else if (x>xx && y<yy)//左下
    {
        d[0] = Left;
        d[1] = Down;
        d[2] = Right;
        d[3] = Up;
    }
    else{//右上
        d[0] = Right;
        d[1] = Up;
        d[2] = Left;
        d[3] = Down;
    }

    for(int i=0; i<4; i++)
    {
        switch (d[i]) {
        case Left:
            if (tryMoveTo(sq-1))
            {
                return true;
            }
            break;
        case Right:
            if (tryMoveTo(sq+1))
            {
                return true;
            }
            break;
        case Up:
            if (tryMoveTo(sq-11))
            {
                return true;
            }
            break;
        case Down:
            if (tryMoveTo(sq+11))
            {
                return true;
            }
            break;
        default:
            break;
        }
    }
//    //left
//    if (tryMoveTo(sq-1))
//    {
//        return true;
//    }
//    //right
//    if (tryMoveTo(sq+1))
//    {
//        return true;
//    }
//    //up
//    if (tryMoveTo(sq-11))
//    {
//        return true;
//    }
//    //down
//    if (tryMoveTo(sq+11))
//    {
//        return true;
//    }
    m_sqList.removeLast();
    return false;
}

bool FindPath::tryMoveTo(int sq)
{
    if (!m_visit[sq] && m_board[sq]==0)
    {
        if (sq==m_dst)
        {
            m_sqList.append(QString::number(sq));
            qDebug()<<"------Find-----";
            qDebug()<<m_sqList.join("->");

            return true;
        }
        return goFrom(sq);
    }
    return false;

}
