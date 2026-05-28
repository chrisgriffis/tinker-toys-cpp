
// Problem:
//     Given a string s and a list of substrings y that need to be bold, return s with bold HTML tags around the substring
//     components provided in y.  if two substring overlap or are contiguous, use only one pair of bold tags.
//
// Example:
//     s = "abcklmnopqrstuvwxyz"
//     y = {
//         "a"
//         "mn"
//         "t"
//         "u"
//         "nop"
//      }
//     Answer = "<b>a</b>bckl<b>mnop</b>qrs<b>tu</b>vwxyz"
//


// S = “ABCD”
// Y = “BC”

#include <iostream>
#include <unordered_set>
#include <set>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <functional>
#include <bitset>

std::string boldify3(std::string s, std::vector<std::string> substrings)
{
    using namespace std;
    string bolded;

    //maps a string to its char count to be used for calc ranges within main string    
    unordered_set<string> fragments;
    set<int, greater<int>> window_sizes; //descending sort, largest first
    for (auto subst : substrings)
    {
        fragments.insert(subst);
        window_sizes.insert(subst.size());
    }
    using interval = std::pair<int, int>;
    vector<bool> bold_this_char(s.size(), false); //set sorts std::pair by first and then by second in a tie; exactly whats needed
    for (int str_idx = 0; str_idx < s.size() - *window_sizes.rbegin(); ++str_idx)
    {
        for (auto window_size : window_sizes)
        {
            if (
                (str_idx + window_size <= s.size()) &&
                fragments.find(s.substr(str_idx, window_size)) != fragments.end())
            {
                //have a match
                auto curr_char = bold_this_char.begin() + str_idx;
                fill(curr_char, curr_char + window_size, true);
                break; //don't need to check smaller windows that will overlap with current window at this index
            }
        }
    }

    //now have this in scope “merged_sorted_intervals;”
    //create final string
    const string bold_start_tag("<b>");
    const string bold_end_tag("</b>");

    enum bold_activation { bold_inactive, bold_active } bold_indicator(bold_inactive);
    size_t string_idx(0);
    auto intvl = bold_this_char.begin();
    while (string_idx < s.size())
    {
        if (intvl == bold_this_char.end())
        {
            bolded += s.substr(string_idx, s.size() - string_idx);
            break;
        }

        else if (bold_indicator == bold_inactive && true == bold_this_char[string_idx])
        {
            bolded += bold_start_tag;
            bold_indicator = bold_active;
        }
        else if (bold_indicator == bold_active && false == bold_this_char[string_idx])
        {
            bolded += bold_end_tag;
            bold_indicator = bold_inactive;
        }

        bolded += s[string_idx++];

    }
    return bolded;
}

int main()
{
    using namespace std;

    string s = "abcklmnopqrstuvwxyz";
    vector<string> y{
        "a",
        "mn",
        "t",
        "u",
        "nop"
    };

    string bolded = boldify3(s, y);
    string expected("<b>a</b>bckl<b>mnop</b>qrs<b>tu</b>vwxyz");
    cout << boolalpha << (bolded == expected) << endl << "bolded\t\t" << bolded << endl << "expected\t" << expected << endl;
    cin >> s;

    return 0;
}
