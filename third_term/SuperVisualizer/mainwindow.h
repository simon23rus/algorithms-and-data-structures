#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QPushButton>
#include "relabeltofront.h"

struct Dot {
    double XCoordinate;
    double YCoordinate;
};


namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    QTimer *timer;
    double getLengthOfEdge(const Edge &edge);
    double getSquaredLengthBetweenVertecies(const Dot &first, const Dot &second);



     MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
     void letsRepaintNet();

private slots:

private:
    Net myNet;
    std::vector<std::vector<char>> adjacent;
    std::vector<Dot> vertices;
    std::vector<Dot> verticesWithHeights;
    std::vector<Edge> edges;
    std::vector<std::vector<size_t>> matrix;
    size_t vertexNumber;
    size_t height;
    size_t width;
    size_t pushesForInitialize;
    size_t currentPushEdgeNumber;
    size_t currentStepNumber;
    bool thisIsTheEnd;
    std::list<size_t>::iterator position;


    Ui::MainWindow *ui;
    Dot calculateMiddle(const Edge &toCalculate, const bool &withHeight = false);


    void forceMovement();
    void paintEvent(QPaintEvent *event);
    void makeSomeMagic(QPainter *painter);
    void drawVertex(QPainter *painter, const bool &withHeights = false);
    std::pair<Dot, Dot> drawArrow(Edge &toDraw);
    void drawEdge(QPainter *painter, const bool &withHeights = false);
    void generateRandomNet();
    void shiftVertices();
    void keyPressEvent(QKeyEvent *event);
};

#endif
