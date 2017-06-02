#ifndef INTERPOLATION_H
#define INTERPOLATION_H
#include <QWidget>
#include <QVector>
#include <QValidator>
#include <QGraphicsScene>
class QLabel;
class QGridLayout;
class Interpolation : public QWidget
{
    Q_OBJECT

private:
    QVector<QVector<double>> *vector;
    QLabel *result;
    QGridLayout *table;
    QLabel *graphTable;
    double *valuesY;
    double *valuesX;
    void setTable();
    void getData();
    double calculate(double);
    void recountPixels();
    void drawGraph();
    double determinant(  double x11, double x12, double x21, double x22);
    int n;
    double lastY;
    QValidator *validator;
    QGraphicsScene *scene;
    double value;
    double leftX,rightX;
    double leftY,rightY;
    int pictWidth,pictHeight;
    double step;
    double onePixelX,onePixelY;
    double Ox,Oy;
    double range;

public:
    explicit Interpolation(QWidget *parent = 0);
public slots:
    void count();
    void addPoint();
    void removePoint();
    void onChange();
    void countValueChanged();
    void drawClicked();
};
#endif // INTERPOLATION_H
