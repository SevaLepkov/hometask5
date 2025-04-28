#include<iostream>
#include <stdio.h>
#include <stdlib.h>
#include "pattern.h"
#pragma once
#include <vector>
#include <list>
using namespace std;


enum class Color : int
{
    Green,
    Red,
    Orange,
    Unknown
};

enum class qualityEnum : int
{
    whole,
    spoiled,
    rotten,

    None
};

class TestStrategy{
public:
    virtual ~TestStrategy(){}
    virtual void test() = 0;
};

class TestWholeStrategy : public TestStrategy
{
    void test() {cout << "amam delishes";}
};
class TestSpoiledStrategy : public TestStrategy
{
    void test() {cout << "fuuu is junk";}
};
class TestRottenStrategy : public TestStrategy
{
    void test() {cout << "is not wear testy";}
};


TestStrategy *CreateTestStrategy(qualityEnum testManner)
 {
   switch(testManner)
   {
     case qualityEnum::whole: return new TestWholeStrategy;
     case qualityEnum::spoiled: return new TestSpoiledStrategy;
     case qualityEnum::rotten: return new TestRottenStrategy;

     default: return nullptr;
   }
 }


class vegetable {
private:
    int weight = 1;
    Color color;
    //bool children;
    TestStrategy *testManner;

    void DoTestUsingStrategy() {
        if(testManner == nullptr)
        {
            cout << "Do nothing!";
            return;
        }
        else
        {
            testManner -> test();
        }
    }
    void DetectedSeed()
    {
        if(Haveseed()){
            cout << "need extract";
        }
        else{
            cout << "all good";
        }
    }

protected:
    int lenght = 1;
    bool seed;

public:
    ~vegetable(){
        if (testManner != nullptr) delete testManner;
    };
    vegetable(Color color) :weight(55), color(color), lenght(165), seed(false), testManner(nullptr) {
        seed = static_cast<bool>(rand() % 2);
    }

    bool Haveseed() const { return seed; }

    Color GetColor() const { return color; }

    virtual void Cut()
    {
        if (Haveseed())
        {
            cout << "We need extract seed (";
        }
        else
        {
            cout << "Nice! no seed ) ";
        }
    }

    virtual void printType() = 0;

    virtual void Peel() = 0;
    void test() {

        printType();
        cout << " : ";
        DetectedSeed();
        cout << " : ";

        Cut();
        cout << " : ";
        DoTestUsingStrategy();
        cout << endl;
    }
     void SetTestManner(TestStrategy *TManner)
    {
      testManner = TManner;
    };

};
class cucumber : public vegetable {
protected:
    bool taste = false;
public:
    cucumber() : vegetable(Color::Green) { SetTestManner(CreateTestStrategy(qualityEnum::whole)); }

    ~cucumber() { }//cout << "cucumber removed" << endl; }
    void Cut() override;
    void printType(){cout << "cucumber";}
    void Peel() {cout << "delete peel";}

};

void cucumber::Cut()
{
    vegetable::Cut(); // Вызов функции, определенной в базовом классе
    cout << "slicing cucumber";
}


class carot : public vegetable {
protected:
    bool taste = false;
public:
    //carot();
    carot() : vegetable(Color::Orange) { SetTestManner(CreateTestStrategy(qualityEnum::spoiled)); }

    ~carot() { }//cout << "carot removed" << endl; }
    void Cut() override;
    void printType(){cout << "carot";}
    void Peel() {cout << "wash peel";}

};

void carot::Cut()
{
    vegetable::Cut(); // Вызов функции, определенной в базовом классе
    cout << "slicing carot";
}


class tomato : public vegetable {
protected:
    bool taste = true;
public:
    tomato() : vegetable(Color::Orange) { SetTestManner(CreateTestStrategy(qualityEnum::rotten)); }
    ~tomato() { }//cout << "tomato removed" << endl; }
    void printType(){cout << "tomato";}
    void Peel() {cout << "no peel";}
    void Cut() override;
};

void tomato::Cut()
{
    vegetable::Cut(); // Вызов функции, определенной в базовом классе
    cout << "dicing a tomato";
}

enum class vegetableType : int
{
    tomato = 1,
    carot = 2,
    cucumber = 3,

    Undefined = 0

};

vegetable* createVegetable(vegetableType type) {

	vegetable* newVegetable = nullptr;

	if (type == vegetableType::tomato) {
		newVegetable = new tomato;

	}
	else if (type == vegetableType::carot) {
		newVegetable = new carot;
	}
	else if(type == vegetableType::cucumber){
		newVegetable = new cucumber;
	}
	return newVegetable;
}

void getAllVegetable(Iterator<vegetable*> *it)
{
    for(it->First(); !it->IsDone(); it->Next())
    {
        vegetable *currentVegetable = it->GetCurrent();
        currentVegetable->Cut();
    }
}

class VegetableColorDecorator : public IteratorDecorator<class vegetable*>
{
private:
    Color TargetColor;

public:
    VegetableColorDecorator(Iterator<vegetable*>* it, Color color)
        : IteratorDecorator<vegetable*>(it), TargetColor(color) {}

    void First()
    {
        this->It->First();
        while (!this->It->IsDone() && It->GetCurrent()->GetColor() != TargetColor)
        {
            this->It->Next();
        }
    }

    void Next()
    {
        do
        {
            this->It->Next();

        } while (!this->It->IsDone() && It->GetCurrent()->GetColor() != TargetColor);
    }
};

// Декоратор итератора для выделения
class VegetableDecorator : public IteratorDecorator<class vegetable*>
{
private:
    bool TargetSeed;

public:
    VegetableDecorator(Iterator<vegetable*>* it, bool seed) : IteratorDecorator<vegetable*>(it), TargetSeed(seed) {}

    void First()
    {
        this->It->First();
        while (!this->It->IsDone() && this->It->GetCurrent()->Haveseed() != TargetSeed)
        {
            this->It->Next();
        }
    }

    void Next()
    {
        do
        {
            this->It->Next();

        } while (!this->It->IsDone() && this->It->GetCurrent()->Haveseed() != TargetSeed);
    }
};

void TestEmAll(Iterator<vegetable*> *it)
 {
     for(it->First(); !it->IsDone(); it->Next())
     {
         vegetable *currentVegetable = it->GetCurrent();
         currentVegetable->test();
     }
 }


int main()

{

    /*cout << "choose vegetable (1 - tomato, 2 - carot,  3 - cucumber)" << endl;

    vegetableType type = vegetableType::Undefined;
    int ii;
    cin >> ii;
    type = static_cast<vegetableType>(ii);

    //сложный способ создания обьектов

    vegetable *newVegetable = nullptr;
    if(type == vegetableType::tomato){
        newVegetable = new tomato;
    }
    else if(type == vegetableType::carot){
        newVegetable = new carot;
    }
    else if(type == vegetableType::cucumber){
        newVegetable = new cucumber;
    }

    //newVegetable->eat();
    //delete newVegetable;

    vegetable *newVegetable2 = createVegetable(type);
    //newVegetable2->eat();
    //delete newVegetable2;

    cout << endl << endl;
    size_t N = 0;
    cout << "How many vegetable you need? : ";
    cin >> N;

    cout << endl;






    StackClass<carot> carotStack;
    for(size_t i=0; i<N; i++)
    {
        carot newCarot;
        carotStack.Push(newCarot);
    }

    cout << "size steck carot: " << carotStack.Size() << endl;*/
    int N = 6;





     StackClass<vegetable*> vegetableStack;
    for(size_t i=0; i<N; i++)
    {
        int vegetable_num = rand()%3+1; // Число от 1 до 3
        vegetableType vegetable_type = static_cast<vegetableType>(vegetable_num);
        vegetable *newVegetable= createVegetable(vegetable_type);
        vegetableStack.Push(newVegetable);
    }

    cout << "size steck vegetable: " << vegetableStack.Size() << endl;




    Iterator<vegetable*>* it2 = vegetableStack.GetIterator();

    cout << endl;



    Array<vegetable*> vegetableArray;
    for(size_t i=0; i<N; i++)
    {
        int vegetable_num = rand()%3+1; // Число от 1 до 3 (случайный woman)
        vegetableType vegetable_type = static_cast<vegetableType>(vegetable_num);
        vegetable *newvegetable = createVegetable(vegetable_type);
        vegetableArray.Add(newvegetable);
    }

    wcout << "size array vegetable: " << vegetableStack.Size() << endl;


    list<vegetable*> vegetableVector;
    for (size_t i = 0; i < N; i++)
    {
        int vegetable_num = rand() % 3 + 1;
        vegetableType vegetable_type = static_cast<vegetableType>(vegetable_num);
        vegetable* newvegetable = createVegetable(vegetable_type);
        vegetableVector.push_back(newvegetable);
    }

    cout << "Stack class";

        cout << endl << "size stack: " << vegetableStack.Size() << endl;
        Iterator<vegetable*> *it7 = vegetableStack.GetIterator();
        TestEmAll(it7);
    cout << endl << "woman with black skin and children:" << endl;
        Iterator<vegetable*>* seedSkinIt =
        new VegetableDecorator(new VegetableColorDecorator(vegetableStack.GetIterator(), Color::Green), true);
        TestEmAll(seedSkinIt);
        delete seedSkinIt;

    // Обход всех овощей с семечкой
    cout << "Array class "<< endl;
    cout << endl << "vegetable with seed:" << endl;
    Iterator<vegetable*>* seedIt = new VegetableDecorator(vegetableArray.GetIterator(), true);
    TestEmAll(seedIt);
    delete seedIt;

    // Обход всех зеленых овощей
    cout << endl << "vegetable with green color:" << endl;
    Iterator<vegetable*>* ColorIt = new VegetableColorDecorator(vegetableArray.GetIterator(), Color::Green);
    TestEmAll(ColorIt);
    delete ColorIt;

    // Обход всех зеленых овощей с семечком
    cout << endl << "vegetable with green color and have seed:" << endl;
    Iterator<vegetable*>* haveseedIt =
    new VegetableDecorator(new VegetableColorDecorator(vegetableArray.GetIterator(), Color::Green), true);
    TestEmAll( haveseedIt);
    delete  haveseedIt;

    // Демонстрация работы адаптера
    cout << endl << "green vegetable with seed using adapted iterator (another container):" << endl;
    Iterator<vegetable*>* adaptedIt = new ConstIteratorAdapter<std::list<vegetable*>, vegetable*>(&vegetableVector);
    Iterator<vegetable*>* adaptedGreenIt = new VegetableDecorator(new VegetableColorDecorator(adaptedIt, Color::Green), true);
    TestEmAll(adaptedGreenIt);
    delete adaptedGreenIt;

    cout << "STL vegetable:" << endl;
    vector<vegetable*> stlVector;
        for (int i = 0; i < 5; ++i) {
            int vegetable_num = rand() % 3 + 1;
            vegetableType vegetable_type = static_cast<vegetableType>(vegetable_num);
            vegetable* newvegetable = createVegetable(vegetable_type);
            stlVector.push_back(newvegetable);
    }

        Iterator<vegetable*>* stlIt = new ConstIteratorAdapter<vector<vegetable*>, vegetable*>(&stlVector);
        TestEmAll(stlIt);
        delete stlIt;

    return 0;
}
