


// Author: Teague Wright
// CS-222 HW2 Part 2, Task 2: Paint Dry Timer
// Times batches of drying spheres: one second per square centimetre of surface area.
// I read the assignment notes (note #3).
// Help: Claude (Anthropic's AI assistant) reviewed this code and explained C++ concepts.

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


// Cast to signed first: subtracting from an unsigned value would wrap a finished
// batch to a huge positive number instead of going negative (negative means done).
long long int get_time_remaining(DryingSnapShot dss){
	return static_cast<long long int>(dss.timeToDry->GetTimeCodeAsSeconds()) - (time(0) - dss.startTime);
}


// The TimeCode is built only in the positive branch, since a negative count would
// wrap when converted to TimeCode's unsigned seconds.
string drying_snap_shot_to_string(DryingSnapShot dss){
	string s = "	" + dss.name + " (takes " + dss.timeToDry->ToString() + " to dry) ";
	long long int timeRemaining = get_time_remaining(dss);
	if (timeRemaining > 0) { 
		TimeCode tc = TimeCode(0, 0, timeRemaining);
		s = s + "time remaining: " + tc.ToString(); }
	else { s = s + "DONE!"; }
	return s;
}


// Surface area doubles as the drying time in seconds, so no unit conversion is needed.
double get_sphere_sa(double rad){
	return 4 * M_PI * rad * rad;
}


// new, because each batch's TimeCode outlives this function: the caller must delete it.
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

	// Each batch holds a TimeCode on the heap, so its timeToDry must be deleted
	// before the batch leaves this vector (or before the program ends).
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
			// A finished batch prints DONE! once and is removed afterwards, so the
			// count below is taken before the clean-up pass.
			for (size_t i = 0; i < tracker.size(); i++) {
				cout << drying_snap_shot_to_string(tracker.at(i)) << endl;
			}
			cout << "\t" << tracker.size() << " batches being tracked." << endl;

			// Backwards, so erasing can't shift an unchecked element past the cursor.
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

	// Batches still drying at quit time still own a TimeCode; without this they leak.
	for (size_t i = 0; i < tracker.size(); i++) {
		delete tracker.at(i).timeToDry;
	}
	tracker.clear();

	return 0;
}
