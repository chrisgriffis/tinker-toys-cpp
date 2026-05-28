#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>


int main() {
    using namespace std;
    const char* text = 
        "fc85t 346 45 67 3456 235\n"\
        "5gjhg asdf ergfh sdfv tgmhj\n"\
        "64mjb 2345 3468 45678 678\n"\
        "456jhb 3456 6789 34 56235 45678\n"\
        "456hjbbg sd fg ghmj sdf vgf ukj wedrg\n"\
        "j3b54 sd gf hjk sd gf dghj t\n";
    cout << text << "\n";

    stringstream cinss(text);
    string s;
    vector<string> lines;
    while (getline(cinss, s)) lines.push_back(s);

    auto intrangebegin =
        partition(lines.begin(), lines.end(), [](string& s)
    {
        stringstream ss(s);
        string ident;
        int cand;
        ss >> ident; //ident, toss it;
        ss >> cand;
        //if parse fails is not an int whole line is alpha
        //so move when alpha make pred true so lines with nums are
        //pushed to right side of gather.
        return ss.fail();
    });

    //sort the lines with alpha, begin lines range up to itr to int range
    auto pred = [](string l, string r) {
        stringstream lss(l);
        stringstream rss(r);
        string identl, identr, currl, currr;
        lss >> identl;
        rss >> identr;
        while (lss >> currl && rss >> currr)
        {
            if (currl == currr) continue;
            else return currl < currr;
        }
        //all equal, fall back to compare idents
        return identl < identr;
    };

    //sort the lines with ints
    sort(lines.begin(), intrangebegin, pred);
    sort(intrangebegin, lines.end(), pred);

    cout << endl;
    for (string s : lines) cout << s << endl;

    // your code goes here
    return 0;
}

