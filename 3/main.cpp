#include <iostream>

using namespace std;

string cut(string text, char c)
{
    int i {-1}, j{text.size()};
    while(text[++i]==c );
    while(text[--j]==c);
    return text.substr(i,j-i+1);
}

int main()
{
    string napi= "    jak jest cieplo    ";
    cout<<napi<<" | "<<cut(napi,' ')<<"#";
    
    
    return 0;
}