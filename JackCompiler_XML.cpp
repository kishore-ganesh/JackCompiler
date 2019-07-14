#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <map>
#include <vector>
#include <string.h>
#include <experimental/filesystem>

#define KEYWORD 0
#define SYMBOL 1
#define IDENTIFIER 2
#define INT_CONST 3
#define STRING_CONST 4
#define CLASS 5
#define METHOD 6
#define FUNCTION 7
#define CONSTRUCTOR 8
#define INT 9
#define BOOLEAN 10
#define CHAR 11
#define VOID 12
#define VAR 13
#define STATIC 14
#define FIELD 15
#define LET 17
#define DO 18
#define IF 19
#define ELSE 20
#define WHILE 21
#define RETURN 22
#define TRUE 23
#define FALSE 24
#define NULL 25
#define THIS 26
#define INVALID 27

//While not invalid keep advancing

namespace fs = std::experimental::filesystem;

using namespace std;

struct symbolInformation{
    string type;
    int kind;
    int number;
};

string trim(string input)
{
    int l = 0;
    int r = input.size() - 1;
    while (input[l] == ' ' || input[l] == '\r')
    {
        l++;
    }

    while (input[r] == ' ' || input[r] == '\r')
    {
        r--;
    }

    int length = r - l + 1;

    return input.substr(l, length);
}

string removeComments(string input)
{
    string output = "";
    for (int i = 0; i < input.size() - 1; i++)
    {
        if (input[i] == '/' && input[i + 1] == '*')
        {
            while (!(input[i] == '*' && input[i + 1] == '/'))
            {
                i++;
            }

            i++;
        }

        else if (input[i] == '/' && input[i + 1] == '/')
        {
            while (input[i] != '\n')
            {
                i++;
            }
        }

        else
        {
            output += input[i];
        }
    }

    // cout<<input<<endl;
    cout<<output<<endl;

    output += input[input.size() - 1];

    return output;
}

class FileWriter
{

    ofstream outstream;

  public:
    FileWriter()
    {
    }
    FileWriter(const char *path)
    {
        outstream.open(path);
    }
    void writeLine(string line)
    {
        outstream << line;
        outstream << "\n";
    }
};

class JackTokenizer
{
    string currentToken;
    string prevToken = "";
    string nextToken;
    string fileBuffer = "";
    int currentTokenType;
    int firstFree;
    int index = 0;
    int prevIndex = 0;

  public:
    JackTokenizer(const char *path)
    {
        readFileIntoBuffer(path);
        fileBuffer = removeComments(fileBuffer);
    }

    JackTokenizer()
    {
    }

    void readFileIntoBuffer(const char *path)
    {
        ifstream instream;
        instream.open(path);
        while (!instream.eof())
        {
            string line;
            getline(instream, line);
            line = trim(line);
            line += '\n';
            // cout<<line<<endl;
            fileBuffer += line;
        }
    }

    bool hasMoreTokens()
    {
        return index < fileBuffer.size();
    }

    bool isSymbol(char c)
    {
        if (c == '{' || c == '}' || c == '(' || c == ')' || c == '[' || c == ']' || c == '.' || c == ','

            || c == '.' || c == ';' || c == '+' || c == '-' || c == '*' || c == '/' || c == '&' || c == '|' || c == '<' || c == '>'

            || c == '=' || c == '_' || c == '~')
        {
            return true;
        }

        return false;
    }

    bool isOperand()
    {
        if (currentToken.size() == 1 && tokenType() == SYMBOL)
        {
            char c = currentToken[0];
            switch (c)
            {
            case '+':
                return true;
            case '-':
                return true;
            case '*':
                return true;
            case '/':
                return true;
            case '&':
                return true;
            case '|':
                return true;
            case '<':
                return true;
            case '>':
                return true;
            case '=':
                return true;
            }
        }

        return false;
    }

    void advanceTillValid()
    {
        if(nextToken==""){
            prevIndex = index;
        
        }
        else{
            prevIndex = index-1;
        }

    prevToken = currentToken;        
        cout<<prevIndex<<" ";
        do
        {
            advance();
        } while (hasMoreTokens()&&tokenType() == INVALID);

        cout<<index<<endl;
    }
    bool isSymbol(string token)
    {
        if (token.size() == 1)
        {
            char c = token[0];

            if (c == '{' || c == '}' || c == '(' || c == ')' || c == '[' || c == ']' || c == '.' || c == ','

                || c == '.' || c == ';' || c == '+' || c == '-' || c == '*' || c == '/' || c == '&' || c == '|' || c == '<' || c == '>'

                || c == '=' || c == '_' || c == '~')
            {
                return true;
            }

            return false;
        }

        return false;
    }

    char getNextCharacter()
    {
        char c = fileBuffer[index];
        index++;
        return c;
    }

    bool rollBack()
    {
        index = prevIndex;
        currentToken = prevToken;
        nextToken = "";
    }

    void advance()
    {
        // prevIndex = index;
        // prevToken = currentToken;
        getNextToken();
    }

    void getNextToken()
    {
        currentToken = "";
        if (nextToken != "")
        {
            currentToken = nextToken;
            nextToken = "";
        }

        else
        {
            char c = getNextCharacter();
            if (c == '"')
            {
                currentToken += c;
                do
                {
                    c = getNextCharacter();
                    currentToken += c;
                } while (c != '"');
            }

            else if ((c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z'))
            {
                while (c != ' ' && (!isSymbol(c) || c == '_'))
                {
                    currentToken += c;
                    c = getNextCharacter();
                }

                if (isSymbol(c))
                {
                    nextToken = c;
                }
            }

            else if (c >= '0' && c <= '9')
            {
                while (c >= '0' && c <= '9')
                {
                    currentToken += c;
                    c = getNextCharacter();
                }

                if (isSymbol(c))
                {
                    nextToken = c;
                }
            }

            // //Pipe outputs

            else if (isSymbol(c))
            {
                currentToken = c;
            }

            else if (c == ' ' || c == '\r' || c == '\n')
            {
                if (hasMoreTokens())
                {
                    getNextToken();
                }
            }
        }
    }

    //preprocess?

    bool isNumber(string token)
    {

        if (token.size() == 0)
        {
            return false;
        }
        for (int i = 0; i < token.size(); i++)
        {
            if (!(token[i] >= '0' && token[i] <= '9'))
            {
                return false;
            }
        }

        ;

        return true;
    }

    bool isKeyword(string token)
    {

        vector<string> keywords{"class",
                                "constructor",
                                "function",
                                "method",
                                "field",
                                "static",
                                "var",
                                "int",
                                "char",
                                "boolean",
                                "void",
                                "true",
                                "false",
                                "null",
                                "this",
                                "let",
                                "do",
                                "if",
                                "else",
                                "while",
                                "return"};

        for (int i = 0; i < keywords.size(); i++)
        {
            if (token == keywords[i])
            {
                return true;
            }
        }

        return false;
    }

    int tokenType()
    {
        if (currentToken.find('"') != -1)
        {
            return STRING_CONST;
        }

        if (isKeyword(currentToken))
        {
            return KEYWORD;
        }

        if (isNumber(currentToken))
        {
            return INT_CONST;
        }

        if (isSymbol(currentToken))
        {
            return SYMBOL;
        }

        if (currentToken.size() == 0 || currentToken.find("\n") != -1 || currentToken.find("\r") != -1 || currentToken.find(" ") != -1)
        {

            return INVALID;
        }

        return IDENTIFIER;
    }

    //Use hashmap instead
    int keyWord()
    {
        string token = currentToken;
        if (token == "class")
        {
            return CLASS;
        }

        if (token == "method")
        {
            return METHOD;
        }

        if (token == "function")
        {
            return FUNCTION;
        }

        if (token == "constructor")
        {
            return CONSTRUCTOR;
        }

        if (token == "int")
        {
            return INT;
        }

        if (token == "boolean")
        {
            return BOOLEAN;
        }

        if (token == "char")
        {
            return CHAR;
        }

        if (token == "void")
        {
            return VOID;
        }

        if (token == "var")
        {
            return VAR;
        }

        if (token == "static")
        {
            return STATIC;
        }

        if (token == "field")
        {
            return FIELD;
        }

        if (token == "let")
        {
            return LET;
        }

        if (token == "do")
        {
            return DO;
        }

        if (token == "if")
        {
            return IF;
        }

        if (token == "else")
        {
            return ELSE;
        }

        if (token == "while")
        {
            return WHILE;
        }

        if (token == "return")
        {
            return RETURN;
        }

        if (token == "true")
        {
            return TRUE;
        }

        if (token == "false")
        {
            return FALSE;
        }

        if (token == "null")
        {
            return NULL;
        }

        if (token == "this")
        {
            return THIS;
        }
    }

    char symbol()
    {
        return currentToken[0];
    }

    string identifier()
    {
        return currentToken;
    }

    int intVal()
    {
        return stoi(currentToken);
    }

    string stringVal()
    {
        string value = currentToken.substr(1, currentToken.size() - 2);
        return value;
    }

    string getIdentifierXML()
    {

        return "<identifier> " + currentToken + " </identifier>";
    }

    string getSymbolXML()
    {

        string output = "";

        if (symbol() == '"')
        {
            output = "&quot;";
        }
        else if (currentToken == "<")
        {
            output = "&lt;";
        }

        else if (currentToken == ">")
        {
            output = "&gt;";
        }

        else if (currentToken == "&")
        {
            output = "&amp;";
        }

        else
        {
            output = currentToken;
        }

        return "<symbol>" + output + "</symbol>";
    }

    string getStringConstantXML()
    {
        string output = stringVal();
        return "<stringConstant> " + output + " </stringConstant>";
    }

    string getIntegerConstantXML()
    {
        return "<integerConstant> " + currentToken + " </integerConstant>";
    }

    string getKeywordXML()
    {
        return "<keyword> " + currentToken + " </keyword>";
    }

    string getXML()
    {

        switch (tokenType())
        {
        case KEYWORD:
            return getKeywordXML();
        case IDENTIFIER:
            return getIdentifierXML();
        case SYMBOL:
            return getSymbolXML();
        case STRING_CONST:
            return getStringConstantXML();
        case INT_CONST:
            return getIntegerConstantXML();
        }
    }


    void test(const char *path)
    {
        FileWriter writer(path);
        writer.writeLine("<tokens>");
        while (hasMoreTokens())
        {
            advanceTillValid();
            switch (tokenType())
            {

            case SYMBOL:
            {

                string toWrite = "";

                if (symbol() == '"')
                {
                    toWrite = "&quot;";
                }
                else if (currentToken == "<")
                {
                    toWrite = "&lt;";
                }

                else if (currentToken == ">")
                {
                    toWrite = "&gt;";
                }

                else if (currentToken == "&")
                {
                    toWrite = "&amp;";
                }

                else
                {
                    toWrite = currentToken;
                }

                writer.writeLine("<symbol> " + toWrite + " </symbol>");
                break;
            }

            case KEYWORD:
            {
                writer.writeLine("<keyword> " + currentToken + " </keyword>");
                break;
            }

            case IDENTIFIER:
            {
                writer.writeLine("<identifier> " + currentToken + " </identifier>");
                break;
            }

            case STRING_CONST:
            {
                string toWrite = stringVal();
                writer.writeLine("<stringConstant> " + toWrite + " </stringConstant>");
                break;
            }

            case INT_CONST:
            {
                writer.writeLine("<integerConstant> " + currentToken + " </integerConstant>");
                break;
            }
            }
        }
        writer.writeLine("</tokens>");
    }
};

class CompilationEngine
{
    JackTokenizer tokenizer;
    const char *outputPath;
    FileWriter writer;

  public:
    CompilationEngine(JackTokenizer tokenizer, const char *outputPath)
    {
        this->tokenizer = tokenizer;
        this->outputPath = outputPath;
        this->writer = FileWriter(outputPath);
        tokenizer.advanceTillValid();
        compileClass();
    }

    //Should  it go to end

    void compileClass()
    {
        writer.writeLine("<class>");

        //write also the keyword dclaration here

        while (tokenizer.hasMoreTokens())
        {
            tokenizer.advanceTillValid();

            if (tokenizer.tokenType() == IDENTIFIER || tokenizer.tokenType() == SYMBOL)
            {
                writer.writeLine(tokenizer.getXML());
            }

            if (tokenizer.tokenType() == KEYWORD)
            {
                //class constructor function method  field static v ar int char boolean void true
                //fakse null this let do if else while return
                // cout << tokenizer.keyWord();
                int keyword = tokenizer.keyWord();
                if (keyword == FUNCTION || keyword == CONSTRUCTOR || keyword == METHOD)
                {
                    compileSubroutine();
                }

                else if (keyword == STATIC || keyword == FIELD)
                {
                    compileClassVarDec();
                }

                else
                {
                    writer.writeLine(tokenizer.getXML());
                }
            }
        }

        writer.writeLine("</class>");
    }

    //Make this correct to excist always

    void compileClassVarDec()
    {
        writer.writeLine("<classVarDec>");
        writer.writeLine(tokenizer.getXML());
        writeTillEndOfLine();
        writer.writeLine("</classVarDec>");
    }

    void compileParameterList()
    {
        writer.writeLine("<parameterList>");
        while (!(tokenizer.tokenType() == SYMBOL && tokenizer.symbol() == ')'))
        {
            tokenizer.advanceTillValid();
            if (tokenizer.tokenType() == KEYWORD || tokenizer.tokenType() == IDENTIFIER)
            {
                writer.writeLine(tokenizer.getXML());
            }

            if (tokenizer.tokenType() == SYMBOL && tokenizer.symbol() != ')')
            {
                writer.writeLine(tokenizer.getXML());
            }
        }
        writer.writeLine("</parameterList>");
        writer.writeLine(tokenizer.getXML());
    }

    void compileStatements()
    {

        writer.writeLine("<statements>");
        while (!(tokenizer.tokenType() == SYMBOL && tokenizer.symbol() == '}'))
        {

            if (tokenizer.tokenType() == KEYWORD)
            {
                switch (tokenizer.keyWord())
                {

                //Mak this for both if and else
                case IF:
                    compileIf();
                    break;
                case WHILE:
                    compileWhile();
                    break;
                case DO:
                    compileDo();
                    break;
                case LET:
                    compileLet();
                    break;
                case RETURN:
                    compileReturn();
                    break;
                }
            }

            tokenizer.advanceTillValid();
        }
        writer.writeLine("</statements>");
        writer.writeLine(tokenizer.getXML());
    }

    void compileDo()
    {
        writer.writeLine("<doStatement>");
        writer.writeLine(tokenizer.getXML());
        while (!(tokenizer.tokenType() == SYMBOL && tokenizer.symbol() == ';'))
        {
            tokenizer.advanceTillValid();

            if (tokenizer.tokenType() == IDENTIFIER || tokenizer.tokenType() == SYMBOL || tokenizer.tokenType() == KEYWORD)
            {
                writer.writeLine(tokenizer.getXML());
            }

            if (tokenizer.tokenType() == SYMBOL && tokenizer.symbol() == '(')
            {
                // writer.writeLine(tokenizer.getXML());
                compileExpressionList();
            }
        }
        writer.writeLine("</doStatement>");
    }

    void compileLet()
    {
        writer.writeLine("<letStatement>");
        writer.writeLine(tokenizer.getXML());
        while (!(tokenizer.tokenType() == SYMBOL && tokenizer.symbol() == ';'))
        {

            tokenizer.advanceTillValid();
            if (tokenizer.tokenType() == SYMBOL || tokenizer.tokenType() == IDENTIFIER)
            {
                writer.writeLine(tokenizer.getXML());
            }

            if (tokenizer.tokenType() == SYMBOL)
            {
                if (tokenizer.symbol() == '[' || tokenizer.symbol() == '=')
                {
                    compileExpression();
                    writer.writeLine(tokenizer.getXML());
                    // tokenizer.rollBack();
                }
            }
        }

        writer.writeLine("</letStatement>");
    }

    void compileWhile()
    {
        writer.writeLine("<whileStatement>");
        writer.writeLine(tokenizer.getXML());
        while (!(tokenizer.tokenType() == SYMBOL && tokenizer.symbol() == '}'))
        {
            tokenizer.advanceTillValid();
            if (tokenizer.tokenType() == SYMBOL)
            {
                writer.writeLine(tokenizer.getXML()); //make this ebtter
                if (tokenizer.symbol() == '(')
                {
                    compileExpression();
                    writer.writeLine(tokenizer.getXML());
                }

                if (tokenizer.symbol() == '{')
                {
                    compileStatements();
                }
            }
        }
        writer.writeLine("</whileStatement>");
    }

    //Make get XML better wiht an interface
    void compileReturn()
    {
        writer.writeLine("<returnStatement>");
        writer.writeLine(tokenizer.getXML());

        while (!(tokenizer.tokenType() == SYMBOL && tokenizer.symbol() == ';'))
        {
            tokenizer.advanceTillValid();
            if (!(tokenizer.tokenType() == SYMBOL && tokenizer.symbol() == ';'))
            {
                tokenizer.rollBack();
                compileExpression();
            }
        }

        writer.writeLine(tokenizer.getXML());
        writer.writeLine("</returnStatement>");
    }

    //problematic

    void compileIf()
    {
        writer.writeLine("<ifStatement>");
        writer.writeLine(tokenizer.getXML());
        if (tokenizer.keyWord() == IF)
        {
            while (!(tokenizer.tokenType() == SYMBOL && tokenizer.symbol() == '}'))
            {
                tokenizer.advanceTillValid();
                if (tokenizer.tokenType() == SYMBOL)
                {
                    writer.writeLine(tokenizer.getXML());
                    if (tokenizer.symbol() == '(')
                    {
                        compileExpression();
                        writer.writeLine(tokenizer.getXML());
                    }

                    if (tokenizer.symbol() == '{')
                    {
                        // tokenizer.advance();
                        compileStatements();
                    }
                }
            }
            //Loop
        }

        tokenizer.advanceTillValid();
        if (tokenizer.keyWord() == ELSE)
        {
            writer.writeLine(tokenizer.getXML());
            while (!(tokenizer.tokenType() == SYMBOL && tokenizer.symbol() == '}'))
            {
                tokenizer.advanceTillValid();
                if (tokenizer.tokenType() == SYMBOL)
                {
                    writer.writeLine(tokenizer.getXML());
                    if (tokenizer.symbol() == '{')
                    {
                        compileStatements();
                    }
                }
            }
        }

        else
        {
            tokenizer.rollBack();
        }
        writer.writeLine("</ifStatement>");
    }

    void compileExpression()
    {
        writer.writeLine("<expression>");

        do
        {

            tokenizer.advanceTillValid();
            compileTerm();
            tokenizer.advanceTillValid();

            if (tokenizer.tokenType() == SYMBOL && tokenizer.isOperand())
            {
                writer.writeLine(tokenizer.getXML());
            }

        } while (tokenizer.tokenType() == SYMBOL && tokenizer.isOperand());

        writer.writeLine("</expression>");
        // writer.writeLine(tokenizer.getXML());

        //Ends at ) or , or ; or ]
        //Make it exit focused
    }

    void compileTerm()
    {

        writer.writeLine("<term>");
        // tokenizer.advanceTillValid();

        writer.writeLine(tokenizer.getXML());
        if (tokenizer.tokenType() == IDENTIFIER)
        {
            tokenizer.advanceTillValid();

            if (tokenizer.tokenType()==SYMBOL)
            {
                switch (tokenizer.symbol())
                {
                case '.':
                {
                    writer.writeLine(tokenizer.getXML());
                    tokenizer.advanceTillValid();
                    writer.writeLine(tokenizer.getXML());
                    tokenizer.advanceTillValid();
                    writer.writeLine(tokenizer.getXML());
                    compileExpressionList();
                    break;
                }
                case '(':
                {
                    writer.writeLine(tokenizer.getXML());
                    compileExpressionList();
                    break;
                }
                case '[':
                {
                    writer.writeLine(tokenizer.getXML());
                    compileExpression();
                    writer.writeLine(tokenizer.getXML());
                    break;
                }

                default:
                {
                    tokenizer.rollBack();
                }
                }
            }

            else
            {
                tokenizer.rollBack();
            }
        }

        else if (tokenizer.tokenType() == SYMBOL && (tokenizer.symbol() == '-' || tokenizer.symbol() == '~'))
        {
            tokenizer.advanceTillValid();
            compileTerm();
        }

        else if(tokenizer.tokenType()==SYMBOL&&tokenizer.symbol()=='('){
            compileExpression();
            writer.writeLine(tokenizer.getXML());
        }

        // if()
        // if(tokeni)

        writer.writeLine("</term>");
    }

    //Figure out a better way for the return, and for all  other statements
    void compileExpressionList()
    {
        writer.writeLine("<expressionList>");

        do
        {
            tokenizer.advanceTillValid();
             if(tokenizer.tokenType()!=SYMBOL||tokenizer.symbol()=='(')
            {
                tokenizer.rollBack();
                compileExpression();
                if (tokenizer.tokenType() == SYMBOL && tokenizer.symbol() == ',')
                {
                    writer.writeLine(tokenizer.getXML());
                }
            }
            else if (tokenizer.tokenType() == SYMBOL)
            {
                if (tokenizer.symbol() != ')')
                {
                    writer.writeLine(tokenizer.getXML());
                }
            }

           
        } while (tokenizer.tokenType() == SYMBOL && tokenizer.symbol() == ',');

        writer.writeLine("</expressionList>");
        if (tokenizer.tokenType() == SYMBOL)
        {
            writer.writeLine(tokenizer.getXML());
        }
    }

    void writeTillEndOfLine()
    {
        while (!(tokenizer.tokenType() == SYMBOL && tokenizer.symbol() == ';'))
        {
            tokenizer.advanceTillValid();
            if (tokenizer.tokenType() == KEYWORD || tokenizer.tokenType() == SYMBOL || tokenizer.tokenType() == IDENTIFIER)
            {
                writer.writeLine(tokenizer.getXML());
            }
        }
    }

    void compileVarDec()
    {
        writer.writeLine("<varDec>");
        writer.writeLine(tokenizer.getXML());
        // tokenizer.advance();
        if (tokenizer.tokenType() == KEYWORD && tokenizer.keyWord() == VAR)
        {
            writeTillEndOfLine();
        }
        writer.writeLine("</varDec>");
    }

    void compileSubroutine()
    {
        //write function liune
        string key = "";
        writer.writeLine("<subroutineDec>");
        writer.writeLine(tokenizer.getXML());
        // cout<<"Y"<<endl;
        while (!(tokenizer.tokenType() == SYMBOL && tokenizer.symbol() == '{'))
        {
            tokenizer.advanceTillValid();
            if (tokenizer.tokenType() == KEYWORD)
            {
                writer.writeLine(tokenizer.getXML());
            }

            if (tokenizer.tokenType() == IDENTIFIER)
            {
                writer.writeLine(tokenizer.getXML());
            }

            if (tokenizer.tokenType() == SYMBOL)
            {
                if (tokenizer.symbol() == '{')
                {
                    writer.writeLine("<subroutineBody>");
                }
                writer.writeLine(tokenizer.getXML());
                if (tokenizer.symbol() == '(')
                {
                    compileParameterList();
                }
            }
        }

        // cout<<tokenizer.tokenType()<<endl;
        while (!(tokenizer.tokenType() == SYMBOL && tokenizer.symbol() == '}'))
        {
            tokenizer.advanceTillValid();
            // cout << tokenizer.keyWord() << endl;
            // cout<<tokenizer.keyWord()<<endl;
            if (tokenizer.tokenType() == KEYWORD)
            {
                if (tokenizer.keyWord() == VAR)
                {
                    compileVarDec();
                }

                // vector<int> statementKeywords{LET, IF, WHILE, DO, RETURN};
                else
                {

                    compileStatements();
                }
            }

            else if (tokenizer.tokenType() != INVALID)
            {
                compileStatements();
            }
        }

        writer.writeLine("</subroutineBody>");

        writer.writeLine("</subroutineDec>");
    }
};

class SymbolTable{

    map<string, symbolInformation> classScope;
    map<string, symbolInformation> subroutineScope;


    void startSubroutine(){
        subroutineScope.erase(subroutineScope.begin(), subroutineScope.end());

    }

    void define(string name, string type, int kind)
    {

    }

    int varCount(int kind)
    {

    }

    string typeOf(string name)
    {

    }

    string indexOf(string name)
    {

    }




};
/*For each construct, we have a particular way of wiritng it
For instance, a function consists of the function keyword
*/

class JackAnalyzer
{
  public:
    const char *filepath;
    // JackTokenizer tokenizer;
    JackAnalyzer(const char *path)
    {

        filepath = path;
    }

    void beginAnalyzing()
    {

        JackTokenizer tokenizer(filepath);
        string outputPath = filepath;
        outputPath = outputPath.substr(0, outputPath.size() - 4) + "TT.xml";
        CompilationEngine engine(tokenizer, outputPath.c_str());

        // tokenizer.test(outputPath.c_str());
    }
};

int main(int argc, char *argv[])

{
    //Interpret as colleciton
    //Each time you start traversing a file, inform codewriter class
    //Can function be called across files: Yes, functions are global in scope.
    char *inputPath = argv[1];
    char *outputPath = argv[2];

    int inputPathLength = strlen(inputPath);
    string extension = fs::path(inputPath).extension();
    vector<string> files;

    if (extension != ".jack")
    {
        for (auto &i : fs::directory_iterator(inputPath))
        {
            string path = i.path();
            if (i.path().extension() == ".jack")
            {
                files.push_back(path);
            }
        }
    }

    else
    {
        files.push_back(inputPath);
    }

    for (int i = 0; i < files.size(); i++)
    {

        JackAnalyzer analyzer(files[i].c_str());
        analyzer.beginAnalyzing();
    }
}
