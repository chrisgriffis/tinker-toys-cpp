#include <fstream>
#include <sstream>
#include <string>
#include <stack>

int main(int argc, char **argv)
{
    using namespace std;
    string s1,s2;
    s1 = std::string("C:\\Users\\ChrisG\\Dropbox\\_PortlandStateClasses\\CS586 course\\project\\raw data\\" + string(argv[1]) + "_crime_incident_data.txt");
    s2 = std::string("C:\\Users\\ChrisG\\Dropbox\\_PortlandStateClasses\\CS586 course\\project\\raw data\\" + string(argv[1]) + "_crime_incident_data_sql.txt");
    ifstream csvFile(s1.c_str());
    ofstream sqlOutput(s2.c_str());
    std::string line;
    
    while(csvFile.good())
    {
        std::getline(csvFile,line);
        sqlOutput << "(";
        std::istringstream ss(line);
        stringstream outputbuffer("");
        string token;
        int counter(0);
        while(std::getline(ss,token,','))
        {
            if (counter > 0 && counter < 8)
            {
                outputbuffer << "'" << token << "'" << ",";
                token.clear();
            } 
            else
            {
                outputbuffer << token << ",";
                token.clear();
            }
            counter++;
        }
        line.clear();
        sqlOutput << outputbuffer.str().substr(0,outputbuffer.str().size()-1) << "),\n";
    }

    csvFile.close();
    sqlOutput.flush();
    sqlOutput.close();

    ifstream inFile(s2.c_str());
    int filenum(0);
    while (inFile.good())
    {
        stringstream filename("");
        filename << s2 << argv[1] << "." << filenum++ << ".txt";
        ofstream finalOutput(filename.str().c_str());
        string line;
        int counter(0);
        finalOutput << "INSERT into Crime" << argv[1] << "\nvalues\n";
        while(std::getline(inFile,line))
        {
            finalOutput << line << "\n";
            ++counter;
            if(counter == 10000) break;
        }
        finalOutput << ";";
        finalOutput.flush();
        finalOutput.close();
    }
    inFile.close();

    return 0;
}
