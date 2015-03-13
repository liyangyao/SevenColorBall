/****************************************************************************

Creator: liyangyao@gmail.com
Date: 2015/3/13

****************************************************************************/
#pragma execution_character_set("utf-8")

#ifndef BOARDWIDGET_H
#define BOARDWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <memory>
#include <QVector>
#include <QMap>
#include "board.h"

class BoardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BoardWidget(QWidget *parent = 0);

private:
    QPixmap m_display;
    int m_pieceSize;
    double m_pieceScale;
    int m_selectBallAnimationIndex;
    int m_selectTimerId;
    int m_moveTimerId;
    std::unique_ptr<Board> m_board;
    typedef QVector<QPixmap> Pixmaps;
    QVector<Pixmaps> m_pixmapsVector;
    QMap<int, QPixmap> m_pixmapCache;
    QStringList m_path;
    int m_lastMoveDst;
    Board::BallType m_nextBalls[3];

    Pixmaps loadPixmaps(int size);
    QPixmap getPixmap(Board::BallType ball, int index);
    void drawBall(QPainter *p, Board::BallType ball, int x, int y, int index);
    void updateDisplay();

    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);

    void timerEvent(QTimerEvent *);
    void mousePressEvent(QMouseEvent *event);
    bool findPath(int src, int dst);
};

#endif // BOARDWIDGET_H
