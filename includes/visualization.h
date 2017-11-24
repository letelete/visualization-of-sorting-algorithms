#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtWidgets>

#include "algorithms.h"

namespace Ui {
class Widget;
}

class Thread;

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_sortButton_clicked();
    void on_algorithmSelection_itemClicked(QListWidgetItem*);
    void on_amountChanger_valueChanged(int);
    void on_delayChanger_valueChanged(int);

    void on_comparision(int, int);
    void sortButtonStatus(int);
    void sortDone(int);
    void resetColumns(int, int);

private:
    Ui::Widget *ui;
    Thread *mThread;
    QGraphicsScene *scene;

    //setting up columns, shuffeling them
    void columnsSetUp(int, int);
    void threadUpdate(int, int);

    std::vector<QGraphicsRectItem*> columns;
    std::vector<float> columnsHeight;

    QMap<int, QString> AlgorithmList;

    //delay of sorting in ms
    int sortDelay;

    unsigned amountOfColumns;
    float columnsWidth;
    int sceneHeight;
    int sceneWidth;
    int comparisions;

    //key which is use to establish the algorithm to sort columns
    int algorithmKey;

    // (-1) - App started, 0 - App ready to sort, (1) - App sorting, (2) - App sorted
    int appStates;
};

#endif // WIDGET_H
