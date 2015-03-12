#ifndef BOARD_H
#define BOARD_H

#include <QObject>
#include <vector>

class Board
{
public:
    explicit Board(int colCount, int rowCount);
    enum BallType{NullBall, BlueBall, LimeBall, RedBall, AquaBall, OrangeBall,
                  FuchsiaBall ,BlackBall, WhiteBall, Edge=99};
    void put(BallType ball, int x, int y);
    void put(BallType ball, int sq){ m_data[sq] = ball; };
    void take(int x, int y);
    bool isGameOver();
    void select(int sq);
    int getSqSelected(){ return m_sqSelected; }
    BallType getBall(int sq){ return m_data[sq]; }
    BallType getBall(int x, int y){ return getBall(SQ(x, y)); }
    int SQ(int x, int y) { return (y+1)*m_dataColCount + x + 1;}
    void XY(int sq, int &x, int &y);
    void move(int sqSrc, int sqDst);
    BallType randomBall();
    bool inBoard(int x, int y) {return x<m_colCount && y<m_rowCount; }
    int colCount() { return m_colCount; }
    int rowCount() { return m_rowCount; }
    int dataColCount() { return m_dataColCount; }
    int dataRowCount() { return m_dataRowCount; }
    int dataSize() {return m_data.size(); }
    int score() {return m_score; }
    void addScore(int value) {m_score += value; }
    void reset();


private:
    int m_colCount;
    int m_rowCount;
    int m_dataColCount;
    int m_dataRowCount;
    int m_sqSelected;
    int m_score;
    std::vector<BallType> m_data;


};

#endif // BOARD_H
