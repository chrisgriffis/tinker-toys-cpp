//google jan 2018 slot 4/5

#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

enum color {white=0,black,blue,green,red,color_end};
using code = vector<color>;
struct score
{
    score() :white(0), black(0) {}
    int black;
    int white;
};
ostream& operator<<(ostream& s, const score& sc)
{
    s << "black " << sc.black << " white " << sc.white << endl;
    return s;
}
struct mmind
{
    mmind(code c) :_c(move(c)) {}
    score grade(code attempt)
    {
        score ret;
        vector<size_t> keep;
        unordered_map<color,int> kept;
        for (size_t idx = 0; idx < attempt.size() ; ++idx)
        {
            if (attempt[idx] == _c[idx])
            {
                ++ret.black;
            }
            else
            {
                ++kept[attempt[idx]];
                keep.emplace_back(idx);
            }
        }
        for (size_t idx = 0; idx < keep.size(); ++idx)
        {
        	if (kept[_c[idx]] > 0)
        	{
                --kept[_c[idx]];
                ++ret.white;
        	}
        }
        return ret;
    }
private:
    code _c;
};


int main()
{
    mmind mm(code{ white,white,black,green,red });
    cout << "code: white,white,black,green,red" << endl;
    cout << "mm.grade(code{ black,green,red,white,blue }) " << mm.grade(code{ black,green,red,white,blue });
    cout << "mm.grade(code{ green,red,white,blue,white }) " << mm.grade(code{ green,red,white,blue,white });
    cout << "mm.grade(code{ black,blue,blue ,red,white }) " << mm.grade(code{ black,blue,blue ,red,white});
    cout << "mm.grade(code{ black,white,green,red,blue }) " << mm.grade(code{ black,white,green,red,blue });
    cout << "mm.grade(code{ white,white,black,green,red }) " << mm.grade(code{ white,white,black,green,red });

    int sdfgwert;
	cin >> sdfgwert;
    return 0;
}