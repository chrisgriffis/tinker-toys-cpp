//fold and zip a linked list

#include <iostream>
#include <fstream>
#include <list>
using namespace std;

list<int> zip(list<int> l);
void showlist(list<int>& l);

int main(int argc, char* argv[])
{
    int val;
    list<int> unzipped;
    ifstream file1("1.txt");
    while (file1 >> val)
        unzipped.push_back(val);
    showlist(unzipped);
    cout << endl;
    showlist(zip(unzipped));

}
list<int> zip(list<int> l)
{
    list<int> zipped;
    auto nodeslow = l.begin();
    auto nodefast = l.begin();
    while ((nodeslow != l.end()) && (nodefast != l.end()))
    {
        ++nodeslow;
        ++nodefast;
        if (nodefast != l.end())
        {
            ++nodefast;
        }
        else break;
    }
    list<int> reversedsecondhalf;
    auto reversedsecondhalfhead(nodeslow);
    while (reversedsecondhalfhead != l.end())
    {
        reversedsecondhalf.push_front(*reversedsecondhalfhead++);
    }
    auto firsthalf = l.begin();
    auto secondhalf = reversedsecondhalf.begin();

    while (secondhalf != reversedsecondhalf.end())
    {
        zipped.push_back(*firsthalf++);
        zipped.push_back(*secondhalf++);
    }
    if (*firsthalf != zipped.back())zipped.push_back(*firsthalf);
    return zipped;
}
void showlist(list<int>& l)
{
    auto ptr = l.begin();
    while (ptr != l.end())
    {
        cout << *ptr++ << " ";
    }
}
