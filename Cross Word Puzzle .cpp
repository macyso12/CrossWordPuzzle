// Cross World Puzzle
// Macy So

#include <iostream>
#include <string>
#include <vector>
#include <assert.h>
#include <fstream>

using namespace std;

struct Grid{
    Grid (string n){
        name = n ;
    }
    int rows() const { return lines.size(); }
    int cols() const {
        if (lines.empty()){
            return 0;
        } else {
        return lines[0].size();
        }
    }
    void LoadFromFile(string CWP)  {
        ifstream f ;
        f.open("CWP");
        while (!f.eof()){ // while not at the end of file, keep reading
            string line ;
            getline (f,line);
            cout << line <<"(" << line.length() << ")" ;
            
            if (!line.empty() && line[0]!= '#'){
                lines.push_back(line);
            }
        }
        /*lines.push_back("DOG....");
        lines.push_back("---....");
        lines.push_back("----...");
        lines.push_back("-------");
        lines.push_back("...----");
        lines.push_back("....---");
        lines.push_back("....CAT");*/
    }
    void Check() const {
        for (int i = 0; i < lines.size(); i++){
            assert(lines[i].size() == cols());
        }
    }
    void Print() const {
        cout << "Grid: " << name << " (rows = " << rows() << ", cols = " << cols() << ")"<< endl ;
        
        for (int i = 0; i < lines.size(); i++){
            cout << lines[i] << endl ;
        }
    }
    string name;
    vector <string> lines ;
} ;

int main() {
    
    Grid grid ("MY GRID") ;
    
    grid.LoadFromFile("test") ;
    grid.Check();
    grid.Print();
    
    //cout << "Empty is s0 is: " << s0.empty() << endl;
    
    // s0.append("EXTRA!"); // Add extra code at the end ;
    // cout << "s0 is now:" << s0 << endl ;
    
    //.find();
    //cout << "The G in s0 is at position: " << s0.find ("G") << endl ;
    
    // cout << "Character at pos 2 is " << s0[2] << endl ;
    
    return 0;
}
