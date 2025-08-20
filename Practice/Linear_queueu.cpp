#include <iostream>
#include <vector>
#include <thread>
#include <mutex>    

using namespace std;
#define SIZE 5

int ar[SIZE];
int front=-1;
int rear =-1;
mutex mtx;

void enq(int data)
{
    mtx.lock();
    if (rear == SIZE - 1) {
        cout << "Queue is full" << endl;
        cout << rear << endl;
        return;
    }
    else if(front == -1)
    {
        front =0;
    }
    rear++;
    ar[rear] = data;
    mtx.unlock();
}

int deq()
{
    mtx.lock();
     if((front == -1 && rear ==-1))  // Check if queue is empty   
     {
        cout << "Queue is empty" << endl;
        mtx.unlock();
        return -1;
     }
     if(rear < front)
     {
        front = -1;
        rear =-1;
        mtx.unlock();
        return -1;
     }
     else{
        int tmp = ar[front];
        front++;
        mtx.unlock();
        return tmp;
     }

}

void display()
{
    cout << "Queue: ";
    for(int i=front ;i<=rear; i++ )
    {
        cout << ar[i] << " ";
    }
    cout << endl;
}

int main()
{
    cout << "Linear Queue Implementation using C++14" << endl;

    thread t1[5];
    for(int i=0; i<5; i++)
    {
        t1[i] = thread(enq, i+1);
    }

    for(int i=0; i<5; i++)
    {
        t1[i].join();
    }

    enq(1);
    enq(2);
    enq(3);
    enq(4);
    enq(5);
    display();
    enq(6);
    enq(7);
    enq(8);
    enq(9);
    enq(10);
    display();
    enq(11); // Should print "Queue is full"
    display();
    cout << deq() << endl;
    display();
    cout << deq() << endl;
    display();
    cout << deq() << endl;
    display();
    cout << deq() << endl;
    display();

    return 0;
}