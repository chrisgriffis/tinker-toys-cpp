
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

std::string boldify2(std::string s, std::vector<std::string> substrings)
{
    using namespace std;
    string bolded;

    //maps a string to its char count to be used for calc ranges within main string    
    unordered_set<string> fragments;
    set<int,greater<int>> window_sizes; //descending sort, largest first
    for (auto subst : substrings)
    {
        fragments.insert(subst);
        window_sizes.insert(subst.size());
    }
    using interval = std::pair<int, int>;
    set<interval>  intervals; //set sorts std::pair by first and then by second in a tie; exactly whats needed
    for (int str_idx = 0; str_idx < s.size() - *window_sizes.rbegin(); ++str_idx) 
    {
        for (auto window_size : window_sizes)
        {
            if (
                (str_idx + window_size <= s.size()) && 
                fragments.find(s.substr(str_idx, window_size)) != fragments.end())
            {
                //have a match
                intervals.insert(interval{ str_idx,str_idx + window_size });
                break; //don't need to check smaller windows that will overlap with current window at this index
            }
        }
    }

    //merge intervals
    vector<interval> merged_sorted_intervals;
    if (intervals.size() > 0)
    {
        auto cand_intvl_start(intervals.begin()->first);
        auto cand_intvl_end(intervals.begin()->second);
        for (auto curr_intvl = next(intervals.begin()); curr_intvl != intervals.end(); ++curr_intvl)
        {
            //if interval overlap, as in its beginning is before candidate's end
            if (cand_intvl_end >= curr_intvl->first)
            {
                //update the range and move on
                cand_intvl_end = max(cand_intvl_end, curr_intvl->second);
            }
            else //otherwise, non-overlapping interval, so save candidate and start over
            {
                merged_sorted_intervals.emplace_back(interval{ cand_intvl_start ,cand_intvl_end });
                cand_intvl_start = curr_intvl->first;
                cand_intvl_end = curr_intvl->second;
            }
        }
        merged_sorted_intervals.emplace_back(interval{ cand_intvl_start ,cand_intvl_end });
    }

    //now have this in scope “merged_sorted_intervals;”
    //create final string
    const string start_bold("<b>");
    const string end_bold("</b>");

    size_t string_idx(0);
    auto intvl = merged_sorted_intervals.begin();
    bool seeking_intvl_start(true);
    while (string_idx < s.size())
    {
        if (intvl == merged_sorted_intervals.end())
        {
            bolded += s.substr(string_idx, s.size() - string_idx);
            break;
        }
        else if (seeking_intvl_start && string_idx == intvl->first)
        {
            bolded += start_bold;
            seeking_intvl_start = !seeking_intvl_start;
        }
        else if (string_idx == intvl->second)
        {
            bolded += end_bold;
            seeking_intvl_start = !seeking_intvl_start;
            ++intvl;
        }
        else bolded += s[string_idx++];

    }
    return bolded;
}

int mainb()
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

    string bolded = boldify2(s, y);
    cout << boolalpha << (bolded == string("<b>a</b>bckl<b>mnop</b>qrs<b>tu</b>vwxyz")) << ": " << bolded << endl;
    return 0;
}
