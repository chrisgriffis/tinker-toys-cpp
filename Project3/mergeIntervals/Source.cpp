#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;
using interval = std::pair<int, int>;
using intervals = vector<interval>;


intervals merge_intervals(intervals& intvls)
{
    intervals merged;
    if (intvls.size() < 1) return merged;
    sort(intvls.begin(), intvls.end());
    int left = intvls[0].first;
    int right = intvls[0].second;
    for (unsigned Index = 1; Index < intvls.size(); Index++)
    {
        interval& curr = intvls[Index];
        if (curr.first > right)
        {
            merged.emplace_back(interval{ left,right });
            left = curr.first;
            right = curr.second;
            continue;
        }
        else if (curr.second > right)
        {
            right = curr.second;
        }
    }
    merged.emplace_back(interval{ left,right });
    return merged;
}
ostream& operator<<(ostream& s, interval& i) { s << "[" << i.first << "," << i.second << "] "; return s; }

int main()
{
    //  0   1   2   3   4   5   6   7   8   9   10  11  12  13
    //  [           ]
    //          [               ]
    //  [       ]
    //                  [       ]
    //                              [   ]   
    //                                  [       ]
    //                                      [   ]
    //                                      [       ]
    //-----------------------------------------------
    //  [                       ]   [               ]

    intervals v{ { 0,3 },{ 2,6 },{ 0,2 },{ 4,6 },{ 7,8 },{ 8,10 },{ 9,10 },{ 9,11 } };
    for_each(v.begin(), v.end(), [](auto& e) {cout << e << " "; });
    cout << endl;
    random_shuffle(v.begin(), v.end());
    for_each(v.begin(), v.end(), [](auto& e) {cout << e << " "; });
    cout << endl;

    intervals m(merge_intervals(v));
    for_each(m.begin(), m.end(), [](auto& e) {cout << e << " "; });
    return 0;
}