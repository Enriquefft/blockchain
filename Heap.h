#ifndef HEAP_H
#define HEAP_H

#include <iostream>
#include <vector>
#include <stdexcept>
#include "Data.hpp"

using namespace std;

class Heap
{
public:
    enum Type {
        MAX_HEAP, MIN_HEAP
    };

private:
    Data *elements;
    int capacity{};
    int n{};
    Type type;

public:
    Heap(Data *elements, int n, Type type=MAX_HEAP) : elements(elements), n(n), type(type)
    {
        this->capacity = n;
        buildHeap();
    }

    explicit Heap(int capacity, Type type=MAX_HEAP) : capacity(capacity), type(type)
    {
        this->n = 0;
        this->elements = new Data;
    }

    ~Heap()
    {
        delete[] elements;
    }

    void buildFromArray(Data *elements_, int n_)
    {
        this->elements = elements_;
        this->n = n_;
        buildHeap();
    }

    int size()
    {
        return n;
    }

    bool is_empty()
    {
        return n == 0;
    }

    void push(Data value)
    {
        if (n == capacity) {
            capacity *= 2;
            Data* newElements = new Data;
            for (int i = 0; i < n; i++) {
                newElements[i] = elements[i];
            }
            delete[] elements;
            elements = newElements;
        }

        elements[n] = value;
        n++;
        heapify_up(n - 1);
    }

    Data pop()
    {
        if (n == 0)
            throw out_of_range("El heap esta vacio");

        Data root = elements[0];
        elements[0] = elements[n - 1];
        n--;
        heapify_down(0);

        return root;
    }

    Data top()
    {
        if (n == 0)
            throw out_of_range("El heap esta vacio");

        return elements[0];
    }

    vector<Data> extractTheTopK(int k)
    {
        if (k > n)
            throw out_of_range("k es mayor que n");

        vector<Data> result;
        for (int i = 0; i < k; i++) {
            result.push_back(pop());
        }

        return result;
    }

    static void sortAsc(Data* arr, int n)
    {
        Heap heap(arr, n, MAX_HEAP);
        for (int i = n - 1; i >= 0; i--) {
            arr[i] = heap.pop();
        }
    }

    static void sortDesc(Data* arr, int n)
    {
        Heap heap(arr, n, MIN_HEAP);
        for (int i = n - 1; i >= 0; i--) {
            arr[i] = heap.pop();
        }
    }

private:
    int Parent(int i)
    {
        return (i - 1) / 2;
    }

    int Left(int i)
    {
        return (2 * i + 1);
    }

    int Right(int i)
    {
        return (2 * i + 2);
    }

    void heapify_down(int i)
    {
        int left = Left(i);
        int right = Right(i);
        int Mas_grande = i;

        if (type == MAX_HEAP && left < n && elements[left] > elements[Mas_grande])
            Mas_grande = left;
        if (type == MAX_HEAP &&
            right < n && elements[right] > elements[Mas_grande])
            Mas_grande = right;
        if (type == MIN_HEAP && left < n && elements[left] < elements[Mas_grande])
            Mas_grande = left;
        if (type == MIN_HEAP && right < n && elements[right] < elements[Mas_grande])
            Mas_grande = right;
        if (Mas_grande != i) {
            swap(elements[i], elements[Mas_grande]);
            heapify_down(Mas_grande);
        }
    }

    void heapify_up(int i)
    {
        int parent = Parent(i);

        if (type == MAX_HEAP && i > 0 && elements[i] > elements[parent]) {
            swap(elements[i], elements[parent]);
            heapify_up(parent);
        }

        if (type == MIN_HEAP && i > 0 && elements[i] < elements[parent]) {
            swap(elements[i], elements[parent]);
            heapify_up(parent);
        }
    }

    void buildHeap()
    {
        for (int i = (n / 2) - 1; i >= 0; i--) {
            heapify_down(i);
        }
    }
};

#endif // HEAP_H
