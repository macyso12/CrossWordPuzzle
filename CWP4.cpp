
// Cross World Puzzle
// Macy So

#include <iostream>
#include <string>
#include <vector>
#include <assert.h>
#include <fstream>
#include <unordered_map>

using namespace std;
string word;

string ToUpper (string s){
    string s2 ;
    for (char c : s){
        s2.push_back(toupper(c));
    }
    return s2;
}
//--------------------------------------------------------------------------------------------
// - Point
struct Point {
    Point () {}
    Point (int r, int c): row(r),col(c) {}
    
    friend ostream& operator << (ostream& os, const Point&p);
    
    int row = 0;
    int col = 0;
};

ostream &operator << (ostream& os, const Point &p){
    os << "(" << p.row << "," << p.col << ")" << endl ;
    return os;
}

//--------------------------------------------------------------------------------------------
// -Span
struct Span{
    Span (Point p, int l, bool v): point(p), len(1), vert(v){}
        // definition
        Point GetPoint (int i) const {
            assert(i >=0 && i <len);
            if (vert){
                return Point(point.row+i, point.col);
            }
            else{
                return Point(point.row, point.col+1);
            }
        }
    friend ostream& operator << (ostream& os, const Span&s);
    
    
    Point point;
    int len;
    bool vert ;
};

typedef vector <Span> Spans;
ostream &operator << (ostream& os, const Span &s){
    os << "[" << s.point << "len = " << s.len << "vert = " << s.vert << "]" << endl ;
    return os;
}

//----------------------------------------------------------------------------------------------------------
// - Word
struct Word {
    Word(){}
    Word(string s) : word(s){}
    unsigned long int len() const {
        return word.length();
    }
    string word ;
};
typedef vector <Word*> Words;
typedef unordered_map<string, Words> WordMap;

//const int num_bucket = 1001;
//hash function!!

//----------------------------------------------------------------------------------------------------------
// -Library
class Library {
public:
    Library(){}
    /*~Library(){
        for (const Word* w : words_){
            delete w;
        }
    }*/
    const Words* FindWord(const string& s) const {
        auto it = word_map_.find(s);
        if (it != word_map_.end()){
            return &it->second;
        }
        else {
            return NULL ;
        }
    }
    bool IsWord (string s) const {
        auto it = word_map_.find(s);
        if (it == word_map_.end()){
            return false;
        }
        else {
            return true ;
        }
    }
    void ComputeStats(){
        assert(count_.empty());
        count_.resize(18);
        for (const Word w : words_ ){
            unsigned long int len = w.word.length();
            if (len < 18){
                count_[len] ++ ;
            }
        }
    }
    void PrintStats() const {
        cout << "Here are the counts of each word length" << endl ;
        for (int i = 0; i < count_.size(); i++){
            cout << "[" << i << "]" << count_ [i] << endl ;
        }

    }
    string GetWord (int i) const {
        assert( i >= 0 && i < words_.size());
        return words_[i].word;
    }
    void CreatePatternHash(Word* w){
        unsigned long int len = w->len();
        int num_patterns = pow(2,len);
        
        for (int i = 0; i<num_patterns; i++){
            string temp = w-> word ;
            for (int j = 0; j < len; j++){
                if ((i >> j)&1){
                    temp[j] = '-';
                }
            }
            cout << " " << temp << endl ;
            word_map_[temp].push_back(w);
        }
    }
    void ReadFromFile(string CWP, int max_size)  {
    ifstream f ;
    f.open("CWP");
        while (f.is_open() && !f.eof()){ // while not at the end of file, keep reading
        string line ;
        getline (f,line);

            if (!line.empty()){
                line = ToUpper(line);
                unsigned long int len = line.length();
                if (len > 7){
                    return ;
                }
                if (line[len-1] == '\r'){
                    line = line.substr(0, len-1);
                }
                if (len <= max_size){
                    Word*w = new Word (line);
                    words_.push_back(Word(line));
                    CreatePatternHash(w);
                }
            }
        }
        cout << words_[0].word << endl ;
        cout << words_[0].word.length() << endl ;
        for (char c :words_[0].word){
            int x = c ;
            cout << c << " " << x << endl ;
        }
        cout << "Read " << words_.size() << "word from file"  << CWP << endl ;
    }
    void DebugBuckets() const {
        for (int i =0; i < word_map_.bucket_count(); i++){
            cout << "[" << i << "]" << word_map_.bucket_size(i) << endl ;
        }
    }

private:
    Words words;               // master vector of wrods
    WordMap word_map_;           // pattern hash
    vector <Word> words_ ;
    vector <int> count_ ;
};

// ---------------------------------------------------------------------------------
// - Attr
struct Attr {
    bool is_empty() const {
        return has_blanks && !has_letters;
    }
    bool is_partial() const {
        return has_blanks && has_letters;
    }
    bool is_full() const {
        return !has_blanks && has_letters;
    }
    bool has_letters = false ;
    bool has_blanks = false ;
};

Library lib;
// ---------------------------------------------------------------------------------
// - Grid
struct Grid{
    Grid (string n){
        name = n ;
    }
    unsigned long int rows() const { return lines.size(); }
    unsigned long int cols() const {
        if (lines.empty()){
            return 0;
        } else {
        return lines[0].size();
        }
    }
    unsigned long int max_size() const {
        return max(rows(), cols());
    }
    // returns character value of the box at point 'p'
    // 'p' must be in bound
    char box (const Point& p) const {
        assert (in_bounds(p));
        return lines [p.row][p.col] ;
    }
    
    // Returns true if the point p is a '.'
    // 'p' must be in bounds .
    bool is_block (const Point&p) const {
        return box(p)== '.' ;
        
    }
    
    bool is_letter (const Point& p){
        char c = box (p);
        return c > 'A' && c <= 'Z' ;
    }
    bool is_blank(const Point& p){
        return box(p)== '-' ;
    }
    
    bool in_bounds(const Point& p) const{
        return (p.row >= 0&& p.row <rows() && p.col > 0 && p.col < cols()) ;
    }
    //Next increments the point across the grid, one box at a time
    // returns true if point is still in bound
    
    // Fills in attributes of the string
    string GetString (const Span s, Attr& attr) const {
        int len = s.len;
        string temp ;
        temp.resize(len);
        for (int i = 0; i < len ; i++){
            Point p = s.GetPoint(i) ;
            const char c = box (p) ;
            if (c == '-'){
                attr.has_blanks = true ;
            }
            else if (c == c > 'A' && c <= 'Z'){
                attr.has_letters = true ;
            }
            
            temp[i] = box(p);
            //temp.push_back(box(p));
        }
        
        return temp ;
    }
    
    bool Next(Point &p, bool vert){
        // some code
        if (vert){
            p.row++;
            if (p.row >= rows()){
                p.row = 0;
                p.col ++;
            }
        }
        else {
            p.col++ ;
            if (p.col > cols ()){
                p.col = 0 ;
                p.row ++;
            }
        }
        return in_bounds(p);
    }
    
    // NextStopAtWrap is like "Next" except it return false at every wrap
    //Return true if we stay on the same line
    bool NextStopAtWrap(Point &p, bool vert){
        // some code
        bool wrap = false;
        if (vert){
            p.row++;
            if (p.row >= rows()){
                p.row = 0;
                p.col ++;
                wrap = true ;
            }
        }
        else {
            p.col++ ;
            if (p.col > cols ()){
                p.col = 0 ;
                p.row ++;
                wrap = true;
            }
        }
        return !wrap;
    }
    
    void FillSpans (bool vert){
        Point p;
        while (in_bounds(p)){
            while (in_bounds(p) && is_block(p)){
                Next(p, vert);
            }
            if (!in_bounds(p)) return ;         // FIX
            Point startp = p;
        
        
            int len = 0 ;
            bool keep_going = false ;
            do {
                keep_going = NextStopAtWrap(p, vert);
                len ++;
            }
            while (keep_going && !is_block(p));
            spans.push_back(Span(startp, len, vert));
        }
    }
    
    // Add to spans vector with all viable spans in the grid.
    void FillSpans() {
        assert (spans.empty());
        FillSpans(false);   // horiz
        FillSpans(true);    // vert
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
    void PrintSpans() const{
        cout << "Spans: " << endl ;
        for (const Span& s: spans){
            Attr attr ;
            cout << " " << s << " " << GetString(s, attr) << endl ;
        }
    }
    string name;
    vector <string> lines ;
    Spans spans;
} ;

// ------------------------------------------------------------------------------------------------------------------
// Slot
struct Slot {
    Slot (const Span s, const string& p) : span(s), pattern(p){}
    friend ostream& operator << (ostream& os, const Slot& s);
    
    Span span ;
    string pattern ;
    
};

typedef vector <Slot> Slots ;
ostream& operator << (ostream& os, const Slot& s){
    os << s.span << " '" << s.pattern << "'" ;
    return os;
}

// ------------------------------------------------------------------------------------------------------------------
// -Solver
class Solver{
public:
    Solver(Grid* g) : grid_(g){}
    void Solve(){
        cout << "Solving this grid: " << endl ;
        grid_->Print();
        Loop();
        
    }
private:
    void Loop(){
        Slots empty_slots;
        Slots full_slots;
        Slots partial_slots;
        for (const Span& s: grid_->spans){
            // ...
            Attr attr;
            string temp = grid_-> GetString(s, attr ) ;
            if (attr.is_empty()){
                empty_slots.push_back(Slot(s, temp));
            }
            else if (attr.is_partial()){
                partial_slots.push_back(Slot(s, temp));
            }
            else if (attr.is_full()){
                full_slots.push_back(Slot(s,temp ));
            }
        }
        unsigned long int num_empty = empty_slots.size();
        unsigned long int num_partial = partial_slots.size();
        unsigned long int num_full = full_slots.size();
        
        if (num_partial == 0 && num_empty == 0){
            cout << "Solution!" << endl;
            // FIX what do we do now ?
        }
        assert (num_partial > 0);
        CommitSlot(partial_slots[0]);
        // commit slot, then call loop again
    }
    void CommitSlot (const Slot& slot){
        cout << "Commit slot " << slot << endl ;
        cout << "Possible word choices for this slot are: " << endl ;
        
        const Words* words = lib.FindWord(slot.pattern);
        if (words){
            for (const Word* w: *words){
                cout << " " << w-> word;
            }
        }
        else{
            cout << "No matches to pattern" << endl ;
        }

        
    }
    Grid* grid_;

};

int main() {
    
    Grid grid ("MY GRID") ;
    grid.LoadFromFile("test") ;
    grid.Check();
    grid.Print();
    grid.FillSpans();
    grid.PrintSpans();
    
    
    lib.ReadFromFile("top_12000_txt", grid.max_size());
    lib.FindWord("");
    
    Solver solver(&grid);
    solver.Solve();

    // more code ...
    
    Point p ;
    do {
        cout << p << ":" << grid.is_block(p) << endl ;
    }
    while (grid.Next(p,true));
    
    return 0;
}
