#include <iostream>
#include <conio.h>
#include <algorithm>
using namespace std;

/*
((◣﹏◢))     ((◣﹏◢))     ((◣﹏◢))

Матвиенко Арсений Витальевич 3372

2 лабораторная работа
*/

/*
Задание: Реализовать сортировку Timsort. 
Ваша реализация должна включать все основные элементы алгоритма:

    Сортировка вставками
    Поиск последовательностей run
    Подсчет minrun
    Слияние последовательностей run
    Режим галопа при слиянии
*/



class Stack {
private:
    pair<int, int>* arr;
    int top;
    int capacity;

public:
    Stack(int size) {
        arr = new pair<int, int>[size];
        capacity = size;
        top = -1;
    }

    ~Stack() {
        delete[] arr;
    }

    void push(int start, int length) {
        if (top >= capacity - 1)
        {
            cout << "Стек переполнен" << endl;
        }
        top++;
        arr[top] = make_pair(start, length); // make_pair позволяет легко передать два значения, 
                                             // образующие пару, функции с аргументом типа pair
    }

    pair<int, int> pop()
    {
        if (top < 0) 
        {
            cout << "Стек пуст" << endl;
        }
        return arr[top];
        top--;
    }

    pair<int, int> peek()
    {
        if (top < 0) 
        {
            cout << "Стек пуст" << endl;
        }
        return arr[top];
    }

    bool isEmpty()
    {
        return top < 0;
    }

    int size()
    {
        return top + 1;
    }
};

class Array
{
public:
    int* array;
    int size;
    int capacity;

    Array(int cap) : size(0), capacity(cap)
    {
        array = new int[capacity];
    }

    ~Array()
    {
        delete[] array;
    }

    void add(int value) 
    {
        if (size >= capacity)
        {
            int newCapacity = capacity * 2;
            int* newArray = new int[newCapacity];
            for (int i = 0; i < size; i++)
            {
                newArray[i] = array[i];
            }
            delete[] array;
            array = newArray;
            capacity = newCapacity;
        }
        array[size++] = value;
    }

    void show()
    {
        for (int i = 0; i < size; i++)
        {
            cout << array[i] << " ";
        }
        cout << endl;
    }
private:

};

void insertionSort(int* arr, int left, int right) 
{
    for (int i = left + 1; i <= right; i++) 
    {
        int key = arr[i];
        int j = i - 1;

        while (j >= left && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

int getMinRun(int n) // Чтобы при делении общего количества элементов массива
{                    // на minRun получалось число, близкое к степени двойки
    int r = 0;       // (из диапазона от 32 до 64)
    while (n >= 64)
    {
        r |= n & 1;
        n >>= 1; 
    }
    return n + r;
}

// Чтобы найти индекс в R[] который больше чем L[i]
int binarySearch(int* R, int start, int end, int key) 
{
    while (start < end)
    {
        int mid = start + (end - start) / 2;
        if (R[mid] <= key)
            start = mid + 1;
        else
            end = mid;
    }
    return start; // Возвращаем индекс первого элемента большего key
}

void merge(int* arr, int left, int mid, int right)
{
    // Размеры двух подмассивов
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Создание временных массивов
    int* L = new int[n1];
    int* R = new int[n2];

    // Копирование данных во временные массивы
    for (int i = 0; i < n1; i++)
    {
        L[i] = arr[left + i];
    }
    for (int j = 0; j < n2; j++)
    {
        R[j] = arr[mid + 1 + j];
    }
    // Слияние временных массивов обратно в arr[left...right]
    int i = 0;
    int j = 0;
    int k = left;

    while (i < n1 && j < n2) 
    {
        if (L[i] <= R[j])
        {
            arr[k++] = L[i++];
        }
        else
        {
            int gallopIndex = binarySearch(R, j, n2, L[i]);
            // Вставляем элементы из R[j] в R[gallopIndex - 1]
            while (j < gallopIndex)
            {
                arr[k++] = R[j++];
            }
            arr[k++] = R[j++];
        }
    }

    // Копирование оставшихся элементов L[], если есть
    while (i < n1)
    {
        arr[k++] = L[i++];
    }

    // Копирование оставшихся элементов R[], если есть
    while (j < n2)
    {
        arr[k++] = R[j++];
    }

    delete[] L;
    delete[] R;
}

void timSort(int* arr, int size) // лучший - n, средний/худший - nLogn
{
    int minRun = getMinRun(size);

    // Сортируем подмассивы размера RUN вставками
    for (int start = 0; start < size; start += minRun)
    {
        insertionSort(arr, start, min(start + minRun - 1, size - 1));
    }

    // стек для хранения индексов подмассивов
    Stack mergeStack(size / minRun);

    for (int runSize = minRun; runSize < size;)
    {
        for (int leftStart = 0; leftStart < size; leftStart += runSize * 2)
        {
            int mid = min(leftStart + runSize - 1, size - 1);
            int rightEnd = min((leftStart + runSize * 2 - 1), (size - 1));

            if (mid < rightEnd)
            {
                mergeStack.push(leftStart, rightEnd); // Сохраняем только правый индекс
                merge(arr, leftStart, mid, rightEnd);
            }
        }

        runSize *= 2;

        // слияние из стека
        while (!mergeStack.isEmpty())
        {
            pair<int, int> bounds = mergeStack.pop();
            merge(arr, bounds.first, bounds.second - bounds.first / 2 - 1, bounds.second);
        }
    }
}

int main() {
    setlocale(LC_ALL, "RU");
    int choiceWhile = 0;

    while (choiceWhile != -1) {
        cout << "Выберите действие: " << endl
            << "0 - Выход из программы" << endl
            << "1 - Сортировка массива" << endl;

        int choiceCase;
        cin >> choiceCase;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Неверный ввод" << endl;
            continue;
        }
        switch (choiceCase)
        {
        case 1:
        {
            Array array(10);
            cout << "Введите числа в массив, для прекращения введите любой символ: " << endl;
            int value;
            while (true)
            {
                cin >> value;
                if (cin.fail())
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                }
                array.add(value);
            }
            cout << "Неотсортированный массив: " << endl;
            array.show();
            timSort(array.array, array.size);
            cout << "Отсортированный массив: " << endl;
            array.show();
            break;
        }
        case 0: 
        {
            choiceWhile = -1;
            break;
        }
        default:
        {
            cout << "Неверный ввод" << endl;
            break;
        }
        }
    }
    return 0;
}