#include <iostream>
#include "hello.h"

#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <cstring>
#include <vector>
#include <map>
#include <set>

using namespace std;

void lexicon(map <int, set <string> > &m,   ifstream &ifs);
void openFile (ifstream &file, string fileName);
void getUserWordLength(int &wordLength, const map <int, set <string> > &m);
void  getNumberGuesses(int &numberGuesses);
void play(set <string> &listWord, int numberGuesses, int wordLength);
void getUserLetter(char &userLetter,  set <char> &lettersGuessed);
void getWordsFamily(string &famName,set <string> &listWord,char userLetter,const int &wordLength);
void changeCurentWord(string &curentWord, const string &familyName, const char &userLetter);

const string FILE_NAME = "dictionary.txt";

int main() {

	// 1- First we open the file
	ifstream file;
	openFile(file, FILE_NAME );
	//2- we build our list of words by length
	map <int, set <string> > m;
	lexicon(m, file);
	file.close();
	// cout<<m.size()<<endl;
	// for(string c:m[21]){
	//     cout<<c<<endl;
	// }

	// 3- Get the user word length
	int wordLength;
	getUserWordLength(wordLength,m);

	// 4 we construct our word set
	set <string> listWord = m[wordLength];

	//5- get user number of guesses
	int numberGuesses;
	getNumberGuesses(numberGuesses);

	//6- play the game
	play( listWord, numberGuesses, wordLength);
	return 0;
}

//================ Help Functions ========================

void lexicon(map <int, set <string> > &m,   ifstream &ifs){

	string word;
	while(getline(ifs,word)) {
		// v[word.size()].insert(word);
		// m.push_back(word);
		m[word.size()].insert(word);
	}
}
/**
 * @brief openFile : this function opens the files into a ifstream
 * @param file     : the filestream
 */
void openFile (ifstream &file, string fileName){
	file.open(fileName);
	if(file.is_open()) {
		return;
	}
	cout << "Please enter a valid file name" << endl;
}

void getUserWordLength(int &wordLength,const map <int, set <string> > &m){
	while (true) {
repeat:
		cout << "Please enter the wordl lenth: ";
		string wordLengthStr;
		std::getline(cin, wordLengthStr);
		try{
			wordLength = std::stoi(wordLengthStr);
		}catch(...) {
			cout << "Please enter a valid value" << endl;
			goto repeat;
		}
		if(m.find(wordLength) == m.end()) {
			cout << "There is no word in the English dict with length " << wordLength << endl;
			goto repeat;
		}
		return;
	}
}

void  getNumberGuesses(int &numberGuesses) {
	while (true) {
repeat:
		cout << "Please enter the number of guesses: ";
		string numberGuessesStr;
		std::getline(cin, numberGuessesStr);
		try{
			numberGuesses = std::stoi(numberGuessesStr);
		}catch(...) {
			cout << "Please enter a valid value" << endl;
			goto repeat;
		}
		if(numberGuesses <= 0) {
			cout << "Please enter a positive value (your value was: " << numberGuesses << ")" << endl;
			goto repeat;
		}
		return;
	}
}

void play(set <string> &listWord, int numberGuesses, int wordLength){
	set <char> lettersGuessed;
    std::string curentWord = string(wordLength,'_');
    while(numberGuesses > 0) {
		cout << "You still have " << numberGuesses << " guesses left!" << endl;
		cout << "Words Remaining: " << listWord.size() << endl;
		cout << "Letters Guessed:";
		for(char c:lettersGuessed) cout << c << " ";
		cout << endl;
		cout << "Curent Word: " << curentWord << endl;

		// 1- get letter guessed
		char userLetter;
		getUserLetter(userLetter,lettersGuessed);
		numberGuesses--;

        //2- get words family
        string  familyName;
        getWordsFamily(familyName,listWord,userLetter, wordLength);
       // cout<<familyName<<endl;
       // cout<<familyName.size()<<endl;
       // cout<<curentWord.size()<<endl;
        //3- print new curentWord
        changeCurentWord(curentWord,familyName,userLetter);
        //4- check user won
        if(curentWord.find('_') == std::string::npos){
            cout<< "Great you found the word : " <<curentWord<< endl;
            return;
        }
	}
     cout<< "Sorry you didnt find the word, which was:  ";
     set<string>::const_iterator it(listWord.begin());
     cout <<*it<< endl;
}
void getUserLetter(char &userLetter,  set <char> &lettersGuessed ){
	while (true) {
        string userGuess;
		cout << "Guess! : ";
        std::getline(cin, userGuess);
		cout << endl;
        //userGuess = tolower(userGuess);
        std::transform(userGuess.begin(), userGuess.end(), userGuess.begin(), ::tolower);
		string alphabet = "abcdefghijklmnopqrstuvwxyz";
        if(userGuess.size()!=1 || alphabet.find(userGuess) == std::string::npos) {
			cout << "Not a valid letter (your value was: " << userGuess << ")" << endl;
		}
        else if(lettersGuessed.find(userGuess[0]) != lettersGuessed.end()) {
			cout << "That letter has already been guessed" << endl;
		}
		else{
            lettersGuessed.insert(userGuess[0]);
            userLetter=userGuess[0];
			return;
		}
	}
}
void getWordsFamily(string &famName,set <string> &listWord,char userLetter,const int &wordLength){
    map<string , set<string>> mapOfWordsFamily;
    string familyName;
   // cout <<userLetter<<endl;
   // cout <<wordLength<<endl;

    for(auto word:listWord){
        //cout <<word<<endl;
        //cout <<(word.find(userLetter) == std::string::npos)<<endl;
        if(word.find(userLetter) == std::string::npos){
         //   cout <<string(wordLength,'0')<<endl;
            mapOfWordsFamily[string(wordLength,'0')].insert(word);
        }
        else{
            for(char c :word ){
                if(c==userLetter) familyName.append("1");
                else familyName.append("0");
            }
            //cout <<familyName<<endl;
            mapOfWordsFamily[familyName].insert(word);
            familyName="";
        }
    }
   // for (string s: mapOfWordsFamily[string(wordLength,'0')]) cout<<s<<endl;
    int maxWords=0;
    set <string> maxList;
    for (map<string, set<string>>::iterator itr = mapOfWordsFamily.begin(); itr != mapOfWordsFamily.end(); ++itr){
        maxList=itr->second;
       // cout<<maxList.size()<<endl;
       // cout<<(maxList.size()>maxWords)<<endl;
        if(maxList.size()>maxWords){
          maxWords=maxList.size();
         // cout<<maxWords<<endl;
          listWord = itr->second;
          famName = itr->first;
        //  cout<<famName<<endl;
    }
  }
}

void changeCurentWord(string &curentWord, const string &familyName, const char &userLetter){
    if(curentWord.size()!=familyName.size()) cout<< "Hghhhhhhhhhhhhh"<<endl;
    for(size_t i=0; i<curentWord.size();++i){
        if(familyName[i]=='1'){
            size_t len=0;
            curentWord[i]=userLetter;
        }
    }
}
