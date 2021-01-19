#include "tetriswidget.h"
#include <QPainter>
#include <QTime>
#include <QRandomGenerator>
#include <QKeyEvent>
#include <QTimer>


TetrisWidget::TetrisWidget(QWidget *parent)
    : QWidget(parent)
{
    fillEmptyGrids();
    landed = false;

    timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(falling()));
    timer->start(500);

}

TetrisWidget::~TetrisWidget()
{
}

void TetrisWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    drawFallingFigure(&painter);

}

void TetrisWidget::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()){
    case Qt::Key_Space:
        rotate();
        break;
    case Qt::Key_Left:
        currBlockPos.rx() -= 1;
        break;
    case Qt::Key_Right:
        currBlockPos.rx() += 1;
        break;
    case Qt::Key_Down:
        currBlockPos.ry() += 1;
        break;
    default:
        QWidget::keyPressEvent(event);
    }

}



int TetrisWidget::randGenerator(int number) {
    QTime time;
    int seed = time.currentTime().second();

    QRandomGenerator rand;
    rand.seed(seed);
    int x = rand.bounded(number);
    return x;
}

int TetrisWidget::fillEmptyGrids()
{
    for (int i = 0; i < WIDTH; ++i)
        for(int j = 0; j < HEIGHT; ++j)
            gameGrid[i][j] = Empty;
}

void TetrisWidget::generateBlock()
{
    if (!landed) {
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
            break;
        case 3:
            copyToCurrentBlock(fourth);
            break;
        default:
            break;

    }

  }

    landed = true;

}

void TetrisWidget::copyToCurrentBlock(int block[4][4])
{
    for(int i = 0; i < 4; ++i)
        for(int j = 0; j < 4; ++j)
            currBlock[i][j] = block[i][j];
    currBlockPos = QPoint(WIDTH / 2 - 1, 0);
}

void TetrisWidget::rotate()
{
    for(int i = 0; i < 4 / 2; i++)
      {
        for(int j = i; j < 4 - i - 1; j++)
        {
                int temp = currBlock[i][j];
                currBlock[i][j] = currBlock[4 - 1 - j][i];
                currBlock[4 - 1 - j][i] = currBlock[4 - 1 - i][4 - 1 - j];
                currBlock[4 - 1 - i][4 - 1 - j] = currBlock[j][4 - 1 - i];
                currBlock[j][4 - 1 - i] = temp;
        }
    }
}

void TetrisWidget::drawFallingFigure(QPainter *painter)
{
    generateBlock();

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(Qt::red);

    for(int i = 0; i < 4; ++i)
        for(int j = 0; j < 4; ++j)
            if(currBlock[j][i] != 0)
                painter->drawRoundedRect((currBlockPos.x() + i) * CellWidth, (currBlockPos.y() + j) * CellHeight,
                                         CellWidth, CellHeight, 5, 5);
}

void TetrisWidget::falling()
{
    currBlockPos.ry() += 1;
    update();
}

