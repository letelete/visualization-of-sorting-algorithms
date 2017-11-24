#include "includes/visualization.h"
#include "ui_widget.h"

#include <random>
#include <algorithm>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    //setting up scene
    scene = new QGraphicsScene(this);
    ui->cellsToSortBackground->setScene(scene);
    ui->cellsToSortBackground->verticalScrollBar()->blockSignals(true);
    ui->cellsToSortBackground->horizontalScrollBar()->blockSignals(true);

    //configuring variables
    appStates = -1;

    sceneHeight = ui->cellsToSortBackground->size().height();
    sceneWidth = ui->cellsToSortBackground->size().width();
    comparisions = 0;

    //to improve UX
    ui->amountChanger->setDisabled(true);
    ui->delayChanger->setDisabled(true);
    ui->sortButton->setDisabled(true);

    //creating 100 columns with sorting delay == 1ms , setting them up
    columnsSetUp(1, 100);

    //customize the ListWidget & set a QMap for sorting algorithms
    for(int i=0; i<ui->algorithmSelection->count(); i++)
    {
      QListWidgetItem *item = ui->algorithmSelection->item(i);
      item->setSizeHint(QSize(item->sizeHint().width(), 50));
      AlgorithmList.insert(i, item->text());
    }
}

void Widget::columnsSetUp(int ms, int n)
{
    //configuring variables
    ui->LabelComparisions_var->setNum(0);
    ui->LabelArrayAccesses_var->setNum(0);
    sortDelay = ms;
    amountOfColumns = static_cast<unsigned>(n);
    columnsWidth = static_cast<float>(sceneWidth) / static_cast<float>(amountOfColumns);

    //setting up columns to sort
    columns.resize(amountOfColumns);

    //setting up columns height
    float incrementBy = static_cast<float>(sceneHeight) / static_cast<float>(amountOfColumns);
    for(auto i=incrementBy; i<=sceneHeight; i+=incrementBy)
        columnsHeight.push_back(i);

    //randomize an array
    std::random_device m_random;
    std::mt19937 e_random(m_random());
    std::shuffle(columnsHeight.begin(), columnsHeight.end(), e_random);

    //aplying columns to scene
    auto j = 0;
    double k = 0.0;
    for(auto &p : columns)
    {
        p = new QGraphicsRectItem;
        p->setRect(k, (sceneHeight - columnsHeight[j]), columnsWidth , columnsHeight[j]);
        p->setBrush(QBrush(QColor(255, 0, 68, 255)));

        if(amountOfColumns <= 200)
            p->setPen(QPen(Qt::black, 2));
        else if(amountOfColumns > 200 && amountOfColumns <= 300)
            p->setPen(QPen(Qt::black, 1));
        else
            p->setPen(Qt::NoPen);

        scene->addItem(p);

        j++;
        k += static_cast<double>(columnsWidth);
    }
}

void Widget::resetColumns(int ms, int n)
{
    for(auto &p : columns)
        scene->removeItem(p);

    columnsHeight.clear();

    columnsSetUp(ms, n);
    threadUpdate(sortDelay, algorithmKey);
}

void Widget::threadUpdate(int ms, int key)
{
    mThread = new Thread(ms, key, static_cast<int>(amountOfColumns), columnsHeight, this);

    connect(mThread, SIGNAL(comparision(int, int)), this, SLOT(on_comparision(int, int)));
    connect(mThread, SIGNAL(sortDone(int)), this, SLOT(sortDone(int)));
    connect(mThread, SIGNAL(arrayAccess(int)), ui->LabelArrayAccesses_var, SLOT(setNum(int)));
    connect(mThread, SIGNAL(changeButtonStatus(int)), this, SLOT(sortButtonStatus(int)));
}

void Widget::on_algorithmSelection_itemClicked(QListWidgetItem *item)
{
    if(appStates == -1)
    {
        ui->sortButton->setEnabled(true);
        ui->amountChanger->setEnabled(true);
        ui->delayChanger->setEnabled(true);

        appStates = 0;
        sortButtonStatus(appStates);
    }

    if(appStates == 0)
    {
        algorithmKey = AlgorithmList.key(item->text());
        ui->LabelSortingWith_var->setText(item->text());
    }
}

void Widget::on_amountChanger_valueChanged(int n)
{
    if(appStates == 0)
        resetColumns(sortDelay, n);
}

void Widget::on_delayChanger_valueChanged(int ms)
{
    if(appStates == 0)
        resetColumns(ms, static_cast<int>(amountOfColumns));
}


void Widget::on_sortButton_clicked()
{   

    switch(appStates)
    {
        case 0:
            sortButtonStatus(1);
            comparisions = 0;
            threadUpdate(sortDelay, algorithmKey);
            mThread->start();
            break;

        case 1:
            mThread->terminate();
            sortButtonStatus(2);
            break;

        case 2:
            for(auto &p : columns)
                scene->removeItem(p);

            columnsHeight.clear();
            columnsSetUp(sortDelay, static_cast<int>(amountOfColumns));
            sortButtonStatus(0);
            break;

        default:
            break;

    }
}

void Widget::sortButtonStatus(int state)
{
    QString buttonText;
    QString style;

    switch(state)
    {
    case 0:
        buttonText = "sort";
        ui->amountChanger->setEnabled(true);
        ui->delayChanger->setEnabled(true);
        ui->algorithmSelection->setEnabled(true);
        style = "background-color: rgba(255, 0, 68, 255); color: #fff";
        break;

    case 1:
        buttonText = "cancel";
        ui->amountChanger->setDisabled(true);
        ui->delayChanger->setDisabled(true);
        ui->algorithmSelection->setDisabled(true);
        style = "background-color: #000; color: #fff";
        break;

    case 2:
        buttonText = "new sort";
        ui->sortButton->setEnabled(true);
        style = "background-color: rgb(85, 0, 255); color: #fff";
        break;
    }

    appStates = state;
    ui->sortButton->setText(buttonText);
    ui->sortButton->setStyleSheet(style);
}

void Widget::on_comparision(int n, int k)
{
    columns[n]->setX(columns[n]->pos().x() + columnsWidth);
    columns[k]->setX(columns[k]->pos().x() - columnsWidth);

    std::swap(columns[n], columns[k]);

    comparisions++;
    ui->LabelComparisions_var->setNum(comparisions);
}

void Widget::sortDone(int n)
{
    columns[n]->setBrush(QBrush(QColor(0,200,0)));
}

Widget::~Widget()
{
    mThread->terminate();
    delete ui;
}
