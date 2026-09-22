//reads the data from this Space_Corrected.csv, extracts the time of day (UTC values), computes the average, and prints it out.
#include <iostream> // provides cout and probably other stuff
#include <fstream> // provides file I/O like ifstream and getline()
#include <vector> // allows use of vector<>
#include <string> // yeah, strings
#include <unordered_map> // for the weights HashMap
#include <cstdlib> // for srand() and rand()

using namespace std;

string vec_to_string(vector<string> v) {
    string str("[");
    if (v.size() == 0) {
        return "[]";
    }
    for (size_t i = 0; i < v.size(); i++) {
 
        str += v.at(i);
        if (i != v.size() - 1) {
            str += ", ";
        }
    }
    str += "]";
    return str;
}

vector<string> split(string line, string delim){
    // C++ has no split function so I had to make one!
    // example:   "what do you mean?"   delim: 'o'  -> ["what d", " y", "u mean?"]
	vector<string> ans(0);
	
	size_t s = 0;
	size_t e = line.find(delim);
	size_t tokenLength = (e-s);
	
	while(e != string::npos){
		ans.push_back(line.substr(s, tokenLength));
		//print_vector(ans);
		s = e + 1;
		e = line.find(delim, s);
		tokenLength = (e - s);
	}

	tokenLength = (line.size() - s);
	ans.push_back(line.substr(s, tokenLength));
	
	return ans;
}

vector<string> get_words_from_file(char* filename){

    ifstream text_file;
    text_file.open(filename);
    if(!text_file.is_open()){
        cout << "Could not open file: " << filename << endl;
        exit(1);
    }

    vector<string> words;
    string line;
    while(getline(text_file, line)){
        vector<string> new_words = split(line, " ");
        //cout << "new words: " << vec_to_string(new_words) << endl;
        words.insert(words.end(), new_words.begin(), new_words.end());
        words.push_back("\n");
        //cout << new_words.size() << " words added..." << endl;
    }


    return words;

}

int main(int argc, char *argv[]){

    if(argc == 1){
        std::cout << "Please provide text file name" << std::endl;
        std::cout << "Example: ./Space_Corrected.csv" << std::endl;
        return 1;
    }

    if(argc != 2){
        std::cout << "Incorrect usage!" << std::endl;
        std::cout << "Example: ./Space_Corrected.csv" << std::endl;
        return 1;
    }
}