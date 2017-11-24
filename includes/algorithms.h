#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <QtWidgets>

class Thread : public QThread
{
    Q_OBJECT
public:
    Thread(int, int, int, std::vector<float>, QObject*);

signals:
    void comparision(int, int);
    void sortDone(int);
    void arrayAccess(int);
    void changeButtonStatus(int);

protected:
    void run() override;

private:
    std::vector<float> columnsHeight;
    int sortDelay;
    int amount;
    int sortWith;
    int arrayAccessVariable;

    void Sorted();

    //sorting algorithms declaration
    void BubbleSort();
    void CocktailSort();
};

#endif // THREAD_H
