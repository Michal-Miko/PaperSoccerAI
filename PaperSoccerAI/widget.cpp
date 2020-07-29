#include "widget.h"
#include "./ui_widget.h"
#include <random>

Widget::Widget(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::Widget)
{
    ui->setupUi(this);


    auto line_pen = QPen(Qt::white);
    line_pen.setWidth(3);

    scene = new Scene(this);
    scene->setSceneRect(0,0,450,650);
    scene->addLine(QLine(0,325,450,325),line_pen);
    scene->addLine(QLine(225,0,225,650),line_pen);

    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0,1);


    auto fcount = 13*9;
    auto separation = 50;
    QGraphicsPixmapItem **fields = new QGraphicsPixmapItem*[fcount];
    for(int i=0;i<fcount;i++){
        if(distribution(generator))
                fields[i] = new QGraphicsPixmapItem(QPixmap(":/img/res/taken_field.png"));
        else
            fields[i] = new QGraphicsPixmapItem(QPixmap(":/img/res/empty_field.png"));
        fields[i]->setPos(QPointF(25-8+ i%9 * separation, 25-8+ i/9 * separation));
        fields[i]->setScale(0.5);
        scene->addItem(fields[i]);
    }


    // Board <- object, drawable item group


    ui->graphicsView->setScene(scene);
}

Widget::~Widget()
{
    delete ui;
}

