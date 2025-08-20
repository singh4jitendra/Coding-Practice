#include <iostream>
#include <vector>

using namespace std;



int merge(vector<int> &vec, int start, int mid, int end)
{   
    int i=start;
    int j=mid+1;
    int invcount=0;
    vector<int> res;
    while(i<=mid && j<=end)
    {
        if(vec[i]<=vec[j])
        {
            res.push_back(vec[i]);
            i++;
        }
        else
        {
            res.push_back(vec[j]);
            j++;
            invcount++;
        }
    }
    while(i<=mid){
        res.push_back(vec[i]);
        i++;
    }
    while(j<=end){
        res.push_back(vec[j]);
        j++;
    }
    for(int k=0; k<res.size(); k++)
    {   
        vec[start+k] = res[k];
    }
    return invcount;
}


int merge_sort(vector<int> &vt, int start, int end)
{
    if(start<end)
    {
        int mid = (start+end)/2;
        int lic= merge_sort(vt, start, mid);
        int loc= merge_sort(vt, mid+1, end);
        int ic = merge(vt,start, mid, end);
        return lic+loc+ic;
    }
    return 0;
    
}

int main()
{
    vector<int> vt={3,2,1,5,7,6};
    int d = merge_sort(vt,0,vt.size()-1);
    for(auto val: vt)
    {
        cout << val <<", ";
    }
    cout << d << endl;
    return 0;
}