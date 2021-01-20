#include "tetriswidget.h"
#include <QPainter>
#include <QTime>
#include <QRandomGenerator>
#include <QKeyEvent>
#include <QTimer>
#include <QApplication>
#include <QScreen>
#include <QDebug>


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

    for(int i = 0; i < WIDTH; ++i)
        for(int j = 0; j < HEIGHT; ++j)
            if(gameGrid[i][j] == Part)
                painter.drawRoundRect(i * CellWidth, j * CellHeight, CellWidth - 1 , CellHeight - 1,
                                      5, 5);

}

void TetrisWidget::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()){
    case Qt::Key_Space:
        rotate();
        leftCollision();
        rightCollision();
        break;
    case Qt::Key_Left:
        currBlockPos.rx() -= 1;
        leftCollision();
        break;
    case Qt::Key_Right:
        currBlockPos.rx() += 1;
        rightCollision();
        break;
    case Qt::Key_Down:
        currBlockPos.ry() += 1;
        piceCollision();
        bottomCollision();
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

        int fourth[4][4] = {{0, 0, 1, 0},
                            {1, 1, 1, 0},
                            {0, 0, 0, 0},
                            {0, 0, 0, 0}};

        int fifth[4][4] = {{0, 0, 0, 0},
                           {0, 0, 1, 0},
                           {0, 1, 1, 1},
                           {0, 0, 0, 0}};

        const int rand = randGenerator(5);

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
        case 4:
            copyToCurrentBlock(fifth);
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

    currBlockPos = QPoint(WIDTH / 2 - 2, 0);
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


void TetrisWidget::mapTotheGrid()
{
    for(int i = 0; i < 4; ++i)
        for(int j = 0; j < 4; ++j)
            if(currBlock[j][i] == 1)
            {
                QPoint pos = getPicePossition(i, j);
                gameGrid[pos.x()][pos.y()] = Part;
            }
    for(int i = 0; i < 4; ++i)
        for(int j = 0; j < 4; ++j)
            currBlock[i][j] = 0;
}


QPoint TetrisWidget::getPicePossition(int x, int y)
{
    return QPoint(currBlockPos.x() + x, currBlockPos.y() + y);
}

void TetrisWidget::drawFallingFigure(QPainter *painter)
{
    generateBlock();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(Qt::red);

    for(int i = 0; i < 4; ++i)
    {
        for(int j = 0; j < 4; ++j)
        {
            if(currBlock[j][i] == 1)
            {
                QPoint pos = getPicePossition(i, j);
                painter->drawRoundRect(pos.x() * CellWidth, pos.y() * CellHeight,
                                       CellWidth, CellHeight, 5, 5);
            }
        }
    }
}

void TetrisWidget::bottomCollision()
{
    piceCollision();
    for(int i = 0; i < 4; ++i)
    {
        for(int j = 0; j < 4; ++j)
        {
            if(currBlock[j][i] == 1)
                if(getPicePossition(i, j).y() == HEIGHT - 1)
                {
                    landed = false;
                    mapTotheGrid();
                }
        }
    }
}

void TetrisWidget::piceCollision()
{
    for(int i = 0; i < 4; ++i)
    {
        for(int j = 0; j < 4; ++j)
        {
            if(currBlock[j][i] == 1)
            {
                QPoint pos = getPicePossition(i, j);
                if(gameGrid[pos.x()][pos.y()] == Part)
                {
                    currBlockPos.ry() -= 1;
                    landed = false;
                    mapTotheGrid();
                }
            }
        }
    }
}

void TetrisWidget::leftCollision()
{
    for(int i = 0; i < 4; ++i)
    {
        for(int j = 0; j < 4; ++j)
        {
            if(currBlock[j][i] == 1)
            {
                if(getPicePossition(i, j).x() < 0)
                    while(getPicePossition(i, j).x() != 0)
                        currBlockPos.rx() += 1;
            }
        }
    }
}

void TetrisWidget::rightCollision()
{
    for(int i = 0; i < 4; ++i)
    {
        for(int j = 0; j < 4; ++j)
        {
            if(currBlock[j][i] == 1)
            {
                if(getPicePossition(i, j).x() >= WIDTH)
                    while(getPicePossition(i, j).x() != WIDTH - 1)
                        currBlockPos.rx() -= 1;
            }
        }
    }
}

void TetrisWidget::deletingLines()
{

}


void TetrisWidget::falling()
{
    currBlockPos.ry() += 1;
    bottomCollision();
    update();
}

