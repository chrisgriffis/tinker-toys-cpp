#include <fstream>
#include <string>
#include <stack>

int main(int argc, char **argv)
{
    using namespace std;
    ifstream f(argv[1]);
    ofstream out(argv[2]);
    string s;
    stack<string> strstk;
    while(getline(f,s))
    {
        strstk.push(s);
        s.clear();
    }

    while (!strstk.empty())
    {
        s = strstk.top();
        strstk.pop();
        out << s << '\n';
    }

    f.close();
    out.flush();
    out.close();
    return 0;

}