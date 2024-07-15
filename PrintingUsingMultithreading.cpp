#include <bits/stdc++.h>
#include <thread>
using namespace std;
int turn = 1;
int n = 20;
void print(int k,string th)
{
    while(turn != k%2)
    {

    }
    cout<<th<<" : "<<k<<" "<<endl;
    turn = (k+1)%2;

    if(k+2 <= n)
    print(k+2,th);
}
int main()
{

    
    int l = 1;

    thread x = thread(print,1,"Odd Thread");
    thread y = thread(print,2,"Even Thread");

    
    x.join();
    y.join();

    return 0;
}