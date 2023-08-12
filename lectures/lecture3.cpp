#include <iostream>

using namespace std;


void printInfo(int &i) {
    cout << "address:" << &i << " value:" << i << " size:" << sizeof i << endl;
}

void allocate() {
    int a = 10;
    int b = 25;

    printInfo(a);
    printInfo(b);
}

void arrAllocate() {
    int arr[10] = {}; // with = {} we are given a default value to the arr
    for (size_t i = 0; i < 10; ++i) {
        printInfo(arr[i]);
    }

    int *heapArr = new int[10];
    for (size_t i = 0; i < 10; ++i) {
        printInfo(heapArr[i]);
    }
}

void pointersTo() {
    int a = 10;
    int b = 25;
    int *pb = &b;
    int *pa = &a;

    //*pa = 17;// here we change the value of "a" to 17
    //*(&a) = 66;// the same as *pa, because we are changing the value throw dereference

//    *(pb) = 1000;
    *(pb - 1) = 5000;// this could potentially change the value of "a", depends on compiler

    printInfo(a);
    printInfo(b);
}

class IntArray { // normal implementation
    int *m_arr;
    size_t m_size;

public:
    IntArray(size_t size)
            : m_arr(new int[size]),
              m_size(size) {
        cout << "arr constructor" << endl;
    }
    ~IntArray(){
        delete []m_arr;
        cout << "array destructor"<<endl;
    }

    void set(size_t index, int value) const {
        m_arr[index] = value;
    }

    int get(size_t index) const {
        return m_arr[index];
    }

    void print() const {
        for (size_t i = 0; i < m_size; ++i) {
            cout << i << " " << m_arr[i] << endl;
        }
    }
};


template <typename T>
class GenericArray { // generic implementation
    T *m_arr;
    size_t m_size;

public:
    GenericArray(size_t size)
            : m_arr(new T[size]),
              m_size(size) {
        cout << "generic arr constructor" << endl;
    }
    ~GenericArray(){
        delete []m_arr;
        cout << "generic array destructor"<<endl;
    }

    void set(size_t index, T value) const {
        m_arr[index] = value;
    }

    T get(size_t index) const {
        return m_arr[index];
    }

    void print() const {
        for (size_t i = 0; i < m_size; ++i) {
            cout << i << " " << m_arr[i] << endl;
        }
    }
};


template <typename T>
class GenOperatorOverloadingArr { // generic implementation
    T *m_arr;
    size_t m_size;

public:
    GenOperatorOverloadingArr(size_t size)
            : m_arr(new T[size]),
              m_size(size) {
        cout << "generic operator arr constructor" << endl;
    }
    ~GenOperatorOverloadingArr(){
        delete []m_arr;
        cout << "generic operator array destructor"<<endl;
    }

    // c++ allow us to have 2 functions that differs on the const constraint
    // with this it can choose in what scenario use the correct one
    // it's a must in a data structure class
    const T &operator[](size_t index) const{
        return m_arr[index];
    }

    T &operator[](size_t index) {
        return m_arr[index];
    }

    void print() const {
        for (size_t i = 0; i < m_size; ++i) {
            cout << i << " " << m_arr[i] << endl;
        }
    }
};

void usingARII(){
    IntArray myArr(10);
    myArr.set(2,1000);
    myArr.set(6,242);
    myArr.print();

    GenericArray<float> floatArray(5);
    floatArray.set(1, 10.1);
    floatArray.set(3, 99.99);
    floatArray.print();

    GenericArray<string> stringArray(5);
    stringArray.set(1, "hello");
    stringArray.set(3, "generics");
    stringArray.print();

    GenOperatorOverloadingArr<string> operatorArr(5);
    operatorArr[0] = "hello";
    operatorArr.print();

    GenOperatorOverloadingArr<bool> boolArr(5);
    boolArr[0] = "hello";
    boolArr.print();
}

int main() {
//    allocate();
//    arrAllocate();
//pointersTo();
    usingARII();
    return 0;
}

