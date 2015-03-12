#include "board.h"

Board::Board(int colCount, int rowCount):
    m_colCount(colCount),
    m_rowCount(rowCount),
    m_dataColCount(colCount+2),
    m_dataRowCount(rowCount+2)
{
    m_data.resize(m_dataColCount*m_dataRowCount);
    reset();
}

void Board::put(Board::BallType ball, int x, int y)
{
    m_data[SQ(x, y)] = ball;
}

void Board::take(int x, int y)
{
    m_data[SQ(x, y)] = NullBall;
}

bool Board::isGameOver()
{
    for(size_t i=0; i<m_data.size(); i++)
    {
        if (m_data[i]==NullBall)
        {
            return false;
        }
    }
    return true;
}

void Board::select(int sq)
{
    m_sqSelected = sq;
}

void Board::XY(int sq, int &x, int &y)
{
    x = sq%m_dataColCount-1;
    y = sq/m_dataColCount-1;
}

void Board::move(int sqSrc, int sqDst)
{
    qDebug("move %d->%d", sqSrc, sqDst);
    m_data[sqDst] = m_data[sqSrc];
    m_data[sqSrc] = NullBall;
}

void Board::reset()
{
    m_score = 0;
    m_sqSelected = -1;
    int sq = 0;
    for(int i=0; i<m_dataRowCount; i++)
    {
        for(int j=0; j<m_dataColCount; j++)
        {
            if (i==0||j==0||i==(m_dataRowCount-1)||j==(m_dataColCount-1))
            {
                m_data[sq] = Edge;
            }
            else{
                m_data[sq] = NullBall;
            }
            sq++;
        }
    }
    //随机5个棋子
    int count = 0;
    while(count<5)
    {
        int r = qrand()%(m_colCount*m_rowCount);
        int x = r%m_colCount;
        int y = r/m_colCount;
        int sq = SQ(x, y);
        if (m_data[sq]==NullBall)
        {
            m_data[sq] = randomBall();
            count++;
        }
    }
}

Board::BallType Board::randomBall()
{
     return (BallType)(qrand()% 8 + 1);
}
