#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
using namespace std;

struct MeetingInfo
{
    MeetingInfo(int s = 0, int e = 0) :startTime(s), endTime(e) {}
    int startTime;
    int endTime;
};

int GetNumRoomsRequired(list<MeetingInfo> meetings)
{
    using namespace std;
    vector<int> startTimes, endTimes;
    for (auto m : meetings)
    {
        startTimes.push_back(m.startTime);
        endTimes.push_back(m.endTime);
    }
    sort(startTimes.begin(), startTimes.end());
    sort(endTimes.begin(), endTimes.end());
    int maxoverlap(0), currentOverlap(0);
    auto endTime(endTimes.begin());
    for(auto startTime : startTimes )
    {
        if (startTime >= *endTime)
        {
            --currentOverlap;
            ++endTime;
        }
        else 
        {
            ++currentOverlap;
        }
        maxoverlap = max(maxoverlap, currentOverlap);
    }
    return maxoverlap;
}

int main1() {
    // your code goes here
    using namespace std;
    list<MeetingInfo> meetings;
//     meetings.push_back(MeetingInfo(1, 7));
//     meetings.push_back(MeetingInfo(8,10));
    meetings.push_back(MeetingInfo(2, 8));
    meetings.push_back(MeetingInfo(8, 18));
    meetings.push_back(MeetingInfo(18, 80));
//     meetings.push_back(MeetingInfo(3, 9));
//     meetings.push_back(MeetingInfo(5, 10));
//     meetings.push_back(MeetingInfo(8, 10));
//     meetings.push_back(MeetingInfo(5, 7));
    cout << GetNumRoomsRequired(meetings);
    return 0;
}