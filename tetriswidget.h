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
    int fillEmptyGrids();
    void generateBlock();
    void copyToCurrentBlock(int block[4][4]);
    void rotate();
    void mapTotheGrid();
    QPoint getPicePossition(int x, int y);
    void drawFallingFigure(QPainter *painter);
    void bottomCollision();
    bool piceCollision();
    void leftCollision();
    void rightCollision();
    void wallCollision();
    void restartGame();
    void deletingLines();
    void deleteLine(int row);

   // QSize sizeHint() const { return QSize(200, 400);}

protected slots:
    void falling();

protected:
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    const double CellWidth = width() / double(WIDTH);
    const double CellHeight = height() / double(HEIGHT);

    enum {Empty = 0, Part};
    int gameGrid[WIDTH][HEIGHT];

    QPoint currBlockPos;
    int currBlock[4][4];
    QTimer *timer;

    bool landed;
    int interval;
    int score;
    QString title;


};
#endif // TETRISWIDGET_H
