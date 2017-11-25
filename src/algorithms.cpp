#include "includes/algorithms.h"

Thread::Thread(int ms, int alg, int n, std::vector<float> col, QObject *parent = nullptr) : QThread(parent)
{
    sortWith = alg;
    amount = n;
    columnsHeight = col;
    arrayAccessVariable = 0;

    sortDoneDelay = (amount >= 300 ? 2 : 5);
    sortDelay = ms;
}

void Thread::run()
{
    switch(sortWith)
    {
        case 0: BubbleSort(); break;
        case 1: CocktailSort(); break;
        case 2: GnomeSort(); break;
        case 3: QuickSort(0, amount -1); break;
    }
    Sorted();
}

void Thread::Sorted()
{
    for(auto i=0; i<amount; i++)
    {
        emit sortDone(i);
        msleep(sortDoneDelay);
    }
    emit changeButtonStatus(2);
}

void Thread::swap(int n, int k)
{
    std::swap(columnsHeight[n], columnsHeight[k]);
    emit comparision(n, k);
}

void Thread::isAccessToArray()
{
    arrayAccessVariable++;
    emit arrayAccess(arrayAccessVariable);
}

//--------- BUBBLE SORT -----------

void Thread::BubbleSort()
{
    for(auto i = 0; i < amount; i++)
            for(auto j = 0; j< amount-1; j++)
            {
                if(columnsHeight[j] > columnsHeight[j+1])
                    swap(j, j+1);

                isAccessToArray();
                msleep(sortDelay);
            }
}

//--------- COCKTAIL SORT ---------

void Thread::CocktailSort()
{
    bool swapped = true;
    auto start = 0;
    auto end = amount-1;

    while(swapped)
    {
        swapped = false;
        for (auto i = start; i < end; ++i)
        {
            if(columnsHeight[i] > columnsHeight[i + 1])
            {
                swap(i, i+1);
                swapped = true;
            }
            isAccessToArray();
            msleep(sortDelay);
        }

        if (!swapped)
            break;

        swapped = false;
        --end;

        for (auto i = end-1; i >= start; --i)
        {
            if (columnsHeight[i] > columnsHeight[i + 1])
            {
                swap(i, i+1);
                swapped = true;
            }
            isAccessToArray();
            msleep(sortDelay);
        }
        ++start;
    }
}

//--------- GNOME SORT ------------

void Thread::GnomeSort()
{
    int index = 0;

    while (index < amount)
    {
        if (index == 0)
            index++;
        if (columnsHeight[index] >= columnsHeight[index-1])
            index++;
        else
        {
            swap(index, index-1);
            index--;
        }
        isAccessToArray();
        msleep(sortDelay);
    }
}

//--------- QUICK SORT ------------

int Thread::QuickSortPartition(int arrayBegin, int arrayEnd)
{
    auto pivot = columnsHeight[arrayEnd];
    auto i = (arrayBegin - 1);

    for (auto j = arrayBegin; j <= arrayEnd - 1; j++)
    {
        if (columnsHeight[j] <= pivot)
        {
            i++;
            swap(i, j);
        }

        isAccessToArray();
        msleep(sortDelay);
    }

    swap(i+1, arrayEnd);

    msleep(sortDelay);
    return(i + 1);
}

void Thread::QuickSort(int arrayBegin, int arrayEnd)
{
    if (arrayBegin < arrayEnd)
    {
        auto pi = QuickSortPartition(arrayBegin, arrayEnd);

        QuickSort(arrayBegin, pi - 1);
        QuickSort(pi + 1, arrayEnd);
    }
}
