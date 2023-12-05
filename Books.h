#include <string>
#include <vector>
using namespace std;

class GutenbergBook{
    int id;
    vector<string> genres;
    vector<string> authors;
    string language;
    string title;
public:
GutenbergBook(int id_, vector<string> genres_, vector<string> authors_,   string language_, string title_){
  id = id_;
  genres = genres_;
  authors = authors_;
  language = language_;
  title  = title_;
}
int getId(){
        return id;
}
vector<string> getGenres(){
  return genres;
}
vector<string> getAuthors(){
  return authors;
}
string getLanguage(){
  return language;
}
string getTitle(){
  return title;
}
};