//plagiarism detect


#include <iostream>
#include <fstream>
#include <unordered_set>
#include <string>
using namespace std;

class Detector
{
public:

    Detector() {}
    ~Detector() {}
    //returns 'true' if a line is shared
    bool check(ifstream file1, ifstream file2)
    {
        string line;
        while (getline(file1, line))
        {
            m_table.insert(line);
        }
        while (getline(file2, line))
        {
            if (m_table.insert(line).second == false)
                return true;
        }
        return false;
    }

private:
    //     using buffer = char[100];
    unordered_set<string> m_table;
};

int main(int argc, char* argv[]) {
    // your code goes here

    Detector d;
    bool result = d.check(ifstream(argv[1]), ifstream(argv[2]));
    cout << (result ? "yes" : "no");

    return 0;
}