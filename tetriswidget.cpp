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
    interval = 500;
    score = 0;
    title = "Score: ";

    timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(falling()));
    timer->start(interval);
    setWindowTitle(title + QString::number(score));

}

TetrisWidget::~TetrisWidget()
{
}

void TetrisWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setWindow(rect());
    painter.setViewport(rect());
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
        update();
        break;
    case Qt::Key_Left:
        currBlockPos.rx() -= 1;
        leftCollision();
        update();
        break;
    case Qt::Key_Right:
        currBlockPos.rx() += 1;
        rightCollision();
        update();
        break;
    case Qt::Key_Down:
        currBlockPos.ry() += 1;
        piceCollision();
        bottomCollision();
        update();
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

        int fifth[4][4] = {{0, 0, 1, 0},
                           {0, 1, 1, 1},
                           {0, 0, 0, 0},
                           {0, 0, 0, 0}};

        int six[4][4] = {{1, 0, 0, 0},
                         {1, 1, 1, 0},
                         {0, 0, 0, 0},
                         {0, 0, 0, 0}};

        int seven[4][4] = {{0, 1, 0, 0},
                           {0, 1, 1, 0},
                           {0, 0, 1, 0},
                           {0, 0, 0, 0}};

        int eight[4][4] = {{0, 0, 1, 0},
                           {0, 0, 0, 0},
                           {0, 0, 0, 0},
                           {0, 0, 0, 0}};

        const int randNum = randGenerator(8);
        switch (randNum) {
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
        case 5:
            copyToCurrentBlock(six);
            break;
        case 6:
            copyToCurrentBlock(seven);
            break;
        case 7:
            copyToCurrentBlock(eight);
            break;
        default:
            break;

    }
        qDebug() << randNum;

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
    deletingLines();
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

bool TetrisWidget::piceCollision()
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
                    return true;
                }
            }
        }
    }
    return false;
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
                else if(gameGrid[getPicePossition(i, j).x()][getPicePossition(i, j).y()] == Part)
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
                else if(gameGrid[getPicePossition(i, j).x()][getPicePossition(i, j).y()] == Part)
                    currBlockPos.rx() -= 1;
            }
        }
    }
}

void TetrisWidget::wallCollision()
{
    if(piceCollision())
    {
        for(int i = 0; i < 4; ++i)
            for(int j = 0; j < 4; ++j)
                if(getPicePossition(i, j).y() <= 0)
                {
                    restartGame();
                }

    }
}

void TetrisWidget::restartGame()
{
    fillEmptyGrids();
    score = 0;
    setWindowTitle(title + QString::number(score));
    interval = 500;
    timer->setInterval(interval);
}

void TetrisWidget::deletingLines()
{
    int temp = 0;
    for(int i = HEIGHT - 1; i >= 0; --i)
    {
        for(int j = 0; j < WIDTH; ++j)
        {
            if(gameGrid[j][i] != Part) {
                temp = 0;
                break;
            }
            ++temp;
        }
        if (temp == WIDTH)
            deleteLine(i);
    }
}

void TetrisWidget::deleteLine(int row)
{
    for(int i = row ; i >= 0; --i)
        for(int j = 0; j < WIDTH; ++j) {
                gameGrid[j][i] = gameGrid[j][i - 1];
                if (i == 0)
                    break;
        }
    interval -= interval * 5 / 100;
    score += 100;
    setWindowTitle(title + QString::number(score));
}


void TetrisWidget::falling()
{  
    currBlockPos.ry() += 1;
    wallCollision();
    bottomCollision();
    update();
}

