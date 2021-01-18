#include "tetriswidget.h"
#include <QPainter>
#include <QTime>
#include <QRandomGenerator>

TetrisWidget::TetrisWidget(QWidget *parent)
    : QWidget(parent)
{
    initGame();

}

TetrisWidget::~TetrisWidget()
{
}

void TetrisWidget::paintEvent(QPaintEvent *event)
{
    double cellWidth = width() / double(WIDTH);
    double cellHeight = height() / double(HEIGHT);

    generateBlock();

    currBlockPos = QPoint(WIDTH / 2 - 1, 0);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(Qt::red);

    for(int i = 0; i < 4; ++i)
        for(int j = 0; j < 4; ++j)
            if(currBlock[i][j] != 0)
                painter.drawRoundRect((currBlockPos.x() + i) * cellWidth,
                                      (currBlockPos.y() + j) * cellHeight,
                                      cellWidth, cellHeight, 10);

}



int TetrisWidget::randGenerator(int number) {
    QTime time;
    int seed = time.currentTime().second();

    QRandomGenerator rand;
    rand.seed(seed);
    int x = rand.bounded(number);
    return x;
}

int TetrisWidget::initGame()
{
    for (int i = 0; i < WIDTH; ++i)
        for(int j = 0; j < HEIGHT; ++j)
            gameGrid[i][j] = Empty;
}

void TetrisWidget::generateBlock()
{
    int first[4][4] = {{0, 1, 1, 0},
                       {0, 1, 1, 0},
                       {0, 0, 0, 0},
                       {0, 0, 0, 0}};

    int second[4][4] = {{0, 0, 1, 0},
                        {0, 1, 1, 0},
                        {0, 1, 0, 0},
                        {0, 0, 0, 0}};

    int third[4][4] = {{0, 0, 1, 0},
                       {0, 0, 1, 0},
                       {0, 0, 1, 0},
                       {0, 0, 1, 0}};

    int fourth[4][4] = {{1, 0, 0, 0},
                        {1, 1, 1, 0},
                        {0, 0, 0, 0},
                        {0, 0, 0, 0}};

    const int rand = randGenerator(4);

    switch (rand) {
    case 0:
        copyToCurrentBlock(first);
        break;
    case 1:
        copyToCurrentBlock(second);
        break;
    case 2:
        copyToCurrentBlock(third);
    case 3:
        copyToCurrentBlock(fourth);
        break;
    default:
        break;

    }
}

void TetrisWidget::copyToCurrentBlock(int block[4][4])
{
    for(int i = 0; i < 4; ++i)
        for(int j = 0; j < 4; ++j)
           currBlock[i][j] = block[i][j];
}

