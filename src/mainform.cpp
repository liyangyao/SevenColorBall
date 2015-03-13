#include "mainform.h"
#include "ui_mainform.h"
#include <QImage>
#include <QPainter>
#include <time.h>
#include <QMouseEvent>
#include <QDebug>
#include <QMessageBox>
#include "findpath.h"
#include <cstring>

#pragma execution_character_set("UTF-8")

int score[16]={0, 0, 0, 0, 100, 200, 350, 550, 800, 1100, 1450, 1850, 2300, 2800, 3350, 3950};

const int PieceSize = 54;

MainForm::MainForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainForm),
    m_board(7, 9)
{
    ui->setupUi(this);
    setWindowTitle(tr("七彩连珠"));
    connect(&m_generateBallTimer, SIGNAL(timeout()), this, SLOT(onGenerateBallTimeout()));
    connect(&m_animationTimer, SIGNAL(timeout()), this, SLOT(onAnimationTimeout()));
    m_animationTimer.start(70);

    loadImages(m_images48, ":/res/48.png", 48);
    loadImages(m_images44, ":/res/44.png", 44);
    loadImages(m_images40, ":/res/40.png", 40);
    loadImages(m_images36, ":/res/36.png", 36);
    loadImages(m_images32, ":/res/32.png", 32);
    loadImages(m_images28, ":/res/28.png", 28);
    loadImages(m_images20, ":/res/20.png", 20);

    genNextBalls();
}

MainForm::~MainForm()
{
    delete ui;
}

void MainForm::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.fillRect(0, 0, m_board.colCount()*PieceSize, m_board.rowCount()*PieceSize, QColor(144, 144, 133));
    for(int col=0; col<=m_board.colCount(); col++)
    {
        p.drawLine(col*PieceSize, 0, col*PieceSize, m_board.rowCount()*PieceSize);
    }
    for(int row=0; row<=m_board.rowCount(); row++)
    {
        p.drawLine(0, row*PieceSize, m_board.colCount()*PieceSize, row*PieceSize);
    }

    for(int x=0; x<m_board.colCount(); x++)
    {
        for(int y=0; y<m_board.rowCount(); y++)
        {
            int sq = m_board.SQ(x, y);
            Board::BallType ball = m_board.getBall(sq);
            //qDebug()<<"sq="<<sq<< "ball="<<ball;
            if (ball!=Board::NullBall)
            {
                int sq = m_board.SQ(x, y);
                if (sq==m_board.getSqSelected())
                {
                    switch (m_selectBallAnimationIndex) {
                    case 0:
                        draw(&p, x, y, m_images48[ball-1]);
                        break;
                    case 1:
                        draw(&p, x, y, m_images44[ball-1]);
                        break;
                    case 2:
                        draw(&p, x, y, m_images40[ball-1]);
                        break;
                    case 3:
                        draw(&p, x, y, m_images36[ball-1]);
                        break;
                    case 4:
                        draw(&p, x, y, m_images32[ball-1]);
                        break;
                    case 5:
                        draw(&p, x, y, m_images28[ball-1]);
                        break;
                    case 6:
                        draw(&p, x, y, m_images20[ball-1]);
                        break;
                    default:
                        break;
                    }

                }
                else{
                    draw(&p, x, y, m_images48[ball-1]);
                }
            }
        }
    }
    //画下一轮
    for(int i=0; i<3; i++)
    {
        p.drawPixmap(i*PieceSize + 490, 140, m_images48[m_nextBalls[i]-1]);
    }
}

void MainForm::mousePressEvent(QMouseEvent *event)
{
    int x = event->x() / PieceSize;
    int y = event->y() / PieceSize;
    if (m_board.inBoard(x, y))
    {
        int sq = m_board.SQ(x, y);
        Board::BallType ball = m_board.getBall(sq);
        if (ball!=Board::NullBall && sq!=m_board.getSqSelected())
        {
            m_board.select(sq);
            m_selectBallAnimationIndex = 0;
            update();
            return;
        }
        if (ball==Board::NullBall && m_board.getSqSelected()>=0)
        {
            if (findPath(m_board.getSqSelected(), sq))
            {
                //m_board.move(m_board.getSqSelected(), sq);
                m_board.select(-1);
                repaint();
                //startMovePath();//do noting


            }
            return;
        }
    }
}

void MainForm::on_btnTest_clicked()
{
    m_board.reset();
    ui->lblScore->setText(QString::number(m_board.score()));
    update();
}

void MainForm::onGenerateBallTimeout()
{
    while(true)
    {
        int r = qrand()%(m_board.colCount()*m_board.rowCount());
        int x = r%m_board.colCount();
        int y = r/m_board.colCount();
        int sq = m_board.SQ(x, y);
        Board::BallType ball = m_board.getBall(sq);
        if (ball==Board::NullBall)
        {
            m_board.put(m_nextBalls[m_generateBallCount-1], x, y);
            break;
        }
    }
    m_generateBallCount--;
    checkScore();

    if (checkGameOver()||m_generateBallCount==0)
    {
        genNextBalls();
        m_generateBallTimer.stop();
    }
    update();
}

void MainForm::onAnimationTimeout()
{
    bool needRepaint = false;
    //当前选中Ball的动画
    if (m_board.getSqSelected()>0)
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
    if (m_path.count()>=2)
    {
        int src = m_path[0].toInt();
        int dst = m_path[1].toInt();
        //qDebug()<<"move "<<src<<"->"<<dst;
        m_path.removeFirst();
        m_board.move(src, dst);
        needRepaint = true;
    }
    else if (m_path.count()==1)
    {

        m_path.clear();
        if (!checkScore())
        {
            generateRandom(3);
        }
    }
    if (needRepaint)
    {
        repaint();
    }
}

bool MainForm::findPath(int src, int dst)
{
    FindPath fp(src, dst, &m_board);
    if (fp.start())
    {
        m_path = fp.path();
        m_lastMoveDst = dst;
        return true;
    }
    return false;

}

void MainForm::loadImages(QVector<QPixmap> &images, const QString &fileName, int size)
{
    QImage image;
    image.load(fileName, "PNG");
    for(int i=0; i<8; i++)
    {
        int y = i/4*size;
        int x = i%4*size;
        images.push_back(QPixmap::fromImage(image.copy(x, y, size, size)));
    }
}

void MainForm::genNextBalls()
{
    for(int i=0; i<3; i++)
    {
        m_nextBalls[i] = m_board.randomBall();
    }
}

void MainForm::generateRandom(int count)
{
    if (!checkGameOver())
    {
        m_generateBallCount = count;
        m_generateBallTimer.start(250);
    }
}

bool MainForm::checkGameOver()
{
    if (m_board.isGameOver())
    {
        QMessageBox::information(this, windowTitle(), tr("挑战失败!"));
        return true;
    }
    else{
        return false;
    }
}

bool MainForm::checkScore()
{
    Board snapshot = m_board;


    QVector< QVector<int> > clearItemList;
    for(int x=0; x<m_board.colCount(); x++)
    {
        for(int y=0; y<m_board.rowCount(); y++)
        {
            int sq = m_board.SQ(x, y);
            Board::BallType ball = snapshot.getBall(sq);
            if (ball==Board::NullBall || ball==Board::WhiteBall)
            {
                continue;
            }
//            //横线
            QVector<int> horzItems;
            horzItems.push_back(sq);
            for(int xx=x-1; xx>=0; xx--)
            {
                int sq = m_board.SQ(xx, y);
                int b = snapshot.getBall(sq);
                if (b!=Board::WhiteBall && b!=ball)
                {
                    break;
                }
                horzItems.push_back(sq);
            }
            for(int xx=x+1; xx<m_board.rowCount(); xx++)
            {
                int sq = m_board.SQ(xx, y);
                int b = snapshot.getBall(sq);
                if (b!=Board::WhiteBall && b!=ball)
                {
                    break;
                }
                horzItems.push_back(sq);
            }
            if (horzItems.count()>=5)
            {
                foreach(int sq, horzItems)
                {
                    if (sq!=m_lastMoveDst)
                    {
                        snapshot.put(Board::NullBall, sq);
                    }
                }
                clearItemList.push_back(horzItems);
            }

            //竖线
            QVector<int> vertItems;
            vertItems.push_back(sq);
            for(int yy=y-1; yy>=0; yy--)
            {
                int sq = m_board.SQ(x, yy);
                int b = snapshot.getBall(sq);
                if (b!=Board::WhiteBall && b!=ball)
                {
                    break;
                }
                vertItems.push_back(sq);
            }
            for(int yy=y+1; yy<m_board.rowCount(); yy++)
            {
                int sq = m_board.SQ(x, yy);
                int b = snapshot.getBall(sq);
                if (b!=Board::WhiteBall && b!=ball)
                {
                    break;
                }
                vertItems.push_back(sq);
            }
            if (vertItems.count()>=5)
            {
                foreach(int sq, vertItems)
                {
                    if (sq!=m_lastMoveDst)
                    {
                        snapshot.put(Board::NullBall, sq);
                    }
                }
                clearItemList.push_back(vertItems);
            }

            //斜线
            QVector<int> xItems;
            xItems.push_back(sq);
            {
                int xx = x - 1;
                int yy = y - 1;
                while(1)
                {
                    int sq = m_board.SQ(xx, yy);
                    int b = snapshot.getBall(sq);
                    if (b!=Board::WhiteBall && b!=ball)
                    {
                        break;
                    }
                    xItems.push_back(sq);
                    xx--;
                    yy--;
                }
            }
            {
                int xx = x + 1;
                int yy = y + 1;
                while(1)
                {
                    int sq = m_board.SQ(xx, yy);
                    int b = snapshot.getBall(sq);
                    if (b!=Board::WhiteBall && b!=ball)
                    {
                        break;
                    }
                    xItems.push_back(sq);
                    xx++;
                    yy++;
                }
            }
            if (xItems.count()>=5)
            {
                foreach(int sq, xItems)
                {
                    if (sq!=m_lastMoveDst)
                    {
                        snapshot.put(Board::NullBall, sq);
                    }
                }
                clearItemList.push_back(xItems);
            }

            QVector<int> yItems;
            yItems.push_back(sq);
            {
                int xx = x + 1;
                int yy = y - 1;
                while(1)
                {
                    int sq = m_board.SQ(xx, yy);
                    int b = snapshot.getBall(sq);
                    if (b!=Board::WhiteBall && b!=ball)
                    {
                        break;
                    }
                    yItems.push_back(sq);
                    xx++;
                    yy--;
                }
            }
            {
                int xx = x - 1;
                int yy = y + 1;
                while(1)
                {
                    int sq = m_board.SQ(xx, yy);
                    int b = snapshot.getBall(sq);
                    if (b!=Board::WhiteBall && b!=ball)
                    {
                        break;
                    }
                    yItems.push_back(sq);
                    xx--;
                    yy++;
                }
            }
            if (yItems.count()>=5)
            {
                foreach(int sq, yItems)
                {
                    if (sq!=m_lastMoveDst)
                    {
                        snapshot.put(Board::NullBall, sq);
                    }
                }
                clearItemList.push_back(yItems);
            }
        }
    }
    if (clearItemList.count()>0)
    {
        int scoreBallCount = 0;
        foreach(const QVector<int> &items, clearItemList)
        {
            scoreBallCount += items.count();
            foreach(int sq, items)
            {
                m_board.put(Board::NullBall, sq);
            }
        }
        if (m_board.getSqSelected()>=0 && m_board.getBall(m_board.getSqSelected())==Board::NullBall)
        {
            m_board.select(-1);
        }
        qDebug()<<"scoreBallCount="<<scoreBallCount;
        m_board.addScore(score[scoreBallCount-1]);
        ui->lblScore->setText(QString::number(m_board.score()));
        repaint();
        return true;
    }
    return false;

}

void MainForm::startMovePath()
{

}

void MainForm::draw(QPainter *p, int x, int y, const QPixmap &ball)
{
    p->drawPixmap(x*PieceSize + (PieceSize-ball.width())/2, y*54 + (PieceSize-ball.height())/2, ball);
}
