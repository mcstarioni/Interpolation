#include "interpolation.h"
#include <QDebug>
#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QValidator>
#include <math.h>
#include <QPainter>
#include <QPixmap>
Interpolation::Interpolation(QWidget *parent) : QWidget(parent)
{
    this->n = 0;
    this->lastY = 7;
    this->valuesY = new double[50];
    this->valuesX = new double[50];
    this->vector = new QVector<QVector<double>>();
    this->setTable();
    this->pictHeight = 350;
    this->pictWidth = 400;
    this->step = 0.5;


    QHBoxLayout *all = new QHBoxLayout();
    QVBoxLayout *base = new QVBoxLayout();
    QHBoxLayout *bottom1 = new QHBoxLayout();
    QHBoxLayout *bottom2 = new QHBoxLayout();

    QLabel *enterX = new QLabel("Введите значение X: ");
    QLineEdit *countValue = new QLineEdit();
    countValue->setMinimumWidth(30);
    QDoubleValidator *valid = new QDoubleValidator();
    valid->setLocale(QLocale(QLocale::English));
    countValue->setValidator(valid);
    connect(countValue,SIGNAL(editingFinished()),SLOT(countValueChanged()));
    countValue->setMaxLength(10);
    QPushButton *count = new QPushButton("Расчет");
    connect(count,SIGNAL(clicked()),SLOT(count()));
    bottom1->addWidget(enterX);
    bottom1->addWidget(countValue);
    bottom1->addWidget(count);
    bottom1->addStretch();
    QLabel *res =  new QLabel("Результат: ");
    result = new QLabel("");
    QPushButton *draw = new QPushButton("Нарисовать график");
    connect(draw,SIGNAL(clicked()),SLOT(drawClicked()));
    bottom2->addWidget(res);
    bottom2->addWidget(result);
    bottom2->addWidget(draw);

    base->addLayout(this->table);
    QPushButton *add = new QPushButton("Добавить точку");
    connect(add,SIGNAL(clicked()),SLOT(addPoint()));
    base->addWidget(add);
    base->addStretch();
    base->addLayout(bottom1);
    base->addLayout(bottom2);

    graphTable = new QLabel();
    graphTable->hide();
    all->addLayout(base);
    all->addWidget(graphTable);
    this->setLayout(all);
}
void Interpolation::setTable()
{
    this->table = new QGridLayout();
    //this->table->setAlignment();
    QLabel *varN = new QLabel("N");
    varN->setAlignment(Qt::AlignHCenter);
    QLabel *varX = new QLabel("X");
    varX->setAlignment(Qt::AlignHCenter);
    QLabel *varY = new QLabel("Y");
    varY->setAlignment(Qt::AlignHCenter);
    table->addWidget(varN,0,0);
    table->addWidget(varX,0,1);
    table->addWidget(varY,0,2);
    addPoint();
    addPoint();
    table->columnStretch(2);
}
void Interpolation::addPoint()
{
    qDebug()<<"added 1 n = "<<this->n;
    this->n++;
    QLabel *i = new QLabel(QString::number(n));
    QLineEdit *x = new QLineEdit(QString::number(n));
    valuesX[n-1] = n;
    x->setMaxLength(10);
    QDoubleValidator *valid1 = new QDoubleValidator();
    valid1->setLocale(QLocale(QLocale::English));
    x->setValidator(valid1);
    connect(x,SIGNAL(editingFinished()),SLOT(onChange()));

    QLineEdit *y = new QLineEdit(QString::number(this->lastY));
    valuesY[n-1] = lastY;
    y->setMaxLength(10);
    QDoubleValidator *valid2 = new QDoubleValidator();
    valid2->setLocale(QLocale(QLocale::English));
    y->setValidator(valid2);
    connect(y,SIGNAL(editingFinished()),SLOT(onChange()));
    lastY +=4;

    QPushButton *removeButton = new QPushButton("X");
    connect(removeButton,SIGNAL(clicked()),SLOT(removePoint()));
    removeButton->setFlat(true);

    int temp = table->rowCount();
    table->addWidget(i,temp,0);
    table->addWidget(x,temp,1);
    table->addWidget(y,temp,2);
    table->addWidget(removeButton,temp,3);
    table->setColumnStretch(5,2);
    //qDebug()<<"added 2 n = "<<this->n;
}
void Interpolation::removePoint()
{
    QWidget* widget = qobject_cast<QWidget*>(sender());
    int index = table->indexOf(widget);
    int row = index/4;
    for(int i = row; i < n+1; i++)
    {
        valuesX[i - 1] = valuesX[i];
        valuesY[i - 1] = valuesY[i];
        //qDebug()<<i<<" "<<i*4 - 1;
        QLabel* lable = (QLabel*)table->itemAt(i*4 - 1)->widget();
        lable->setText(QString::number(i-1));
    }
    table->removeWidget(widget);
    delete widget;
    widget = table->itemAt(index - 1)->widget();
    table->removeWidget(widget);
    delete widget;
    widget = table->itemAt(index - 2)->widget();
    table->removeWidget(widget);
    delete widget;
    widget = table->itemAt(index - 3)->widget();
    table->removeWidget(widget);
    delete widget;
    //qDebug()<<"remove  n = "<<this->n<<", row to start = "<<row;
    n--;
}

void Interpolation::count()
{
    for(int i = 0; i < n; i++)
    {
        //qDebug()<<"x = "<<valuesX[i]<<",  y = "<<valuesY[i];
    }
    result->setText(QString::number(calculate(value)));
}
void Interpolation::onChange()
{
    int row = 0,col = 0,rs = 0,cs = 0;
    QWidget *w = qobject_cast<QWidget*>(sender());
    int index = table->indexOf(w);
    table->getItemPosition(index,&row,&col,&rs,&cs);
    if(col < 3 && col > 0)
    {
        QLineEdit *line = (QLineEdit*)w;
        double value = line->text().toDouble();
        if(col == 1)
            valuesX[row-1] = value;
        else
            valuesY[row-1] = value;
    }
}
void Interpolation::countValueChanged()
{
    value = qobject_cast<QLineEdit*>(sender())->text().toDouble();
}
double Interpolation::determinant( double x11, double x12, double x21, double x22)
{
   return (x11*x22 - x21*x12);
}
double Interpolation::calculate(double X)
{
    QVector<double> valY;
    for(int k = 0; k < n; k++)
    {
        valY.push_back(valuesY[k]);
    }
    vector->push_back(valY);
    int order = 1;
    for(int j = n-1; j > 0; j--)
    {
        QVector<double> polynom;
        for(int i = 0; i < j; i++)
        {
            double det = determinant(
                        vector->at(order - 1).at(i),
                        valuesX[i] - X,
                        vector->at(order - 1).at(i+1),
                        valuesX[i+order]-X);
            double res = det/(valuesX[i+order] - valuesX[i]);
            polynom.push_back(res);
        }
        order++;
        vector->push_back(polynom);
    }
    double d = vector->at(order-1).at(0);
    vector->clear();
    return d;
}
void Interpolation::drawGraph()
{

    QPixmap graph(pictWidth,pictHeight);
    QPainter paint;
    paint.begin(&graph);
    paint.eraseRect(0,0,pictWidth,pictHeight);
    QVector<QPoint> points;
    points.push_back(QPoint(0,pictHeight/2));
    points.push_back(QPoint(pictWidth,pictHeight/2));
    //paint
    paint.setPen(QPen(Qt::gray,3));
    qDebug()<<"X: "<<leftX<<" "<<rightX;
    qDebug()<<"Y: "<<leftY<<" "<<rightY;
    qDebug()<<"range: "<<range;
    int pointStep = pictWidth/range;
    int text = leftX;
    int axisY;
    for(int i = 0; i < pictWidth; i+=pointStep)
    {
        if(text == 0)
        {
            axisY = i;
            paint.setPen(QPen(Qt::gray,1));
            points.push_back(QPoint(i,0));
            points.push_back(QPoint(i,pictHeight));
            paint.drawLines(points);
            points.clear();
            paint.setPen(QPen(Qt::gray,3));
        }
        paint.drawPoint(QPoint(i,pictHeight/2));
        paint.drawText(QPoint(i,pictHeight/2 + 13),QString::number(text));
        text++;
    }
    for(int i = 0; i < pictHeight/2; i+=pointStep)
    {
        paint.drawPoint(QPoint(axisY,pictHeight/2-i));
        paint.drawPoint(QPoint(axisY,pictHeight/2+i));
    }
    qDebug()<<"1";
    paint.setPen(QPen(Qt::red,1));
    double onePix = ceil(pictWidth/range);
    qDebug()<<onePix;
    double first = calculate(leftX);
    QPoint p1(0,pictHeight/2 - first*onePix);
    qDebug()<<"2";
    for(double i = leftX + step; i < rightX; i+=step)
    {
        qDebug()<<i;
        QPoint p2(axisY + i*onePix,pictHeight/2 - calculate(i)*onePix);
        paint.drawLine(p1,p2);
        points.push_back(p1);
        points.push_back(p2);
        p1 = p2;
    }
    paint.drawLines(points);
    paint.setPen(QPen(Qt::black,4));
    for(int i = 0; i < n; i++)
    {
        QPoint p2(axisY + valuesX[i]*onePix,pictHeight/2 - valuesY[i]*onePix);
        paint.drawPoint(p2);
    }
    this->graphTable->setPixmap(graph);
    return;

}
void Interpolation::getData()
{
    leftX = valuesX[0] - 5;
    rightX = valuesX[n-1] + 5;
    //leftY = valuesY[0] - 10;
    //rightY = valuesY[0] - 10;
    step = 0.1;
    range = abs((leftX) - (rightX));
}
void Interpolation::recountPixels()
{
    onePixelX = 540.0/(rightX-leftX);
    onePixelY = 370.0/(rightY-leftY);
    Ox = abs(leftX); Oy = rightY;
}
void Interpolation::drawClicked()
{
    if(n!= 0)
    {
        graphTable->show();
        getData();
        recountPixels();
        drawGraph();
    }
}
