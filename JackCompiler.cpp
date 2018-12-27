#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <map>
#include <vector>
#include <string.h>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

using namespace std;

string trim(string input)
{
    int l = 0;
    int r = input.size() - 1;
    while (input[l] == ' ' || input[l] == '\r' || input[l] == '/')
    {
        l++;
    }

    while (input[r] == ' ' || input[r] == '\r' || input[r] == '/')
    {
        r--;
    }

    int length = r - l + 1;

    return input.substr(l, length);
}

string removeComment(string input)
{
    string output = input;
    while (output.find("//") != -1)
    {
        output = output.substr(0, output.size() - 1);
    }

    return output;
}

class FileWriter{

    ofstream outstream;
    FileWriter(const char* path)
    {
         outstream.open(path);
    }
    void writeLine(string line)
    {
        outstream << line;
        outstream << "\n";
    }
};



class JackAnalyzer{
    JackAnalyzer(const char* path){
        JackTokenizer tokenizer(path);
        
        
    }
};



class JackTokenizer
{
    string currentCommand;
    int currentCommandType;
    int firstFree;
    ifstream instream;

  public:
    JackTokenizer(const char *path)
    {
        instream.open(path);
        firstFree = 16;
    }

    bool hasMoreCommands()
    {
        return !instream.eof();
    }

    void advance()
    {
        getline(instream, currentCommand);
        currentCommand = removeComment(currentCommand);
        currentCommand = trim(currentCommand);
        cout << "C" << currentCommand << endl;
    }


    vector<string> tokenize(string input, char delim)
    {
        vector<string> output;
        int k = 0;
        for (int i = 0; i < input.size(); i++)
        {
            if (input[i] == delim)
            {
                string token = input.substr(k, i - k);
                if (token.find(delim) == -1)
                {
                    output.push_back(token);
                }
                k = i + 1;
            }
        }

        if (k < input.size())
        {
            output.push_back(input.substr(k));
        }

        return output;
    }

};