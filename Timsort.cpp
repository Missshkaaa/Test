#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

struct RUN
{
    int left;
    int size;
};

class Node
{
public:
    RUN run;
    Node* next;

    Node(int begin, int value_right)
    {
        run.left = begin;
        run.size = value_right;
    }
};

class Stack
{
public:
    int size;
    int fackt_size;
    Node* top;

public:
    Stack(int maxSize) : size(maxSize), fackt_size(0), top(nullptr) {}

    ~Stack()
    {
        while (top != nullptr)
            pop();
    }

    void push(int begin, int value_right)
    {
        if (isFull())
        {
            cout << "Стек переполнен" << endl;
            return;
        }

        Node* newElement = new Node(begin, value_right);
        newElement->next = top;
        top = newElement;
        fackt_size++;
    }

    void pop()
    {
        if (isEmpty())
        {
            cout << "Стэк пуст" << endl;
            return;
        }
        Node* new_run = top;
        Node* temp = top->next;
        delete top;
        top = temp;
        fackt_size--;
    }

    int peek_left()
    {
        return (top != nullptr) ? top->run.left : -1;
    }

    int peek_size()
    {
        return (top != nullptr) ? top->run.size : -1;
    }

    bool isEmpty()
    {
        return fackt_size == 0;
    }

    bool isFull()
    {
        return fackt_size >= size;
    }
};
/////////////////////////////////////////////////////////

void insertionSortFunction(int* arr, int left, int right)
{
    for (int i = left + 1; i < right; i++)
    {
        for (int y = i; y >= left; y--)
        {
            if (arr[y - 1] > arr[y])
            {
                int temp = arr[y];
                arr[y] = arr[y - 1];
                arr[y - 1] = temp;
            }
        }
    }
}

int GetMinrun(int n)
{
    int r = 0;           /* станет 1 если среди сдвинутых битов будет хотя бы 1 ненулевой */
    while (n >= 64) {
        r |= n & 1;
        n >>= 1;
    }
    return n + r;
}

void found_run(int* arr, int& i, int& minrun, int& sum_run, int* numb_run_arr, int& size, int* size_run_arr)
{
    numb_run_arr[sum_run] = i;
    int run_size = 0;
    int j = i + 1;
    while (run_size < minrun || arr[j - 1] <= arr[j])
    {
        if (i == size)
            break;
        run_size++;
        i++;
        j++;
    }
    size_run_arr[sum_run] = run_size;
    sum_run++;
}

void merge(int* arr, int left1, int right1, int left2, int right2) {
    int* temp = new int[right2 - left1 + 1];
    int i = left1, j = left2, k = 0;
    while (i <= right1 && j <= right2) {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        }
        else {
            temp[k++] = arr[j++];
        }
    }
    while (i <= right1) {
        temp[k++] = arr[i++];
    }
    while (j <= right2) {
        temp[k++] = arr[j++];
    }
    for (i = 0; i < k; i++) {
        arr[left1 + i] = temp[i];
    }
    delete[] temp;
}

void Timsort(int* arr, int size)
{
    int minrun = GetMinrun(size);
    int run_size = 0;
    int sum_run = 0;
    int* numb_run_arr = new int[size];
    int* size_run_arr = new int[size];
    int i = 0;

    while (i < size)
    {
        found_run(arr, i, minrun, sum_run, numb_run_arr, size, size_run_arr);
    }
    cout << "Номер элемента с начала рун: " << endl;
    for (int f = 0; f < sum_run; f++)
        cout << numb_run_arr[f] << " ";
    cout << size;
    cout << endl << endl;

    int t = 0;
    for (t = 0; t < sum_run - 1; t++)
    {
        insertionSortFunction(arr, numb_run_arr[t], numb_run_arr[t + 1]);
    }
    insertionSortFunction(arr, numb_run_arr[t], size);

    ///////////////////////////////

    int max_stack_size = size / minrun + 1;
    Stack stack(max_stack_size);

    for (int u = 0; u < sum_run; u++)
    {
        stack.push(numb_run_arr[u], size_run_arr[u]);
    }

    while (stack.fackt_size > 1) {
        RUN X;
        RUN Y;
        X.left = stack.peek_left();
        X.size = stack.peek_size();
        stack.pop();
        Y.left = stack.peek_left();
        Y.size = stack.peek_size();
        stack.pop();

        // Слияние  двух  подмассивов
        if (X.size <= Y.size) {
            merge(arr, X.left, X.left + X.size - 1, Y.left, Y.left + Y.size - 1);
            stack.push(X.left, X.size + Y.size);
        }
        else {
            merge(arr, Y.left, Y.left + Y.size - 1, X.left, X.left + X.size - 1);
            stack.push(Y.left, X.size + Y.size);
        }
    }

    delete[] numb_run_arr;
    delete[] size_run_arr;
}

int main() {
    setlocale(0, "ru");
    int size = 128;
    int arr[128];
    for (int i = 0; i < size; i++)
        arr[i] = rand() % 100;
    cout << "Первоначальный массив: " << endl;
    for (int i = 0; i < size; i++)
        cout << arr[i] << " ";
    cout << endl << endl;

    Timsort(arr, size);

    cout << "Размер минрана = " << GetMinrun(size) << endl << endl;

    cout << "Отсортированный массив:" << endl;
    for (int n = 0; n < size; n++)
        cout << arr[n] << " ";
    cout << endl << endl;

    cout << "Мир" << endl;

    return 0;
}
