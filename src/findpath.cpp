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
    qDebug()<<m_sqList.join("->");
    //left
    if (tryMoveTo(sq-1))
    {
        return true;
    }
    //right
    if (tryMoveTo(sq+1))
    {
        return true;
    }
    //up
    if (tryMoveTo(sq-11))
    {
        return true;
    }
    //down
    if (tryMoveTo(sq+11))
    {
        return true;
    }
    m_sqList.removeLast();
    return false;
}

bool FindPath::tryMoveTo(int sq)
{
    if (!m_visit[sq] && m_board[sq]==0)
    {
        if (sq==m_dst)
        {
            qDebug()<<"------Find-----";
            qDebug()<<m_sqList.join("->");
            return true;
        }
        return goFrom(sq);
    }
    return false;

}
