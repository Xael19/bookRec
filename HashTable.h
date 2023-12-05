// Code from BRIDGES example program of Gutenberg dataset used
#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <queue>
#include <stdlib.h>
#include <stdexcept>
#include "Bridges.h"
#include "DataSource.h"
#include "data_src/GutenbergBook.h"
using namespace std;
using namespace bridges;
// Contains all the properties of a book.
class book {
private:
  vector<std::string> genres;
  string language; 
  vector<std::string> author;
  string title;
  int bookID;
  int similarity;
public:
  book(vector<string> genres_, string language_, vector<string> authors_, string title_, int id_){
    genres = genres_;
    language = language_;
    author = authors_;
    title  = title_;
    bookID = id_;
  }
book(){};
~book(){};
  int getId(){
    return bookID;
  }
  vector<string> getGenres(){
    return genres;
  }
  vector<string> getAuthors(){
    return author;
  }
  string getLanguage(){
    return language;
  }
  string getTitle(){
    return title;
  }
  int getSim(){
    return similarity;
  }
  void setSim(int sim){
    similarity = sim + 1;
  }
};
// Contains all the properties of a hashtable.
class hashTable {
private:
  vector<int> keys{}; 
  vector<book> recommendations;
  vector<vector<book>> allBooks;
  int size;
  int capacity;
  float loadFactor;
public:
~hashTable(){};
  hashTable(): allBooks(50), capacity(50), loadFactor(0.8) {};
  vector<vector<book>> getBooks() {
    return allBooks;
  }
  int getCap() {
    return capacity;
  }
  int getSize() {
    return size;
  }
  float getLoadFac() {
    return loadFactor;
  }
  void pushBook(int& index, book& b) {
    allBooks[index].push_back(b);
  }
  void pushKey(int index) {
    keys.push_back(index);
  }
  void setSize(int s) {
    size = s;
  }
  void setCap() {
    capacity *= 2;
  }
  void setRec(vector<book>& rec) {
    recommendations = rec;
  }
  vector<book> getRec() {
    return recommendations;
  }
  void nukeKeys(){
    keys.clear();
  }
  void nukeBooks(){
    allBooks.clear();
    allBooks.resize(getCap());
  }
  void setBooks(vector<vector<book>>& temp) {
    allBooks = temp;
  }
  // Hash fucntion for hash table.
  int hasher(std::string str, int capacity) {
    int hash = 0;
    for (int i = 0; i < str.length(); i++) {
      hash += str[i];
    }
    return hash % capacity;
  }
};
// Gets a vector of books that have a certain genre and inserts these books into a hash table.
void bookInsertion (vector<GutenbergBook>& gbook_list, hashTable& table) {
  int index{};
  for (auto& b : gbook_list) {
    index = table.hasher(b.getTitle(), table.getCap());
    book temp {b.getGenres(), b.getLang(), b.getAuthors(), b.getTitle(), b.getId()};
    table.pushBook(index, temp);
    table.pushKey(index);
    table.setSize(table.getSize() + 1);
    // load factor of hash table.
    if ((float)(table.getSize()/table.getCap()) >= table.getLoadFac()) {
      table.setCap();
      table.setSize(0);
      vector<vector<book>> tempBook(table.getCap());
      table.nukeKeys();
      for (auto& b : table.getBooks()) {
        for (auto& i : b) {
          index = table.hasher(i.getTitle(), table.getCap());
          book temp { i.getGenres(), i.getLanguage(), i.getAuthors(), i.getTitle(), i.getId()};
          tempBook[index].push_back(temp);
          table.pushKey(index);
          table.setSize(table.getSize() + 1);
        }
      }
      table.nukeBooks();
      table.setBooks(tempBook);
    }
  }
}
// Traverses through entire hash table in order to determine what book best matches the users
// language and author preference. Books with the highest similarity to what the user wants is returned.
void recommender (hashTable& table, string language, string author) {
  int similarity{0};
  int smallestIndex{0};
  vector<book> temp = {};
  // Nested for loops that traverse through entire hash table.
  for (auto& a : table.getBooks()) {
    // Increases similarity if language user wants is in book.
    for (auto& b : a) {
      if (b.getLanguage() == language) {
        b.setSim(b.getSim());
      }
      // Increases similarity if author user wants is in book.
      for(int i = 0; i < b.getAuthors().size(); i++){
        if (b.getAuthors()[i] == author){
          b.setSim(b.getSim());
        }
      }
      if (temp.size() < 3) {
        temp.push_back(b);
      }
      // If a book with a better similarity score is found, it replaces book with the lowest 
      // similarity score.
      else {
        for (int i = 0; i < temp.size(); i++) {
          if (temp[smallestIndex].getSim() > temp[i].getSim()) {
            smallestIndex = i;
          }
        }
        if (temp[smallestIndex].getSim() < b.getSim()) {
          temp.erase(temp.begin() + smallestIndex);
          temp.push_back(b);
        }
     }
   }
    table.setRec(temp);
  }
}
// Prints all the book recommendations for the user.
void printRecs (hashTable& table, DataSource& ds) {
  for (int i = 0; i < table.getRec().size(); i++) {
    cout << "\n\n";
    cout << "Recommendation " << i + 1 << "\n";
    cout << "Title of book: " << table.getRec()[i].getTitle() << "\n";
    if (table.getRec()[i].getAuthors().size() == 1) {
      cout << "Author: " << table.getRec()[i].getAuthors()[0] << "\n";
    }
    else {
      cout << "Authors: ";
      for (int j = 0; j < table.getRec()[i].getAuthors().size(); j++){
        if (j < table.getRec()[i].getAuthors().size() - 1) {
          cout << table.getRec()[i].getAuthors()[j] << " & ";
        }
        else {
          cout << table.getRec()[i].getAuthors()[j] << "\n";
        }
      }
    }
    cout << "Genres: ";
    for (int j = 0; j < table.getRec()[i].getGenres().size(); j++){
      if (j < table.getRec()[i].getGenres().size() - 1) {
        cout << table.getRec()[i].getGenres()[j] << " | ";
      }
      else {
        cout << table.getRec()[i].getGenres()[j] << "\n";
      }
    }
    cout << "\nLanguage: " << table.getRec()[i].getLanguage()<< "\n";
    //string book_str = ds.getGutenbergBookText(table.getRec()[i].getId());
    /*
    cout << "\nThis book has " << book_str.size() << " characters.\n";
    cout << "\nHere is a small snippet of the book: \n";
    for (int k = 9000; k < 10000; k++) {
      cout << book_str[k];
    }
    cout << "\n\n\n";
    */
  }
}