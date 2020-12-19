// Cross World Puzzle
// Macy So

#include <iostream>
#include <string>
#include <vector>
#include <assert.h>
#include <fstream>

using namespace std;
string word;

string ToUpper (string s){
    string s2 ;
    for (char c : s){
        s2.push_back(toupper(c));
    }
    return s2;
}
struct Word {
    Word(string s){
        word = s ;
    }
    string word;
};
typedef vector <Word> Words;

const int num_bucket = 1001;
//hash function!!
int bucket(string s){
    assert(!s.empty());
    int i = 0 ;
    for (char c : s){
        i = (i * 217) + c ;
    }
    i = abs(i);
    int b = i % num_bucket ;
    assert(b >= 0 && b < num_bucket /*shelves.size*/) ;
    return b;
}

class Library {
public:
    Library(){
        shelves.resize(num_bucket);
    }
    bool IsWord (string s) const {
        for (Word w : shelves[bucket(s)]){
            if (s == w.word){
                return true;
            }
        }
        return false ;
    }
    void ComputeStats(){
        assert(count.empty());
        count.resize(18);
        for (Word w : words ){
            int len = w.word.length();
            if (len < 18){
                count[len] ++ ;
            }
        }
    }
    void PrintStats() const {
        cout << "Here are the counts of each word length" << endl ;
        for (int i = 0; i < count.size(); i++){
            cout << "[" << i << "]" << count [i] << endl ;
        }

    }
    string GetWord (int i) const {
        assert( i >= 0 && i < words.size());
        return words[i].word;
    }
    void ReadFromFile(string filename)  {
    ifstream f ;
    f.open("CWP");
    while (!f.eof()){ // while not at the end of file, keep reading
        string line ;
        getline (f,line);
        cout << line <<"(" << line.length() << ")" ;
        
            if (!line.empty()){
                line = ToUpper(line);
                int len = line.length();
                if (line[len-1] == '\r'){
                    line = line.substr(0, len-1);
                }
                shelves[bucket(line)].push_back(Word(line));
            }
        }
        cout << words[0] << endl ;
        cout << words[0].length() << endl ;
        for (char c :words[0]){
            int x = c ;
            cout << c << " " << x << endl ;
        }
        cout << "Read " << words.size() << "word from file"  << filename << endl ;
    }
    void DebugBuckets() const {
        for (int i =0; i < shelves.size(); i++){
            cout << "[" << i << "]" << shelves[i].size() << endl ;
        }
    }
private:
    //Words words;
    vector<Words> shelves;
    vector <string> words ;
    vector <int> count ;
};

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
    
    Library lib;
    lib.ReadFromFile("top_12000_txt");
    //lib.ComputeStats();
    //lib.PrintStats();
    lib.DebugBuckets();

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
