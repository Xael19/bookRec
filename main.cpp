#include "Bridges.h"
#include "DataSource.h"
#include "data_src/GutenbergBook.h"
#include "HashTable.h"
#include "MaxHeap.h"
#include <chrono> 
using namespace bridges;
using namespace std;
int main(int argc, char **argv) {
  // create bridges object
  Bridges bridges (1, getenv("UserID"), getenv("Key"));
  // set title
  bridges.setTitle("Accessing Gutenberg Book Collection Meta Data");
  // read the Gutenberg book data
  DataSource ds (&bridges);
  cout << "\nWelcome to our book recommendation generator!" << endl;
  bool cont = true;
  while(cont) {
    //user chooses their preferred data structure, genre, author, language 
    cout << "\nWould you like to generate your recommendations using a hash table or a max heap? Enter \"hash\" or \"heap\"" << endl;
    string choice, exit;
    getline(cin, choice);
    if(choice == "heap") {
      cout << "\nPlease enter your favorite genre. An example input is \"Historical Fiction\"" << endl; 
      string genre;
      getline(cin, genre);
      cout << "\nPlease enter your favorite author. An example input is \"Cooke, Grace MacGowan\"" << endl;
      string author; 
      getline(cin, author);
      cout << "\nPlease enter your preferred language. An example input is \"es\" for spanish (español)" << endl; 
      string language; 
      getline(cin, language);
      //user sees time taken by heap
      auto start = chrono::high_resolution_clock::now();
      MaxHeap maxHeap;
      maxHeap.getRecs(genre, author, language);
      auto stop = chrono::high_resolution_clock::now();
      auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
      if(maxHeap.getHeapSize() > 0) {
        cout << "\nTime taken by heap: " << duration.count() << " microseconds" << endl;
      }
    }       
    else if(choice == "hash") {
      cout << "\nPlease enter your favorite genre. An example input is \"Historical Fiction\"" << endl; 
      string genre;
      getline(cin, genre);
      cout << "\nPlease enter your favorite author. An example input is \"Cooke, Grace MacGowan\"" << endl;
      string author; 
      getline(cin, author);
      cout << "\nPlease enter your preferred language. An example input is \"es\" for spanish (español)" << endl; 
      string language; 
      getline(cin, language);
      //user sees time taken by hash table
      auto start = chrono::high_resolution_clock::now();
      hashTable table;
      vector<GutenbergBook> gbook_list = ds.getGutenbergBookMetaData(genre, "genre");
      bookInsertion(gbook_list, table);
      recommender(table, language, author);
      printRecs (table, ds);
      auto stop = chrono::high_resolution_clock::now();
      auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
      cout << "\nTime taken by hash table: " << duration.count() << " microseconds" << endl;  
   }
    else{
      cout << "Invalid choice! Please try again." << "\n";
    }
    cout << "\n\nWould you like to generate another recommendation? Type \"yes\" for yes and \"no\" for no." << endl;
    getline(cin, exit);
    if(exit == "no" || exit == "No") {
      cont = false;
    } 
  }
    return 0;
}
