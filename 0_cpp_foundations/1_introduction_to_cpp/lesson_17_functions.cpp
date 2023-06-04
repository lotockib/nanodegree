// functionas

#include <numeric>
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::vector;
using std::accumulate;

// Define a function "AdditionFunction" here.
// Instead of just two ints, this function should accept a vector<int> 
// as the argument, and it should return the sum of all the ints in the vector.

int AdditionFunctionv1(vector<int> vec) {
    int sum = 0;
    for (int i : vec) {
        sum += i;
    }
    return sum;
}

// use & to get reference, instead of making a copy (faster)
// use const to promise it won't be modified
int AdditionFunctionv2(const vector<int> &v)
{
    int sum = 0;
    for(const int &i : v)
        sum += i;
    return sum;
}

// use index based for loop, instead of range based
int AdditionFunctionv3(const vector<int> &v)
{
    int sum = 0;
    // iterator is not an int, so can use auto
    for(auto i = v.begin(); i!= v.end(); ++i)
        sum += *i; // TODO why does i need to be dereferenced?  it throws error without it
    return sum;
}

// use iterator based for loop
int AdditionFunctionv4(const vector<int> &v) {
    int sum = 0;
    for(auto i = v.begin(); i != v.end(); ++i)
        sum += *i;
    return sum;
}

// use accumulate function
int AdditionFunctionv5(const vector<int> &v) {
    return accumulate(v.begin(), v.end(), 0);
}

// void function
void PrintStrings(const std::string &test, const std::string &test2) {
    cout << test << " " << test2 << "\n";
}

int main() 
{
    vector<int> v {1, 2, 3};
    
    // Uncomment the following line to call your function:
    cout << AdditionFunctionv1(v) << "\n";
    cout << AdditionFunctionv2(v) << "\n";
    cout << AdditionFunctionv3(v) << "\n";
    cout << AdditionFunctionv4(v) << "\n";
    cout << AdditionFunctionv5(v) << "\n";

    // Void functions
    std::string s1 = "c++ is";
    std::string s2 = "super awesome";
    PrintStrings(s1, s2);
}