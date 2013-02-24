#include <iostream>
#include <fstream>
#include <exception>
#include <boost/bind.hpp>
#include <boost/unordered_map.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>


using namespace std;
using namespace boost;


typedef  unordered_map<long, long> Histogram;
typedef  vector<string> TitleContainer;

void readInTitles(const string& path, TitleContainer& container)
{
    container.push_back(""); // Base 1, rather than mess w/ offsets just add a dummy value.

    ifstream file;
    file.open(path.c_str());
    string line;
    while(!file.eof())
    {
        getline(file, line);
        trim(line);
        //make sure there's no , in the line.
        erase_all(line, ",");
        container.push_back(line);
    }
    file.close();
}

void readInAndAccumulate(const string& path, Histogram& histogram)
{
    std::string line;

    vector<string> lineParts; 
    vector<string> outboundLinkParts;
    long nodeId;
    vector<long>   outboundLinks;

    outboundLinks.reserve(100);
    lineParts.reserve(100);
    outboundLinkParts.reserve(100);

    ifstream file;
    file.open(path.c_str());
    while (!file.eof())
    {
        // Clear last lines state 
        lineParts.clear(); 
        outboundLinkParts.clear(); 
        outboundLinks.clear();
        line = "";
        nodeId = 0;

        getline(file, line);
        // A line looks like:
        // 0 : 1 2 3 4 5
        split(lineParts, line, boost::is_any_of(":"));

        trim(lineParts[0]);

        if (lineParts.size() != 2 || lineParts.front().empty())
        {
            continue;
        }
        nodeId = lexical_cast<long>(lineParts.front());

//        cout << "node = "  << nodeId <<  endl;
        split(outboundLinkParts, lineParts.back(), boost::is_space());

        // Remove any empty tokens we parsed.
        outboundLinkParts.erase(remove_if(outboundLinkParts.begin(),
                                         outboundLinkParts.end(),
                                         bind(&string::empty,_1)),
                        outboundLinkParts.end());
      //  cout << "num parsed edges " << outboundLinkParts.size() << endl;
        // Parse and store the links of this node.
        BOOST_FOREACH(string & link, outboundLinkParts)
        {
            trim(link);
            outboundLinks.push_back(lexical_cast<long>(link));
  //          cout << outboundLinks.back() <<" ";
        }
        // Push the links into our histogram
        BOOST_FOREACH(long link, outboundLinks) 
        {
            if(histogram.find(link) == histogram.end())
            {
                histogram[link] = 1;
            }
            else 
            {
                ++histogram[link];
            }
        }
    }
    file.close();
}

void printHistogram(Histogram& h, TitleContainer & titles)
{
    long key, value;

    BOOST_FOREACH(boost::tie(key, value), h)
    {
        if (titles.empty())
        {
            cout << key << "," << value << endl;
        }
        else 
        {
            cout << key << "," << titles[key] << "," << value << endl;
        }
    }
}

int main(int argc, char** argv)
{
    Histogram result; 
    TitleContainer titles;

    if (argc != 2 && argc != 3) 
    {
        cerr << "Expected 1 or 2 arguments (link file and title file)" << endl;
        return 1;
    }

    try
    {
        readInAndAccumulate(string(argv[1]), result);
        if (argc == 3)
        {
            readInTitles(argv[2], titles);
        }
        printHistogram(result, titles);
    }
    catch (std::exception & e)
    {
        cerr << "unexpected exception:" << e.what() << endl;
    }
    

    return 0;
}
