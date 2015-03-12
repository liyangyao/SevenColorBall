#ifndef MAINFORM_H
#define MAINFORM_H

#include <QWidget>
#include <QVector>
#include <QTimer>
#include "board.h"

namespace Ui {
class MainForm;
}

class MainForm : public QWidget
{
    Q_OBJECT

public:
    explicit MainForm(QWidget *parent = 0);
    ~MainForm();


private slots:
    void on_btnTest_clicked();
    void onGenerateBallTimeout();
    void onAnimationTimeout();

private:
    Ui::MainForm *ui;
    QVector<QPixmap> m_images48;
    QVector<QPixmap> m_images44;
    QVector<QPixmap> m_images40;
    QVector<QPixmap> m_images36;
    QVector<QPixmap> m_images32;
    QVector<QPixmap> m_images28;
    QVector<QPixmap> m_images20;
    QStringList m_path;
    int m_lastMoveDst;
    Board::BallType m_nextBalls[3];
    bool findPath(int src, int dst);
    QTimer m_generateBallTimer;
    QTimer m_animationTimer;
    int m_generateBallCount;
    int m_selectBallAnimationIndex;
    Board m_board;
    void loadImages(QVector<QPixmap> &images, const QString& fileName, int size);
    void genNextBalls();
    void generateRandom(int count);
    bool checkGameOver();
    bool checkScore();
    void startMovePath();
    void draw(QPainter *p, int x, int y, const QPixmap &ball);
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
};

#endif // MAINFORM_H
