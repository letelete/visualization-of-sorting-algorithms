#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <QtWidgets>

class Thread : public QThread
{
    Q_OBJECT
public:
    Thread(int, int, int, std::vector<double>, QObject*);

signals:
    void comparision(int, int);
    void sortDone(int);
    void arrayAccess(int);
    void changeButtonStatus(int);

protected:
    void run() override;

private:
    std::vector<double> columnsHeight;
    int sortDoneDelay;
    int sortDelay;
    int amount;
    int sortWith;
    int arrayAccessVariable;

    void swap(int, int);
    void isAccessToArray();
    void Sorted();

    //sorting algorithms declaration
    void BubbleSort();
    void RecursiveBubbleSort(int);
    void CocktailSort();
    void GnomeSort();

    int QuickSortPartition(int, int);
    void QuickSort(int, int);

    void Heapify(int n, int i);
    void HeapSort();
};

#endif // THREAD_H
