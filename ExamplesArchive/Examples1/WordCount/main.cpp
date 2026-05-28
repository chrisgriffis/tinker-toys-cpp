#include <string>
#include <set>
#include <map>
#include <fstream>
#include <iostream>
#include <iterator>
#include <utility>

using namespace std;

int main(int ac, char** argv)
{
    if(ac != 2)
    { 
        cout << "Usage: " << argv[0] << " filename" << endl;
        return 1;
    }
    cout << "Reading file " << argv[1] << endl;
    ifstream inputFile(argv[1]);

    // read and count words
    istream_iterator< string > inputStream(inputFile);
    multiset<string> multisetString(inputStream, istream_iterator<string>());

    // sort by count
    multimap<size_t, string> wordstats;
    for(multiset<string>::const_iterator inputStream = multisetString.begin(); inputStream != multisetString.end(); inputStream = multisetString.upper_bound(*inputStream))
        wordstats.insert( make_pair( multisetString.count(*inputStream), *inputStream ));

    // output in decreasing order
    for( multimap<size_t, string>::const_reverse_iterator inputStream = wordstats.rbegin(); inputStream != wordstats.rend(); ++inputStream)
        std::cout << " word " << inputStream->second << " found " << inputStream->first << " times " << endl;
    system("pause");
    return 0;

}




