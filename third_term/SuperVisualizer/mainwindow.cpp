#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <random>
#include <QPainter>
#include <QVector2D>
#include <QDebug>
#include <QKeyEvent>
#include <QImage>
#include <QDesktopWidget>
#include <QSound>
#include <fstream>
#include <unistd.h>

static const short REFRESH_TIME = 1000 / 60;
static const short EPSILA = 100;
static const short EPSILA_POPOLAM = 50;
std::random_device monster;
std::mt19937 justRandom(monster());
std::uniform_int_distribution<int> vertexNumberRandomizer(6, 9);
std::uniform_int_distribution<int> amountRandomizer(1, 10);
std::uniform_int_distribution<int> sigmaRandomizer(80, 150);
std::normal_distribution<double> verticeDistribution (0.0, sigmaRandomizer(justRandom));
std::uniform_real_distribution<double> coordinateX(0.0228, 0.48228);
std::uniform_real_distribution<double> coordinateY(0.0228, 0.98228);


/* TODO LIST!
 * 1. Split net class to 2 parts: Net and Algorithm
 * 2. Add Setters and getters.
 * 3. refactoring
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */

void say(const std::string &whoSayThat, const std::string &toSay)
{
    std::string saying = "say -v " + whoSayThat + " " + toSay;
    if (fork() == 0)
    {
        char* voiceArgs[] = {"/bin/bash", "-c", const_cast<char*>(saying.c_str()), NULL};
        execv(voiceArgs[0], voiceArgs);
        exit(1);
    }
    while(wait(0) > 0);
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    say("Trinoids", "Hi");
    thisIsTheEnd = false;
    currentPushEdgeNumber = 0;
    QApplication::desktop() -> screenGeometry();
    QRect screen = QApplication::desktop() -> screenGeometry();
    resize(screen.width(), screen.height());
    height = screen.height();
    width = screen.width();
    timer = new QTimer();
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(letsRepaintNet()));
    timer -> start(REFRESH_TIME);
    generateRandomNet();
    position = myNet.vertexQueue.begin();
    pushesForInitialize = myNet.getNumberOfEdgesFromSource();
    std::cout << "VERT " << vertexNumber << "\n";
    currentStepNumber = 0;

    verticesWithHeights.resize(vertexNumber);
    for(size_t iteration = 0; iteration < vertexNumber; ++iteration) {
        verticesWithHeights[iteration] = {static_cast<double>(width) - (static_cast<double>(width / 2) / vertexNumber) * (iteration + 1), static_cast<double>(height) - EPSILA_POPOLAM};
    }
    verticesWithHeights[0] = {static_cast<double>(width) - EPSILA_POPOLAM ,static_cast<double>(height) - (static_cast<double>(height) / (3 * vertexNumber - 1)) * (vertexNumber + 1)};

    adjacent.resize(vertexNumber);
    for(int k = 0; k < adjacent.size(); ++k)
        adjacent[k].resize(vertexNumber, false);
    for(int i = 0; i < vertexNumber; ++i) {
        for(int j = 0; j < matrix[i].size(); ++j) {
            if(matrix[i][j] % 2 == 0)
                adjacent[i][myNet.edges[matrix[i][j]].to] = true;
        }
    }

//    forceMovement();

    std::ofstream out;
    out.open("/Users/semenfedotov/Desktop/SuperVisualizer/debug.txt");

    for(int i = 0; i < myNet.edges.size(); ++i)
        out << myNet.edges[i].from << " " << myNet.edges[i].to << " " << myNet.edges[i].capacity << "\n";
    ui->setupUi(this);

}

void MainWindow::shiftVertices() {
    for(auto &vertice : vertices) {
        do {
            vertice = {coordinateX(justRandom) * width, coordinateY(justRandom) * height};
        } while(std::abs(vertice.XCoordinate) < width / 2 && std::abs(vertice.YCoordinate) > height / 2);
    }
}

std::pair<Dot, Dot> MainWindow::drawArrow(Edge &toDraw) {
    double newXFromUp = vertices[toDraw.from].XCoordinate, newYFromUp = vertices[toDraw.from].YCoordinate - 3, newXToUp = vertices[toDraw.to].XCoordinate, newYToUp = vertices[toDraw.to].YCoordinate - 3;
    double newXFromDown = vertices[toDraw.from].XCoordinate, newYFromDown = vertices[toDraw.from].YCoordinate + 3, newXToDown = vertices[toDraw.to].XCoordinate, newYToDown = vertices[toDraw.to].YCoordinate + 3;
    double newVertexXUp, newVertexYUp, newVertexXDown, newVertexYDown;
    newVertexXUp -= (newXToUp - newXFromUp) * 0.99;
    newVertexYUp -= (newYToUp - newYFromUp) * 0.99;
    newVertexXDown -= (newXToDown - newXFromDown) * 0.99;
    newVertexYDown += (newYToUp - newYFromUp) * 0.99;
    return {{newVertexXUp, newVertexYUp}, {newVertexXDown, newVertexYDown}};
}


void MainWindow::keyPressEvent(QKeyEvent *event) {
    switch (event -> key()) {
    case Qt::Key_Space:
        if(timer -> isActive()) {
            timer -> stop();
            say("Yuri" , "Силовой алгоритм завершен.");
        }
        else {
            timer -> start(REFRESH_TIME);
        }
        break;

    case Qt::Key_P:
        std::cout << "IIIAG HA3AD\n";
        if(currentStepNumber == 0)
            break;
        std::cout << currentStepNumber << "STEPIC!\n";
        for(auto &height : myNet.height)
            height = 0;
        myNet.height[0] = vertexNumber;
        myNet.oldHeight = myNet.height;
        myNet.vertexQueue.clear();
        for(auto &excessing : myNet.excess)
            excessing = 0;
        myNet.excess[0] = INT64_MAX;
        thisIsTheEnd = false;
        pushesForInitialize = myNet.getNumberOfEdgesFromSource();
        for(size_t i = 0; i < vertexNumber; ++i)
            myNet.positionInSequence[i] = 0;
        for(auto &edge: myNet.edges)
            edge.flow = 0;
        for(auto &edge : myNet.edges)
            std::cout << edge.flow << " potok ";
        std::cout << "\n";
        for(size_t iteration = 0; iteration < vertexNumber; ++iteration) {
            verticesWithHeights[iteration] = {static_cast<double>(width) - (static_cast<double>(width / 2) / vertexNumber) * (iteration + 1), static_cast<double>(height) - EPSILA_POPOLAM};
        }
        verticesWithHeights[0] = {static_cast<double>(width) - EPSILA_POPOLAM ,static_cast<double>(height) - (static_cast<double>(height) / (3 * vertexNumber - 1)) * (vertexNumber + 1)};
        std::cout << myNet.listOfEdges[0].size() << "SAEZ\n";
        for(size_t iteration = 0; iteration < myNet.listOfEdges[0].size(); ++iteration) {
            if(iteration >= currentStepNumber - 1)
                break;
            std::cout << "bubu " << iteration << "\n";
            myNet.DoInitialize(myNet.listOfEdges[0].size() - iteration);
            --pushesForInitialize;
        }
        position = myNet.vertexQueue.begin();
        for(size_t iteration = 1; iteration < currentStepNumber - myNet.listOfEdges[0].size(); ++iteration) {
                    AlgoStep currentStep = myNet.DoQueueStep(position);
//                    if(currentStep.action == '-') {
//                        thisIsTheEnd = true;
//                        std::cout << "ERRORka\n";
//                        break;
//                    }
//                    std::cout << "V Algoritme " << currentStep.action << " " << currentStep.affectedNode << " " << edges[currentStep.pushingEdgeNumber].to << "\n";
                    if(currentStep.action == 'l') {
                        size_t newHeight = myNet.getVertexHeight(currentStep.affectedNode);
                        verticesWithHeights[currentStep.affectedNode].YCoordinate = static_cast<double>(height) - (static_cast<double>(height) / (2 * vertexNumber - 1)) * (newHeight + 1);
                    }
                    else if(currentStep.action == 'p') {
                        currentPushEdgeNumber = currentStep.pushingEdgeNumber;
//                        std::cout << "PUSHIM\n";
                    }
                    else
                        break;

        }
        --currentStepNumber;
        update();
        say("Cellos", "Back");
        break;

    case Qt::Key_N:
        if(pushesForInitialize > 0) {
            std::cout << "INIcializiruyu\n";
            myNet.DoInitialize(pushesForInitialize);
            ++currentStepNumber;
            --pushesForInitialize;
            if(pushesForInitialize == 0)
                position = myNet.vertexQueue.begin();
        }
        else if(pushesForInitialize <= 0) {

            AlgoStep currentStep = myNet.DoQueueStep(position);
            if(currentStep.action == '-') {
                thisIsTheEnd = true;
                std::cout << "ERRORka\n";
                update();
                say("Albert", "Good Bye");
            }
            else if(currentStep.action == 'l') {
                ++currentStepNumber;
                std::cout << "V Algoritme " << currentStep.action << " " << currentStep.affectedNode << " " << myNet.edges[currentStep.pushingEdgeNumber].to << "\n";
                size_t newHeight = myNet.getVertexHeight(currentStep.affectedNode);
                verticesWithHeights[currentStep.affectedNode].YCoordinate = static_cast<double>(height) - (static_cast<double>(height) / (2 * vertexNumber - 1)) * (newHeight + 1);
//                update();
                say("Bubbles","Lift");
            }
            else if(currentStep.action == 'p') {
                ++currentStepNumber;
                currentPushEdgeNumber = currentStep.pushingEdgeNumber;
//                std::cout << "PUSHIM\n";
//                update();
                say("Diego", "Push");
            }
            else
                break;
        }
        update();
        break;
    default:
        break;
    }

}


void MainWindow::letsRepaintNet() {
//    shiftVertices();
    forceMovement();
    std::cout << "dasd";
    update();
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
       QPainter painter(this);
       painter.setRenderHint(QPainter::Antialiasing);
       QImage* background = new QImage;
       background ->load("/Users/semenfedotov/Desktop/SuperVisualizer/background.jpg");
       painter.drawImage(0, 0, *background);

       if(thisIsTheEnd) {
           makeSomeMagic(&painter);
       }
//       painter.translate(width / 2, height / 2);
       drawEdge(&painter);
       drawVertex(&painter);
       if(!(timer -> isActive())) {
           drawEdge(&painter, true);
           drawVertex(&painter, true);
       }

}

void MainWindow::makeSomeMagic(QPainter *painter) {
    QImage* endImage = new QImage;
    endImage -> load("/Users/semenfedotov/Desktop/SuperVisualizer/end.jpg");
    painter -> drawImage(0, 0, *endImage);
    endImage -> load("/Users/semenfedotov/Desktop/SuperVisualizer/Supa.png");
    painter -> drawImage(width / 4, height / 4, *endImage);
    painter -> drawText(width / 2 - EPSILA_POPOLAM, height / 2 + EPSILA_POPOLAM, QString::fromStdString("Max flow is = " + std::to_string(myNet.excess[vertexNumber - 1])));

}

void MainWindow::drawVertex(QPainter *painter, const bool &withHeights) {
    size_t thisNodeNumber = 0;

    std::vector<Dot> dotsWithCoordinates;
    if(withHeights)
        dotsWithCoordinates = verticesWithHeights;
    else
        dotsWithCoordinates = vertices;

    for(auto &vertex : dotsWithCoordinates) {
        if(thisNodeNumber == 0) {
            painter -> setPen(QPen(Qt::blue, 6));
            painter -> setBrush(QBrush(Qt::gray, Qt::SolidPattern));
            painter -> drawEllipse(QPoint(vertex.XCoordinate, vertex.YCoordinate), 13, 13);
            painter -> setPen(QPen(Qt::blue));
            QString datString = QString::fromStdString("START" + std::to_string(thisNodeNumber) +  "_" + std::to_string(myNet.height[thisNodeNumber]));
            painter -> drawText(vertex.XCoordinate - 4, vertex.YCoordinate - 20, datString);

            painter -> setPen(QPen());
            ++thisNodeNumber;
            continue;
        }
        if(thisNodeNumber == vertexNumber - 1) {
            painter -> setPen(QPen(Qt::red, 6));
            painter -> setBrush(QBrush(Qt::blue, Qt::SolidPattern));
            painter -> drawEllipse(QPoint(vertex.XCoordinate, vertex.YCoordinate), 13, 13);
            painter -> setPen(QPen(Qt::blue));
            QString datString = QString::fromStdString("FINISH" + std::to_string(thisNodeNumber));
            painter -> drawText(vertex.XCoordinate - 4, vertex.YCoordinate - 20, datString);
            QString excessString = QString::fromStdString(std::to_string(myNet.excess[thisNodeNumber]));
            painter -> drawText(vertex.XCoordinate - 4, vertex.YCoordinate + 20, excessString);
            painter -> setPen(QPen());
            ++thisNodeNumber;
            continue;
        }
        painter -> setPen(QPen(Qt::cyan, 6));
        painter -> setBrush(QBrush(Qt::black, Qt::SolidPattern));
        painter -> drawEllipse(QPoint(vertex.XCoordinate, vertex.YCoordinate), 12, 12);

        painter -> setBrush(QBrush());
        painter -> setPen(QPen());

        painter -> setPen(QPen(Qt::yellow));
        QString datString = QString::fromStdString(std::to_string(thisNodeNumber));
        painter -> drawText(vertex.XCoordinate - 4, vertex.YCoordinate - 10, datString);
        QString excessString = QString::fromStdString(std::to_string(myNet.excess[thisNodeNumber]) +  "_" + std::to_string(myNet.height[thisNodeNumber]));
        painter -> drawText(vertex.XCoordinate - 4, vertex.YCoordinate + 20, excessString);
        painter -> setPen(QPen());
        ++thisNodeNumber;
    }
}

Dot MainWindow::calculateMiddle(const Edge &toCalculate, const bool &withHeight) {
    if(withHeight)
        return {(verticesWithHeights[toCalculate.from].XCoordinate + verticesWithHeights[toCalculate.to].XCoordinate) / 2, (verticesWithHeights[toCalculate.from].YCoordinate + verticesWithHeights[toCalculate.to].YCoordinate) / 2};
    return {(vertices[toCalculate.from].XCoordinate + vertices[toCalculate.to].XCoordinate) / 2, (vertices[toCalculate.from].YCoordinate + vertices[toCalculate.to].YCoordinate) / 2};
}


void MainWindow::drawEdge(QPainter *painter, const bool &withHeights) {


    for(size_t edgeNumber = 0; edgeNumber < myNet.edges.size(); edgeNumber += 2) {
        painter -> setPen(QPen(Qt::red, 5, Qt::SolidLine, Qt::RoundCap));
        if(withHeights)
            painter -> drawLine(verticesWithHeights[myNet.edges[edgeNumber].from].XCoordinate, verticesWithHeights[myNet.edges[edgeNumber].from].YCoordinate, verticesWithHeights[myNet.edges[edgeNumber].to].XCoordinate, verticesWithHeights[myNet.edges[edgeNumber].to].YCoordinate);
        else
            painter -> drawLine(vertices[myNet.edges[edgeNumber].from].XCoordinate, vertices[myNet.edges[edgeNumber].from].YCoordinate, vertices[myNet.edges[edgeNumber].to].XCoordinate, vertices[myNet.edges[edgeNumber].to].YCoordinate);
        painter -> setPen(QPen(Qt::green, 8, Qt::SolidLine, Qt::RoundCap));
        QString datString = QString::fromStdString(std::to_string(myNet.edges[edgeNumber].capacity) + "|" + std::to_string(myNet.edges[edgeNumber].flow) + "{}" + std::to_string(myNet.edges[edgeNumber + 1].capacity) + "|" + std::to_string(myNet.edges[edgeNumber + 1].flow));
        Dot edgeCenter;
        if(withHeights) {
            edgeCenter = calculateMiddle(myNet.edges[edgeNumber], true);
            painter -> drawLine(verticesWithHeights[myNet.edges[edgeNumber].to].XCoordinate, verticesWithHeights[myNet.edges[edgeNumber].to].YCoordinate, (verticesWithHeights[myNet.edges[edgeNumber].to].XCoordinate + edgeCenter.XCoordinate) / 2, (verticesWithHeights[myNet.edges[edgeNumber].to].YCoordinate + edgeCenter.YCoordinate) / 2);
        }
        else {
            edgeCenter = calculateMiddle(myNet.edges[edgeNumber]);
            painter -> drawLine(vertices[myNet.edges[edgeNumber].to].XCoordinate, vertices[myNet.edges[edgeNumber].to].YCoordinate, (vertices[myNet.edges[edgeNumber].to].XCoordinate + edgeCenter.XCoordinate) / 2, (vertices[myNet.edges[edgeNumber].to].YCoordinate + edgeCenter.YCoordinate) / 2);
        }
        painter -> setPen(QPen(Qt::magenta));
        painter -> drawText(edgeCenter.XCoordinate - 10, edgeCenter.YCoordinate - 20, datString);
        painter -> setPen(QPen());



    }
}

void MainWindow::generateRandomNet() {
    //Making Dots with Normal distribution
    vertexNumber = vertexNumberRandomizer(justRandom);
//    std::cout << "dasd";
    matrix.resize(vertexNumber);
    size_t edgesCounter = 0;
    vertices.resize(vertexNumber);
    shiftVertices();
//    std::cout << "VERTEX__" << vertexNumber << "\n";
    for(size_t from = 0; from < vertexNumber; ++from) {
        for(size_t to = 1; to < vertexNumber; ++to) {
            if(from == to)
                continue;
            if(rand() % 3 == 1) {
                long long capacity = amountRandomizer(justRandom);
                edges.push_back({from, to, capacity, 0});
                edges.push_back({to, from, 0, 0});
                matrix[from].push_back(edgesCounter);
                matrix[to].push_back(edgesCounter + 1);
                edgesCounter += 2;
            }
        }
    }

//    shiftVertices();


//    std::cout << vertices.size() << "vertsize " << myNet.edges.size() << "\n";
    myNet = Net(edges, vertexNumber, matrix, true);

    int nomer = 0;
    for(auto &edge : myNet.edges) {
        std::cout << "[" << nomer << "] " << edge.from << " " << edge.to << " " << edge.capacity << "EDGE\n";
        ++nomer;
    }

    update();

}

void MainWindow::forceMovement() {



    double hookLawForce = 50, springLength = 1, coulombLawForce = 2234509, c4 = 0.035;
    int counter = 0;
    double coulombLaw;
    for(int i = 0; i < 2; ++i) {
        for(auto &vertice : vertices) {
            QVector2D ourVector = QVector2D(vertice.XCoordinate, vertice.YCoordinate);
            QPoint from(vertice.XCoordinate, vertice.YCoordinate);
            QVector2D force(0, 0);
            int toCounter = 0;
            for(auto &toVertice : vertices) {
                QPoint to(toVertice.XCoordinate, toVertice.YCoordinate);
                QVector2D direction = QVector2D(from - to);
                direction /= direction.length();
                if(QVector2D(from - to).length() != 0 /*&& !adjacent[counter][toCounter]*/ && counter != toCounter) {
                    coulombLaw = coulombLawForce / (QVector2D(from - to).length() * QVector2D(from - to).length());
                    force += direction * (coulombLaw);
//                    std::cout << "NE SILA 4EMY RAVNA!!!!   " << coulombLaw << "\nzuzuzu\n";
                }
                if(adjacent[counter][toCounter]) {
//                    std::cout << "KAUNTER  " << counter << "\naszs\n";
                    direction = QVector2D(to - from);
                    double lengthOfSpring = direction.length();
                    direction /= direction.length();

                    if(lengthOfSpring / springLength != 0)
                        force += direction * std::abs((hookLawForce * log(lengthOfSpring / springLength)));
                }
                force += QVector2D(1,0) * 1.2 * (coulombLawForce / ((vertice.XCoordinate) * (vertice.XCoordinate)));
                force += QVector2D(-1,0) * 1.4 * (coulombLawForce / ((width / 2 - vertice.XCoordinate) * (width / 2 - vertice.XCoordinate)));
                force += QVector2D(0,-1) * 1.6 * (coulombLawForce / ((height - vertice.YCoordinate) * (height - vertice.YCoordinate)));
                force += QVector2D(0,1) * 2 * (coulombLawForce / ((vertice.YCoordinate) * (vertice.YCoordinate)));

                ++toCounter;
            }
            
//
            QPoint newVertice(vertice.XCoordinate, vertice.YCoordinate);
            newVertice += (c4 * force).toPoint();
            vertice.XCoordinate = newVertice.x();
            vertice.YCoordinate = newVertice.y();
            if(vertice.XCoordinate > width / 2 + EPSILA)
                vertice.XCoordinate = width / 2 - EPSILA_POPOLAM;
            if(vertice.XCoordinate < 0)
                vertice.XCoordinate = EPSILA_POPOLAM;
            if(vertice.YCoordinate < 0)
                vertice.YCoordinate = EPSILA_POPOLAM;
            if(vertice.YCoordinate > height)
                vertice.YCoordinate = height - EPSILA_POPOLAM;
//            std::cout << vertice.XCoordinate << "SHEEEEEEEET" << vertice.YCoordinate << "\n";



            ++counter;
//            std::cout << "SILA " << width << " " << height  << "\n";
        }
        counter = 0;


    }

}

double MainWindow::getSquaredLengthBetweenVertecies(const Dot &first, const Dot &second) {
    return (first.XCoordinate - second.XCoordinate) * (first.XCoordinate - second.XCoordinate)
         + (first.YCoordinate - second.YCoordinate) * (first.YCoordinate - second.YCoordinate);
}



double MainWindow::getLengthOfEdge(const Edge &edge) {
    return std::sqrt((vertices[edge.from].XCoordinate - vertices[edge.to].XCoordinate) * (vertices[edge.from].XCoordinate - vertices[edge.to].XCoordinate)
         + (vertices[edge.from].YCoordinate - vertices[edge.to].YCoordinate) * (vertices[edge.from].YCoordinate - vertices[edge.to].YCoordinate));
}



