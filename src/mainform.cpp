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
enum BallType{NullBall, BlueBall, LimeBall, RedBall, AquaBall, OrangeBall, FuchsiaBall ,BlackBall, WhiteBall};

int board[121] = {
    99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
    99, 00, 00, 00, 00, 00, 00, 00, 00, 00, 99,
    99, 00, 00, 00, 00, 00, 00, 00, 00, 00, 99,
    99, 00, 00, 00, 00, 00, 00, 00, 00, 00, 99,
    99, 00, 00, 00, 00, 00, 00, 00, 00, 00, 99,
    99, 00, 00, 00, 00, 00, 00, 00, 00, 00, 99,
    99, 00, 00, 00, 00, 00, 00, 00, 00, 00, 99,
    99, 00, 00, 00, 00, 00, 00, 00, 00, 00, 99,
    99, 00, 00, 00, 00, 00, 00, 00, 00, 00, 99,
    99, 00, 00, 00, 00, 00, 00, 00, 00, 00, 99,
    99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99};
int score[16]={0, 0, 0, 0, 100, 200, 350, 550, 800, 1100, 1450, 1850, 2300, 2800, 3350, 3950};

const int PieceSize = 54;

//int board[121] = {
//             99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
//             99, 01, 02, 03, 04, 05, 06, 07,  8,  1, 99,
//             99, 02, 03, 04, 00, 00, 00, 00, 00, 00, 99,
//             99, 03, 00, 05, 06, 00, 00, 00, 00, 00, 99,
//             99, 04, 00, 00, 07,  8,  9, 00, 00, 00, 99,
//             99, 05, 00, 00, 00, 00, 00, 00, 00, 00, 99,
//             99, 06, 00, 00, 00, 00, 00, 00, 00, 00, 99,
//             99, 07, 00, 00, 00, 00, 00, 00, 00, 00, 99,
//             99,  8, 00, 00, 00, 00, 00, 00, 00, 00, 99,
//             99,  1, 00, 00, 00, 00, 00, 00, 00, 00, 99,
//             99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99};
int X(int sq)
{
    return sq%11-1;
}

int Y(int sq)
{
    return sq/11-1;
}

int SQ(int x, int y)
{
    return (y+1)*11 + x + 1;
}


MainForm::MainForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainForm)
{
    ui->setupUi(this);
    setWindowTitle(tr("七彩连珠"));
    m_selectBallSq = -1;
    m_score = 0;
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


    //随机种子
    qsrand(time(0));
    //随机5个棋子
    int count = 0;
    while(count<5)
    {
        int r = qrand()%81;
        int x = r%9;
        int y = r/9;
        int sq = SQ(x, y);
        if (board[sq]==0)
        {
            board[sq] = qrand()% 8 + 1;
            count++;
        }
    }

    genNextBalls();
}

MainForm::~MainForm()
{
    delete ui;
}

void MainForm::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.fillRect(0, 0, 9*PieceSize, 9*PieceSize, QColor(144, 144, 133));
    for(int i=0; i<=9; i++)
    {
        p.drawLine(0, i*PieceSize, 9*PieceSize, i*PieceSize);
        p.drawLine(i*PieceSize, 0, i*PieceSize, 9*PieceSize);
    }
    for(int x=0; x<9; x++)
    {
        for(int y=0; y<9; y++)
        {
            int sq = SQ(x, y);
            if (board[sq]>0)
            {
                int sq = SQ(x, y);
                if (sq==m_selectBallSq)
                {
                    switch (m_selectBallAnimationIndex) {
                    case 0:
                        draw(&p, x, y, m_images48[board[sq]-1]);
                        break;
                    case 1:
                        draw(&p, x, y, m_images44[board[sq]-1]);
                        break;
                    case 2:
                        draw(&p, x, y, m_images40[board[sq]-1]);
                        break;
                    case 3:
                        draw(&p, x, y, m_images36[board[sq]-1]);
                        break;
                    case 4:
                        draw(&p, x, y, m_images32[board[sq]-1]);
                        break;
                    case 5:
                        draw(&p, x, y, m_images28[board[sq]-1]);
                        break;
                    case 6:
                        draw(&p, x, y, m_images20[board[sq]-1]);
                        break;
                    default:
                        break;
                    }

                }
                else{
                    draw(&p, x, y, m_images48[board[sq]-1]);
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
    if (x<9 && y<9)
    {
        int sq = SQ(x, y);
        int ball = board[sq];
        if (ball>0 && sq!=m_selectBallSq)
        {
            m_selectBallSq = sq;
            m_selectBallAnimationIndex = 0;
            update();
            return;
        }
        if (ball==0 && m_selectBallSq>=0)
        {
            if (findPath(m_selectBallSq, sq))
            {
                m_selectBallSq = -1;
                startMovePath();//do noting

            }
            return;
        }
    }
}

void MainForm::on_btnTest_clicked()
{
    static int index = -1;
    index++;
    if (index>=m_images48.count())
    {
        index = 0;
    }
    ui->label->setPixmap(m_images48[index]);
}

void MainForm::onGenerateBallTimeout()
{
    while(true)
    {
        int r = qrand()%81;
        int x = r%9;
        int y = r/9;
        int sq = SQ(x, y);
        if (board[sq]==0)
        {
            board[sq] = m_nextBalls[m_generateBallCount-1];
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
    if (m_selectBallSq>0)
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
        board[dst] = board[src];
        board[src] = NullBall;
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
    FindPath fp(src, dst, board);
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
        m_nextBalls[i] = qrand()% 8 + 1;
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
    for(int x=0; x<9; x++)
    {
        for(int y=0; y<9; y++)
        {
            int sq = SQ(x, y);
            if (board[sq]==0)
            {
                return false;
            }
        }
    }
    repaint();
    QMessageBox::information(this, windowTitle(), tr("挑战失败!"));
    return true;
}

bool MainForm::checkScore()
{
    int snapshot[121];
    std::memcpy(snapshot, board, sizeof(int)*121);

    QVector< QVector<int> > clearItemList;
    for(int x=0; x<9; x++)
    {
        for(int y=0; y<9; y++)
        {
            int sq = SQ(x, y);
            int ball = snapshot[sq];
            if (ball==0 || ball==8)
            {
                continue;
            }
            //横线
            QVector<int> horzItems;
            horzItems.push_back(sq);
            for(int xx=x-1; xx>=0; xx--)
            {
                int sq = SQ(xx, y);
                int b = snapshot[sq];
                if (b!=8 && b!=ball)
                {
                    break;
                }
                horzItems.push_back(sq);
            }
            for(int xx=x+1; xx<9; xx++)
            {
                int sq = SQ(xx, y);
                int b = snapshot[sq];
                if (b!=8 && b!=ball)
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
                        snapshot[sq] = 0;
                    }
                }
                clearItemList.push_back(horzItems);
            }

            //竖线
            QVector<int> vertItems;
            vertItems.push_back(sq);
            for(int yy=y-1; yy>=0; yy--)
            {
                int sq = SQ(x, yy);
                int b = snapshot[sq];
                if (b!=8 && b!=ball)
                {
                    break;
                }
                vertItems.push_back(sq);
            }
            for(int yy=y+1; yy<9; yy++)
            {
                int sq = SQ(x, yy);
                int b = snapshot[sq];
                if (b!=8 && b!=ball)
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
                        snapshot[sq] = 0;
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
                    int sq = SQ(xx, yy);
                    int b = snapshot[sq];
                    if (b!=8 && b!=ball)
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
                    int sq = SQ(xx, yy);
                    int b = snapshot[sq];
                    if (b!=8 && b!=ball)
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
                        snapshot[sq] = 0;
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
                    int sq = SQ(xx, yy);
                    int b = snapshot[sq];
                    if (b!=8 && b!=ball)
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
                    int sq = SQ(xx, yy);
                    int b = snapshot[sq];
                    if (b!=8 && b!=ball)
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
                        snapshot[sq] = 0;
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
                board[sq] = NullBall;
            }
        }
        if (m_selectBallSq>=0 && snapshot[m_selectBallSq]==0)
        {
            m_selectBallSq = -1;
        }
        qDebug()<<"scoreBallCount="<<scoreBallCount;
        m_score += score[scoreBallCount-1];
        ui->lblScore->setText(QString::number(m_score));
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
