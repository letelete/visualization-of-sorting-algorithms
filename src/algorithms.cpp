#include "includes/algorithms.h"

Thread::Thread(int ms, int alg, int n, std::vector<float> col, QObject *parent = nullptr) : QThread(parent)
{
    sortDelay = ms;
    sortWith = alg;
    amount = n;
    columnsHeight = col;
    arrayAccessVariable = 0;
}

void Thread::run()
{
    switch(sortWith)
    {
        case 0:
            BubbleSort();
            break;
        case 1:
            CocktailSort();
            break;
    }
}

void Thread::Sorted()
{
    for(auto i=0; i<amount; i++)
    {
        emit sortDone(i);
        msleep(5);
    }
    emit changeButtonStatus(2);
}

//---------- BUBBLE SORT ----------

void Thread::BubbleSort()
{
    for(auto i=0; i<amount; i++)
        {
            for(auto j=0; j<amount-1; j++)
            {
                if(columnsHeight[j] > columnsHeight[j+1])
                {
                    std::swap(columnsHeight[j], columnsHeight[j+1]);
                    emit comparision(j, j+1);
                }
                emit arrayAccess(arrayAccessVariable);
                arrayAccessVariable++;

                msleep(sortDelay);
            }
    }
    Sorted();
}

//--------- COCKTAIL SORT ---------

void Thread::CocktailSort()
{
    bool swapped = true;
    int start = 0;
    int end = amount-1;

    while (swapped)
    {
        swapped = false;
        for (int i = start; i < end; ++i)
        {
            if (columnsHeight[i] > columnsHeight[i + 1])
            {
                std::swap(columnsHeight[i], columnsHeight[i+1]);
                emit comparision(i, i+1);
                swapped = true;
            }
            emit arrayAccess(arrayAccessVariable);
            arrayAccessVariable++;

            msleep(sortDelay);
        }

        if (!swapped)
            break;

        swapped = false;
        --end;

        for (int i = end - 1; i >= start; --i)
        {
            if (columnsHeight[i] > columnsHeight[i + 1])
            {
                std::swap(columnsHeight[i], columnsHeight[i+1]);
                emit comparision(i, i+1);
                swapped = true;
            }
            emit arrayAccess(arrayAccessVariable);
            arrayAccessVariable++;

            msleep(sortDelay);
        }
        ++start;
    }
    Sorted();
}
