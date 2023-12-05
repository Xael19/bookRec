/* #include <vector>
#include <string>
#include "Books.h"
using namespace std;

 vector<GutenbergBook> initialize(){
  vector<GutenbergBook> list;
  GutenbergBook one(3, {"History", "Art", "Art History"}, {"J. Gutenburg"}, "English", "Happy, Happy Day");
  list.push_back(one);
  GutenbergBook two(1, {"Model", "Fashion", "Design", "History"}, {"Gucci", "Prada"}, "English", "How to Look");
  list.push_back(two);
  GutenbergBook three(2, {"Fiction", "History", "Art", "Design"}, {"Herman Melville"}, "English", "Moby Dick");
  list.push_back(three);
  GutenbergBook four(4, {"Fiction", "Model", "Art History"}, {"J. Gutenburg"}, "English", "Gutenburg's Day");
  list.push_back(four);
   GutenbergBook five(0, {"Non-Fiction", "History", "Art History"}, {"da Vinci"}, "English", "Mona Lisa");
   list.push_back(five);
  return list;
}

GutenbergBook getGutenbergBookMetaData(int id_, vector<GutenbergBook> list){
  for(int i = 0; i < list.size(); i++){
    if(list[i].getId() == id_){
      return list[i];
    }
  }
  //return nullptr;
}

vector<GutenbergBook> getGutenbergBookMetaData(string rand, string category, vector<GutenbergBook> list){
  vector<GutenbergBook> ret;
  if(category == "title"){
    for(int i = 0; i < list.size(); i++){
      if(list[i].getTitle() == rand){
        ret.push_back(list[i]);
      }
  }
}
  else if(category == "author"){
    for(int i = 0; i < list.size(); i++){
      for(int j = 0; j < list[i].getAuthors().size(); j++){
        if(list[i].getAuthors()[j] == rand){
          ret.push_back(list[i]);
        }
      }
    }
  }
  else if(category == "language"){
    for(int i = 0; i < list.size(); i++){
        if(list[i].getLanguage() == rand){
          ret.push_back(list[i]);
        }
    }
  }
  else if(category == "genre"){
    for(int i = 0; i < list.size(); i++){
      for(int j = 0; j < list[i].getGenres().size(); j++){
        if(list[i].getGenres()[j] == rand){
          ret.push_back(list[i]);
        }
      }
    }
  }
  return ret;
} */