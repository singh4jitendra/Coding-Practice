#include <iostream>
#include <vector>

using namespace std;
#define SIZE 4

int ar[SIZE];
int front=-1;
int rear =-1;

void enq(int data)
{
    if(front == -1)
    {
        front =0;
    }
    int idx = (rear + 1) % SIZE; // Circular increment
    ar[idx] = data;
    rear = idx;
}

int deq()
{
     if((front == -1 && rear ==-1))  // Check if queue is empty   
     {
        cout << "Queue is empty" << endl;
        return -1;
     }
     else{
        int tmp = ar[front%SIZE];
        front = (front + 1) % SIZE;
        return tmp;
     }

}

void display()
{
    cout << "Queue: ";
    for(int i=SIZE-1;i>=front; i--)
    {
        cout << ar[i] << " ";
    }
    cout << endl;
}

int main()
{
    enq(1);
    enq(2);
    enq(3);
    enq(4);
    display();
    enq(5);
    display();
    
    enq(6);
    display();
    enq(7);
    display();
    enq(8);
    display();
    enq(9);
    display();
    enq(10);
    display();


    cout << deq() << endl;
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