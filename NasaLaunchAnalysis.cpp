// Author: Teague Wright
// CS-222 HW2 Part 2, Task 1: NASA Launch Analysis
// Reads Space_Corrected.csv and prints the average time of day of every launch.
// I read the assignment notes (note #3).
// Help: Claude (Anthropic's AI assistant) reviewed this code and explained C++ concepts.
// split() below was provided by Prof. Novak in an in-class Markov chain activity.
#include <iostream> // provides cout and probably other stuff
#include <fstream> // provides file I/O like ifstream and getline()
#include <vector> // allows use of vector<>
#include <string> // yeah, strings
#include <cassert> // assert() for the sanity check on the line count
#include "TimeCode.h" // for TimeCode class

using namespace std;

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
// Launch times look like "Fri Aug 07, 2020 05:12 UTC"; date-only lines have no colon,
// so splitting on ":" gives a single piece.
bool has_time(string str) { // helper, check if line (string) has a TimeCode in it
    return !((split(str, ":")).size() == 1);
}

// Hours are the 2 characters before the colon, minutes the 2 after.
// The CSV has no seconds, so those are always 0.
TimeCode parse_line(string str) {
    vector<string> parts = split(str, ":");
    string hours = parts.at(0).substr(parts.at(0).size() - 2, 2);
    string minutes = parts.at(1).substr(0, 2);
    return TimeCode(stoi(hours), stoi(minutes), 0);
}

vector<TimeCode> get_times(const vector<string>& lines) { // pass by ref to avoid copying 4k lines
    vector<TimeCode> times;
    for (size_t i = 1; i < lines.size(); i++) { // start at 1 to skip the header row
        string curr_line = lines.at(i);
        if (has_time(curr_line)) { // launches with only a date are ignored completely
            times.push_back(parse_line(curr_line));
        }
    }
    return times;
}

int main(){
    vector<string> lines = get_lines_from_file("Space_Corrected.csv");
    assert(lines.size() == 4325); // sanity check: the whole file was read, header included

    vector<TimeCode> times = get_times(lines);
    cout << times.size() << " data points. " << endl;

    // TimeCode's own + and / keep the arithmetic inside the class (as the spec requires),
    // and the result is already a TimeCode, so ToString() formats it as h:m:s.
    TimeCode sum;
    for (TimeCode tc : times) {
        sum = sum + tc;
    }
    sum = sum / times.size();
    cout << "AVERAGE: " + sum.ToString() << endl;

    return 0;
}
