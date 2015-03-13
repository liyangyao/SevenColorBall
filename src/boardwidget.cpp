/****************************************************************************

Creator: liyangyao@gmail.com
Date: 2015/3/13

****************************************************************************/
#pragma execution_character_set("utf-8")

#include "boardwidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QStringList>
#include <QtAlgorithms>
#include "findpath.h"

BoardWidget::BoardWidget(QWidget *parent) :
    QWidget(parent),
    m_board(new Board(8, 9))
{
    Pixmaps pixmaps48 = loadPixmaps(48);
    m_pixmapsVector.push_back(pixmaps48);
    Pixmaps pixmaps40 = loadPixmaps(40);
    m_pixmapsVector.push_back(pixmaps40);
    Pixmaps pixmaps36 = loadPixmaps(36);
    m_pixmapsVector.push_back(pixmaps36);
    Pixmaps pixmaps32 = loadPixmaps(32);
    m_pixmapsVector.push_back(pixmaps32);
    Pixmaps pixmaps28 = loadPixmaps(28);
    m_pixmapsVector.push_back(pixmaps28);
    Pixmaps pixmaps20 = loadPixmaps(20);
    m_pixmapsVector.push_back(pixmaps20);
    m_selectBallAnimationIndex = 0;

    m_selectTimerId = startTimer(70);
    m_moveTimerId = startTimer(50);
    for(int i=0; i<3; i++)
    {
        m_nextBalls[i] = m_board->randomBall();
    }
}

BoardWidget::Pixmaps BoardWidget::loadPixmaps(int size)
{
    Pixmaps pixmaps;
    QString fileName = QString(":/res/%1.png").arg(size);
    QImage image;
    image.load(fileName, "PNG");
    pixmaps.push_back(QPixmap());
    for(int i=0; i<8; i++)
    {
        int y = i/4*size;
        int x = i%4*size;
        pixmaps.push_back(QPixmap::fromImage(image.copy(x, y, size, size)));
    }
    return pixmaps;
}

QPixmap BoardWidget::getPixmap(Board::BallType ball, int index)
{
    int key = ball*1000 + index;
    auto it = m_pixmapCache.find(key);
    if (it==m_pixmapCache.end())
    {
        //qDebug()<<"BoardWidget::getPixmap New "<<key;
        QPixmap orign = m_pixmapsVector.at(index).at(ball);
        QPixmap result = orign.scaled(orign.width()*m_pieceScale,
                                      orign.height()*m_pieceScale, Qt::KeepAspectRatio,
                                      Qt::SmoothTransformation);
        m_pixmapCache.insert(key, result);
        return result;
    }
    else{
        //qDebug()<<"BoardWidget::getPixmap Cache "<<key;
        return it.value();
    }
}

void BoardWidget::drawBall(QPainter *p, Board::BallType ball, int x, int y, int index)
{
    QPixmap pixmap = getPixmap(ball, index);
    p->drawPixmap(x*m_pieceSize + (m_pieceSize-pixmap.width())/2,
                  y*m_pieceSize + (m_pieceSize-pixmap.height())/2, pixmap);
}

bool BoardWidget::findPath(int src, int dst)
{
    FindPath fp(src, dst, m_board.get());
    if (fp.start())
    {
        //再找一次,对调src, dst 谁短找谁
        FindPath fp2(dst, src, m_board.get());
        fp2.start();
        if (fp.path().size()<fp2.path().size())
        {
            m_path = fp.path();
        }
        else{
            m_path = fp2.path();
            qDebug()<<m_path;
            //for(int k=0, s=m_path.size(), max=(s/2); k<max; k++) m_path.swap(k,s-(1+k));
            std::reverse(m_path.begin(), m_path.end());
            qDebug()<<m_path;

        }

        m_lastMoveDst = dst;
        return true;
    }
    return false;

}

void BoardWidget::mousePressEvent(QMouseEvent *event)
{
    int x = event->x() / m_pieceSize;
    int y = event->y() / m_pieceSize;
    if (m_board->inBoard(x, y))
    {
        int sq = m_board->SQ(x, y);
        Board::BallType ball = m_board->getBall(sq);
        if (ball!=Board::NullBall && sq!=m_board->getSqSelected())
        {
            m_board->select(sq);
            m_selectBallAnimationIndex = 0;
            update();
            return;
        }
        if (ball==Board::NullBall && m_board->getSqSelected()>=0)
        {
            if (findPath(m_board->getSqSelected(), sq))
            {
                //m_board->move(m_board->getSqSelected(), sq);
                m_board->select(-1);
                updateDisplay();
                repaint();
            }
            return;
        }
    }
}

void BoardWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawPixmap(0, 0, m_display);
}

void BoardWidget::resizeEvent(QResizeEvent *)
{
    m_display = QPixmap(size());
    const int DEFAULT_PIECE_SIZE = 54;
    int xpieceSize = width() / m_board->colCount();
    int ypieceSize = height() / m_board->rowCount();
    m_pieceSize = xpieceSize<ypieceSize?xpieceSize:ypieceSize;
    m_pieceScale = m_pieceSize / (DEFAULT_PIECE_SIZE + 0.0);
    m_pixmapCache.clear();
    updateDisplay();
}

void BoardWidget::timerEvent(QTimerEvent *e)
{
    bool needRepaint = false;
    //当前选中Ball的动画
    if (e->timerId()==m_selectTimerId && m_board->getSqSelected()>0)
    {
        static int delta = 1;
        const int MaxIndex = 3;
        m_selectBallAnimationIndex += delta;
        if(m_selectBallAnimationIndex<0)
        {
            m_selectBallAnimationIndex = 1;
            delta = 1;
        }
        if (m_selectBallAnimationIndex>MaxIndex)
        {
            m_selectBallAnimationIndex = MaxIndex-1;
            delta = -1;
        }
        //qDebug()<<"m_selectBallAnimationIndex="<<m_selectBallAnimationIndex;
        needRepaint = true;
    }

    if (e->timerId()==m_moveTimerId && m_path.count()>=2)
    {
        int src = m_path[0].toInt();
        int dst = m_path[1].toInt();
        //qDebug()<<"move "<<src<<"->"<<dst;
        m_path.removeFirst();
        m_board->move(src, dst);
        needRepaint = true;
    }
    else if (m_path.count()==1)
    {
        m_path.clear();
//        if (!checkScore())
//        {
//            generateRandom(3);
//        }
    }
    if (needRepaint)
    {
        updateDisplay();
        repaint();
    }
}

void BoardWidget::updateDisplay()
{
    QPainter p(&m_display);
    p.fillRect(rect(), QColor(144, 144, 133));
    for(int col=0; col<=m_board->colCount(); col++)
    {
        p.drawLine(col*m_pieceSize, 0, col*m_pieceSize, m_board->rowCount()*m_pieceSize);
    }
    for(int row=0; row<=m_board->rowCount(); row++)
    {
        p.drawLine(0, row*m_pieceSize, m_board->colCount()*m_pieceSize, row*m_pieceSize);
    }

    for(int x=0; x<m_board->colCount(); x++)
    {
        for(int y=0; y<m_board->rowCount(); y++)
        {
            int sq = m_board->SQ(x, y);
            Board::BallType ball = m_board->getBall(sq);
            //qDebug()<<"sq="<<sq<< "ball="<<ball;
            if (ball!=Board::NullBall)
            {
                int sq = m_board->SQ(x, y);
                if (sq==m_board->getSqSelected())
                {
                    drawBall(&p, ball, x, y, m_selectBallAnimationIndex);
                }
                else{
                    drawBall(&p, ball, x, y, 0);
                }
            }
        }
    }
//    //画下一轮
//    for(int i=0; i<3; i++)
//    {
//        p.drawPixmap(i*PieceSize + 490, 140, m_images48[m_nextBalls[i]-1]);
//    }
}


