//reads the data from this Space_Corrected.csv, extracts the time of day (UTC values), computes the average, and prints it out.
#include <iostream> // provides cout and probably other stuff
#include <fstream> // provides file I/O like ifstream and getline()
#include <vector> // allows use of vector<>
#include <string> // yeah, strings
#include <unordered_map> // for the weights HashMap
#include <cstdlib> // for srand() and rand()
#include <cassert>
#include "TimeCode.h" // for TimeCode class

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

vector<string> get_lines_from_file(string filename){

    ifstream text_file;
    text_file.open(filename);
    if(!text_file.is_open()){
        cout << "Could not open file: " << filename << endl;
        exit(1);
    }

    vector<string> lines;
    string line;
    while(getline(text_file, line)){
        lines.push_back(line);
        
    }
    return lines;
}
bool has_time(string str) { // helper, check if line (string) has a TimeCode in it
    return !((split(str, ":")).size() == 1);
}
TimeCode parse_line(string str) { //takes a line from the file (a string). Returns the TimeCode object for the time embedded in that line.
    TimeCode tc;
    return tc;
}

vector<TimeCode> get_times(const vector<string>& lines) { // pass by ref to avoid copying 4k lines
    vector<TimeCode> times;
    for (size_t i = 1; i < lines.size(); i++) {
        string curr_line = lines.at(i);
        if (has_time(curr_line)) {
            vector<string> parts = split(curr_line, ":");
            string hours = parts.at(0).substr(parts.at(0).size() - 2, 2);
            string minutes = parts.at(1).substr(0, 2);
            TimeCode tc;
            tc.SetHours(stoi(hours));
            tc.SetMinutes(stoi(minutes));
            /**string s_hours = split(curr_line, ":").at(0).substr(curr_line.size() - 2, 2);
            string s_minutes = split(curr_line, ":").at(1).substr(0, 2);
            int i_hours = stoi(s_hours);
            int i_minutes = stoi(s_minutes);
            TimeCode tc;
            tc.SetHours(i_hours);
            tc.SetMinutes(i_minutes);
            **/
            times.push_back(tc);
        }
    }
    return times;
}

int main(){
    vector<string> lines = get_lines_from_file("Space_Corrected.csv");
    assert(lines.size() == 4325);
    cout << "# lines: " << lines.size() << endl;
    string test = lines.at(1);
    cout << test << endl;
    //We want the 2 characters left of the colon (hours) and the two right of the colon (minutes)
    vector<string> parts = split(test, ":");
    string hours = parts.at(0).substr(parts.at(0).size() - 2, 2);
    string minutes = parts.at(1).substr(0, 2);
    cout << hours + ":" + minutes << endl;

    vector<TimeCode> times = get_times(lines);
    cout << times.size() << endl;
    return 0;
}