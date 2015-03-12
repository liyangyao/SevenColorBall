/****************************************************************************

Creator: liyangyao@gmail.com
Date: 2015/3/11

****************************************************************************/
#pragma execution_character_set("utf-8")

#ifndef FINDPATH_H
#define FINDPATH_H

#include <QVector>
#include <QStringList>
#include "board.h"

class FindPath
{
public:
    FindPath(int src, int dst, Board &board);
    bool start();
    QStringList path()
    {
        return m_sqList;
    }

private:
    int m_src;
    int m_dst;
    Board& m_board;
    bool goFrom(int sq);
    bool tryMoveTo(int sq);
    QVector<bool> m_visit;
    QStringList m_sqList;
};

#endif // FINDPATH_H
