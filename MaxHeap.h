#pragma once
#include <vector>
#include <iostream>
#include <string>
#include "Bridges.h"
#include "DataSource.h"
#include "data_src/GutenbergBook.h"
using namespace std;
using namespace bridges;
//class for implementation of max heap, with nested class for heap nodes
class MaxHeap{
  class HeapNode {
  private:
    //stores the node's library of congress ID and similarity rank value
    int bookId;
    int similarity;
  public:
    HeapNode(int bookId, int similarity){
      this->bookId = bookId;
      this->similarity= similarity;
    }
    HeapNode(){
      bookId = 0;
      similarity = 0;
    }
    int getBookId(){
      return bookId;
    }
    int getSimilarity() {
      return similarity;
    }
    ~HeapNode(){}
  };
  private:
    //stores the size of the heap and the vector representation of the heap
    int size;
    vector<HeapNode> bookVec;
    void buildHeap(string genre, string author, string language);
  public:
    void getRecs(string genre, string author, string language);
    MaxHeap(){
      this->size = 0;
      this->bookVec.resize(1);
    }
    int getHeapSize() {
      return size;
    }
    ~MaxHeap(){}
};
//builds the heap 
void MaxHeap::buildHeap(string genre, string author, string language) {
  //connects to BRIDGES
  Bridges bridges (1, getenv("UserID"), getenv("Key"));
  bridges.setTitle("Accessing Gutenberg Book Collection Meta Data");
  DataSource ds (&bridges);
  //creates a vector of books based on inputted genre
  vector<GutenbergBook> gbookList;
  try{
    gbookList = ds.getGutenbergBookMetaData(genre, "genre");
  }
  catch(...){
    return;
  }
  for (auto& b : gbookList) {
    int simVal = 1;
    //if the book's genre is explictly mentioned, the similarity value increases
    for(string verifiedGenre : b.getGenres()){
      if(verifiedGenre == genre){
        simVal++;
      }
    }
    //if book's language matches, the similarity value increases
    if(b.getLang() == language){
      simVal++;
    }
    //if one of the authors of the book was the inputted author, similarity value increases 
    for(int j = 0; j < b.getAuthors().size(); j++){
      if (b.getAuthors()[j] == author){
        simVal++;
      }
    }
    //a new node is created given library of congresss ID and similarity value
    MaxHeap::HeapNode insertedNode(b.getId(), simVal);
    //based on min heap pseudocode in Mark Weiss's DSA Analysis in C++ book
    //heapifying code 
    int hole = ++size;
    if(size == bookVec.size()){
      bookVec.resize(bookVec.size() * 2);
    }
    for(; hole > 1 && insertedNode.getSimilarity() > bookVec[hole / 2 - 1].getSimilarity(); hole/= 2)  {
      bookVec[hole - 1] = move(bookVec[hole / 2 - 1]);
    }
    bookVec[hole - 1] = move(insertedNode);
  }
}
//takes user inputted genre, author, and language 
//to output three recommended books from the Gutenberg BRIDGES dataset
void MaxHeap::getRecs(string genre, string author, string language) {
  //connects to BRIDGES 
  Bridges bridges (1, getenv("UserID"), getenv("Key"));
  bridges.setTitle("Accessing Gutenberg Book Collection Meta Data");
  DataSource ds (&bridges);
  //builds the heap
  buildHeap(genre, author,language);
  //if the heap is empty, an invalid genre was input
  if(size <= 0) {
    cout << "\nInvalid genre. Please try again." << "\n";
    return;
  }
  //prints the top three results in the heap
  //with their title, authors, language, and genres 
  for(int i = 0; i < 3; i++){
    GutenbergBook g = ds.getGutenbergBookMetaData(bookVec[i].getBookId());
    cout << "\n\n";
    cout << "Recommendation " << i + 1 << "\n";
    cout << "Title of book: " << g.getTitle() << "\n";
    if(g.getAuthors().size() == 1){
      cout << "Author: ";
    }
    else{
      cout << "Authors: ";
    }
    for(int j = 0; j < g.getAuthors().size(); j++){
      cout << g.getAuthors()[j];
      if (j < g.getAuthors().size() -1) {
        cout << " & ";
      }
    }
    cout << "\n"; 
    cout << "Genres: ";
    for(int h = 0; h < g.getGenres().size(); h++){
      cout << g.getGenres()[h];
      if (h < g.getGenres().size() - 1) {
        cout << " | ";
      }
    }
    cout << "\nLanguage: " << g.getLang() << "\n";
    /*
    string bookText = ds.getGutenbergBookText(bookVec[i].getBookId());
    cout << "\nThis book has: " << bookText.size() << " characters.\n";
    cout << "\nHere is a small snippet of the book: \n";
    for (int k = 9000; k < 10000; k++){
      cout << bookText[k];
    }
    cout << "\n\n\n";
    */
  }
}
