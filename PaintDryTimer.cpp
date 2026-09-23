


// Author: Teague Wright
// CS-222 HW2 Part 2, Task 2: Paint Dry Timer
// Tracks batches of spheres drying, where a batch takes one second per square
// centimetre of surface area. Times are stored as heap-allocated TimeCodes and
// deleted once a batch finishes or the user quits, so the program has no leaks.
// I read the assignment notes (note #3).
// Help: I used Claude (Anthropic's AI assistant) to review this code and explain C++ concepts.

#include <ctime> // for time(0)
#include <iostream> // for cin and cout
#include <cmath> // for M_PI and others
#include <vector> // for vectors (duh)
#include <cstdlib> // for random
#include <cassert> // for assert in the tests() function
#include "TimeCode.h" // for timecode's (duh)

using namespace std;




struct DryingSnapShot {
	// This is a struct, it's like an object
	// that doesn't have any methods.
	// You can read more about them in the ZyBook
	// just search for "struct"
	string name;
	time_t startTime;
	TimeCode *timeToDry;
};


// Seconds left before this batch is dry: its total drying time minus how long it has
// been sitting. The total is cast to a signed type first, because subtracting from an
// unsigned value would wrap a finished batch around to a huge positive number instead
// of going negative. Negative is meaningful here: it means the batch is done.
long long int get_time_remaining(DryingSnapShot dss){
	return static_cast<long long int>(dss.timeToDry->GetTimeCodeAsSeconds()) - (time(0) - dss.startTime);
}


// Builds the display line for one batch. Both forms share the same prefix and differ
// only at the end: a finished batch says DONE! instead of a remaining time. The TimeCode
// is only built inside the positive branch, since a negative count would wrap when
// converted to TimeCode's unsigned seconds.
string drying_snap_shot_to_string(DryingSnapShot dss){
	string s = "	" + dss.name + " (takes " + dss.timeToDry->ToString() + " to dry) ";
	long long int timeRemaining = get_time_remaining(dss);
	if (timeRemaining > 0) { 
		TimeCode tc = TimeCode(0, 0, timeRemaining);
		s = s + "time remaining: " + tc.ToString(); }
	else { s = s + "DONE!"; }
	return s;
}


// Surface area of a sphere: 4 * pi * r^2. This value doubles as the batch's drying
// time in seconds, which is why the timer needs no other unit conversion.
double get_sphere_sa(double rad){
	return 4 * M_PI * rad * rad;
}


// One second of drying per square centimetre. Allocated with new because each batch's
// TimeCode has to outlive this function; the caller owns it and must delete it.
// The cast to a whole number of seconds drops any fraction of a second.
TimeCode *compute_time_code(double surfaceArea){
	TimeCode *tc = new TimeCode(0, 0, static_cast<unsigned long long int>(surfaceArea));
	return tc;
}


void tests(){
	// get_time_remaining
	DryingSnapShot dss;
	dss.startTime = time(0);
	TimeCode tc = TimeCode(0, 0, 7);
	dss.timeToDry = &tc;
	long long int ans = get_time_remaining(dss);
	assert(ans > 6 && ans < 8);
	// add more tests here


	// get_sphere_sa
	double sa = get_sphere_sa(2.0);
	assert (50.2654 < sa && sa < 50.2655);
	// add more tests here


	// compute_time_code
	TimeCode *tc2 = compute_time_code(1.0);
	//cout << "tc: " << tc.GetTimeCodeAsSeconds() << endl;
	assert(tc2->GetTimeCodeAsSeconds() == 1);
	delete tc2;


	// add more tests here


	cout << "ALL TESTS PASSED!" << endl;

}


int main(){
	tests();

	// Every batch currently drying. Each holds a TimeCode on the heap, so a batch must
	// have its timeToDry deleted before it leaves this vector (or before the program ends).
	vector<DryingSnapShot> tracker;

	while (true) {
		cout << "Choose an option: (A)dd, (V)iew Current Items, (Q)uit: ";
		char choice;
		cin >> choice;

		if (choice == 'a' || choice == 'A') {
			cout << "\tradius: ";
			double radius;
			cin >> radius;

			DryingSnapShot dss;
			dss.name = "Batch-" + to_string(rand()); // the spec says duplicate ids are fine
			dss.startTime = time(0);                 // drying starts the moment it is added
			dss.timeToDry = compute_time_code(get_sphere_sa(radius));
			tracker.push_back(dss);

			cout << drying_snap_shot_to_string(dss) << endl;
		}
		else if (choice == 'v' || choice == 'V') {
			// Show every batch, including any that just finished: a finished batch prints
			// DONE! once and is only removed afterwards, which is why the count below is
			// taken before the clean-up pass.
			for (size_t i = 0; i < tracker.size(); i++) {
				cout << drying_snap_shot_to_string(tracker.at(i)) << endl;
			}
			cout << "\t" << tracker.size() << " batches being tracked." << endl;

			// Free finished batches. Looping backwards means erasing an element can't shift
			// an element we have not checked yet into a position we already passed.
			for (size_t i = tracker.size(); i > 0; i--) {
				size_t index = i - 1;
				if (get_time_remaining(tracker.at(index)) <= 0) {
					delete tracker.at(index).timeToDry; // matches the new in compute_time_code
					tracker.erase(tracker.begin() + index);
				}
			}
		}
		else if (choice == 'q' || choice == 'Q') {
			break;
		}
		else {
			cout << "\tUnknown option: " << choice << endl;
		}
	}

	// Batches still drying at quit time still own a TimeCode, so free them here.
	// Without this, quitting with batches left would leak one TimeCode each.
	for (size_t i = 0; i < tracker.size(); i++) {
		delete tracker.at(i).timeToDry;
	}
	tracker.clear();

	return 0;
}
