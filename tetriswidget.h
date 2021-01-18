#ifndef TETRISWIDGET_H
#define TETRISWIDGET_H

#include <QWidget>

#define WIDTH 20
#define HEIGHT 20

class TetrisWidget : public QWidget
{
    Q_OBJECT

public:
    TetrisWidget(QWidget *parent = nullptr);
    ~TetrisWidget();

    int randGenerator(int);
    int initGame();
    void generateBlock();
    void copyToCurrentBlock(int block[4][4]);

protected:
    void paintEvent(QPaintEvent *event);

private:
    enum {Empty = 0, Part};
    int gameGrid[WIDTH][HEIGHT];

    QPoint currBlockPos;
    int currBlock[4][4];
    QTimer *time;

};
#endif // TETRISWIDGET_H
