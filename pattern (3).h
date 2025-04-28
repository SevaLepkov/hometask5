#ifndef PATTERN_H_INCLUDED
#define PATTERN_H_INCLUDED

#pragma once


#include <vector>
#include <cstddef>

using namespace std;

// Абстрактный класс итератора
template <typename T>
class Iterator {
public:
    virtual ~Iterator() {}
    virtual void First() = 0;
    virtual void Next() = 0;
    virtual bool IsDone() const = 0;
    virtual T GetCurrent() const = 0;
};

// Абстрактный интерфейс для всех контейнеров
template <typename T>
class Containers {
public:
    virtual Iterator<T>* GetIterator() = 0;
    virtual ~Containers() {}
};


// стек с итератором
const size_t MaxSize = 100;

template <typename T>
class StackClass : public Containers<T> {
    T items[MaxSize];
    size_t Top;

public:
    StackClass() : Top(0) {}

    void Push(T value) { items[Top++] = value; }
    T Pop() { return items[--Top]; }
    size_t Size() const { return Top; }
    T Get(size_t index) const { return items[index]; }

    class StackIterator : public Iterator<T> {
        const StackClass<T>* container;
        size_t pos;
    public:
        StackIterator(const StackClass<T>* cont) : container(cont), pos(0) {}
        void First() override { pos = 0; }
        void Next() override { ++pos; }
        bool IsDone() const override { return pos >= container->Size(); }
        T GetCurrent() const override { return container->Get(pos); }
    };

    Iterator<T>* GetIterator() override {
        return new StackIterator(this);
    }
};

// массив с итератором
template <typename T> class Array : public Containers<T> {
    T items[MaxSize];
    size_t arraySize;

public:
    Array() : arraySize(0) {}

    void Add(T value) { items[arraySize++] = value; }
    size_t Size() const { return arraySize; }
    T Get(size_t index) const { return items[index]; }

    class ArrayIterator : public Iterator<T> {
        const Array<T>* container;
        size_t pos;
    public:
        ArrayIterator(const Array<T>* cont) : container(cont), pos(0) {}
        void First() override { pos = 0; }
        void Next() override { ++pos; }
        bool IsDone() const override { return pos >= container->Size(); }
        T GetCurrent() const override { return container->Get(pos); }
    };

    Iterator<T>* GetIterator() override {
        return new ArrayIterator(this);
    }
};

// вектор с итератором
template <typename T> class Vector : public Containers<T> {
    vector<T> items;

public:
    Vector() {}

    void Add(T value) { items.push_back(value); }
    size_t Size() const { return items.size(); }
    T Get(size_t index) const { return items[index]; }

    class VectorIterator : public Iterator<T> {
        const Vector<T>* container;
        size_t pos;
    public:
        VectorIterator(const Vector<T>* cont) : container(cont), pos(0) {}
        void First() override { pos = 0; }
        void Next() override { ++pos; }
        bool IsDone() const override { return pos >= container->Size(); }
        T GetCurrent() const override { return container->Get(pos); }
    };

    Iterator<T>* GetIterator() override {
        return new VectorIterator(this);
    }
};

// абстрактный декоратор для итератора
template<typename T>
class IteratorDecorator : public Iterator<T>
{
protected:
    Iterator<T>* It;

public:
    IteratorDecorator(Iterator<T>* it) : It(it) {}
    virtual ~IteratorDecorator() { delete It; }
    virtual void First() { It->First(); }
    virtual void Next() { It->Next(); }
    virtual bool IsDone() const { return It->IsDone(); }
    virtual T GetCurrent() const { return It->GetCurrent(); }
};

// Адаптер STL
template <typename ContainerType, typename ItemType> class ConstIteratorAdapter : public Iterator<ItemType> {
protected:
    ContainerType* Container;
    typename ContainerType::const_iterator It;

public:
    ConstIteratorAdapter(ContainerType* container) : Container(container) {
        It = Container->begin();
    }

    virtual ~ConstIteratorAdapter() {}

    void First() override { It = Container->begin(); }

    void Next() override { ++It; }

    bool IsDone() const override { return It == Container->end(); }

    ItemType GetCurrent() const override { return *It; }
};




#endif // PATTERN_H_INCLUDED
