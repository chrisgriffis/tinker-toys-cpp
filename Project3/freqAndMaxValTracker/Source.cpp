#include <iostream>
#include <queue>
#include <utility>

using namespace std;

struct timer { timer(int x = 0) :x_(x) {} int operator()() { return x_++; } int x_; };
struct tracker
{
    tracker(int w) :window_size(w) {}
    tracker& operator<<(int val)
    {

        return *this;
    }

    int freq()
    {
        int time = timer_();
        while (window.size() > 0 && time - window.front().first > window_size)
        {
            if (max_tracker.size() > 0 && window.front().second == max_tracker.front())
                max_tracker.pop_front();
            window.pop();
        }
        return window.size();
    }
    int add(int val)
    {
        window.emplace(val);
        while (max_tracker.size() > 0 && max_tracker.back() < val)
            max_tracker.pop_back();
        max_tracker.emplace_back(val);        
        if (window.size() > window_size)
        {
            if (max_tracker.size() > 0 && window.front() == max_tracker.front())
                max_tracker.pop_front();
            window.pop();
        }
        if (max_tracker.size() > 0)
            return max_tracker.front();
        else return 0;
    }

    timer& timer_;
    int window_size;
    queue<int> window;
    deque<int> max_tracker;
};


int main()
{
    timer t;
    tracker tr(t, 5);
    tr << 1 << 5 << 5 << 5 << 5 << 5 << 7 << 2 << 5;
    cout << tr.freq() << endl;
    cout << tr.maxv() << endl;
    cout << tr.freq() << endl;
    cout << tr.freq() << endl;
    cout << tr.maxv() << endl;
    cout << tr.maxv() << endl;
    cout << tr.maxv() << endl;
    cout << tr.maxv() << endl;
    tr << 1 << 9 << 1 << 1 << 1 << 1 << 1 << 2 << 1;
    cout << tr.maxv() << endl;
    cout << tr.maxv() << endl;
    cout << tr.maxv() << endl;
    cout << tr.maxv() << endl;
    cout << tr.maxv() << endl;
    cout << tr.maxv() << endl;
    cout << tr.maxv() << endl;
    int sdfgwert;
    cin >> sdfgwert;
    return 0;
}