#ifndef MAINFORM_H
#define MAINFORM_H

#include <QWidget>
#include <QVector>
#include <QTimer>

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

private:
    Ui::MainForm *ui;
    QVector<QPixmap> m_images48;
    QVector<QPixmap> m_images32;
    QVector<QPixmap> m_images20;
    QVector<int> m_path;
    int m_nextBalls[3];
    bool findPath(int src, int dst);
    QTimer m_generateBallTimer;
    int m_generateBallCount;
    int m_selectBallSq;
    void loadImages(QVector<QPixmap> &images, const QString& fileName, int size);
    void genNextBalls();
    void generateRandom(int count);
    bool checkGameOver();
    void checkScore();
    void draw(QPainter *p, int x, int y, const QPixmap &ball);
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
};

#endif // MAINFORM_H
