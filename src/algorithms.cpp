#include "includes/algorithms.h"

Thread::Thread(int ms, int alg, int n, std::vector<double> col, QObject *parent = nullptr) : QThread(parent)
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
        case 1: RecursiveBubbleSort(amount); break;
        case 2: CocktailSort(); break;
        case 3: GnomeSort(); break;
        case 4: QuickSort(0, amount -1); break;
        case 5: HeapSort(); break;
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
//            for(auto j = 0; j< amount-1; j++)
        for(auto j = 0; j< amount-i-1; j++) //FIXME-LYS algorithmic error,leads to more waste
            {
                if(columnsHeight[j] > columnsHeight[j+1])
                    swap(j, j+1);

                isAccessToArray();
                msleep(sortDelay);
            }
}

//--------- RECURSIVE BUBBLE SORT -

void Thread::RecursiveBubbleSort(int n)
{
    if (n == 1)
        return;

    for (int i=0; i<n-1; i++)
    {
        if (columnsHeight[i] > columnsHeight[i+1])
            swap(i, i+1);

        isAccessToArray();
        msleep(sortDelay);
    }
    RecursiveBubbleSort(n-1);
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
        for (auto i = start; i < end; i++)
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
        end--;

        for (auto i = end-1; i >= start; i--)
        {
            if (columnsHeight[i] > columnsHeight[i + 1])
            {
                swap(i, i+1);
                swapped = true;
            }
            isAccessToArray();
            msleep(sortDelay);
        }
        start++;
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

//--------- HEAP SORT -------------

void Thread::Heapify(int n, int i)
{
    int largest = i;
    int l = 2*i + 1;
    int r = 2*i + 2;

    if (l < n && columnsHeight[l] > columnsHeight[largest])
        largest = l;

    isAccessToArray();

    if (r < n && columnsHeight[r] > columnsHeight[largest])
        largest = r;

    isAccessToArray();

    if (largest != i)
    {
        swap(i, largest);
        Heapify(n, largest);

        msleep(sortDelay);
    }
}

void Thread::HeapSort()
{
    auto n = amount;

    for (int i = n / 2 - 1; i >= 0; i--)
    {
        Heapify(n, i);
    }

    for (int i=n-1; i>=0; i--)
    {
        swap(0, i);
        msleep(sortDelay);
        Heapify(i, 0);
    }
}
