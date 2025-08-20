#include <iostream>
#include <queue>

using namespace std;


priority_queue<int> q1;
priority_queue<int, vector<int>, greater<int>> q2;

void process(int data)
{
    if(q1.empty() || q1.top()>data)
        q1.push(data);
    else
        q2.push(data);
    
    if(q1.size() > q2.size()+1)
    {
        q2.push(q1.top());
        q1.pop();
    }

    if(q2.size() > q1.size()+1)
    {
        q1.push(q2.top());
        q2.pop();
    }
    
}

int get_median()
{
    int t1= q1.top();
    int t2= q2.top();

    if(q1.size() == q2.size())
        return ((t1+t2)/2);
        
    else
    return q1.size()>q2.size()?t1:t2;


}

int main()
{
    vector <int>vt= {5,2,3};
    for(auto v : vt)
    {
        process(v);
    }
    cout << "get_median() : " << get_median() << endl;
    return 0;
}