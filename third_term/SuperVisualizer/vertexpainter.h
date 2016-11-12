#ifndef VERTEXPAINTER_H
#define VERTEXPAINTER_H

#include <QWidget>

class VertexPainter : public QWidget
{
    Q_OBJECT
public:
    explicit VertexPainter(QWidget *parent = 0, size_t thisNodeNumber = 0, const double &X = 0, const double &Y = 0);
private:
    size_t thisNodeNumber;
    double XCoordinate;
    double YCoordinate;
    void paintEvent(QPaintEvent *event);
signals:

public slots:
};

#endif // VERTEXPAINTER_H
