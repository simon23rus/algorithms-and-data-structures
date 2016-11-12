#include "vertexpainter.h"
#include <fstream>
#include <QPainter>

const size_t WINDOW_SIZE = 60;

VertexPainter::VertexPainter(QWidget *parent, size_t thisNodeNumber, const double &X, const double &Y) : QWidget(parent), thisNodeNumber(thisNodeNumber), XCoordinate(X), YCoordinate(Y)
{

    setGeometry(QRect(0, 0, WINDOW_SIZE,WINDOW_SIZE));
}

void VertexPainter::paintEvent(QPaintEvent *event) {
    QPainter painter(this);


}

