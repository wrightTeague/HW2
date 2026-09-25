// Author: Teague Wright
// CS-222 HW2: tests for TimeCode
// Help: Claude (Anthropic's AI assistant) reviewed these tests and helped write some of them.

#include <iostream>
#include <assert.h>
using namespace std;

#include "TimeCode.h"


// The core conversion, so it runs first: most other tests depend on it.
void TestComponentsToSeconds(){
	cout << "Testing ComponentsToSeconds" << endl;
	
	// Random but "safe" inputs
	long long unsigned int t = TimeCode::ComponentsToSeconds(3, 17, 42);
	assert(t == 11862);
	
	// More tests go here!
	
	cout << "PASSED!" << endl << endl;
}


void TestDefaultConstructor(){
	cout << "Testing Default Constructor" << endl;
	TimeCode tc;
	
	//cout << "Testing ToString()" << endl;
	//cout << "tc: " << tc.ToString() << endl;
	assert(tc.ToString() == "0:0:0");
	
	cout << "PASSED!" << endl << endl;
}


// The constructor must roll out-of-range minutes/seconds over into a valid time.
void TestComponentConstructor(){
	cout << "Testing Component Constructor" << endl;
	TimeCode tc = TimeCode(0, 0, 0);
	//cout << "Testing ToString()" << endl;
	//cout << "tc: " << tc.ToString() << endl;
	assert(tc.ToString() == "0:0:0");
	
	// more tests go here!
	
	// Roll-over inputs
	TimeCode tc3 = TimeCode(3, 71, 3801);
	//cout << "tc3: " << tc3.ToString() << endl;
	assert(tc3.ToString() == "5:14:21");
	
	// More tests go here!
	
	cout << "PASSED!" << endl << endl;
}


void TestGetComponents(){
	cout << "Testing GetComponents" << endl;
	
	unsigned int h;
	unsigned int m;
	unsigned int s;
	
	// Regular values
	TimeCode tc = TimeCode(5, 2, 18);
	tc.GetComponents(h, m, s);
	assert(h == 5 && m == 2 && s == 18);
	
	// More tests go here!
	
	cout << "PASSED!" << endl << endl;
}


// Subtraction must borrow correctly and throw instead of going negative.
void TestSubtract(){
	cout << "Testing Subtract" << endl;
	TimeCode tc1 = TimeCode(1, 0, 0);
	TimeCode tc2 = TimeCode(0, 50, 0);
	TimeCode tc3 = tc1 - tc2;
	assert(tc3.ToString() == "0:10:0");
	
	
	// subtracting a larger time would go negative, so it must throw
	TimeCode tc4 = TimeCode(1, 15, 45);
	try{
		TimeCode tc5 = tc1 - tc4;
		cout << "tc5: " << tc5.ToString() << endl;
		assert(false);
	}
	catch(const invalid_argument& e){
		// just leave this empty
		// and keep doing more tests
	}

	// borrowing: 1:0:0 - 0:0:1 has to borrow from minutes and hours
	TimeCode tc6 = tc1 - TimeCode(0, 0, 1);
	assert(tc6.ToString() == "0:59:59");

	// equal times give exactly zero, which is allowed (not negative)
	TimeCode tc7 = tc1 - TimeCode(0, 60, 0);
	assert(tc7.ToString() == "0:0:0");

	// subtracting zero changes nothing
	TimeCode tc8 = tc4 - TimeCode();
	assert(tc8.ToString() == "1:15:45");

	// the operands themselves must not change
	assert(tc1.ToString() == "1:0:0");
	assert(tc2.ToString() == "0:50:0");

	// off by just one second still throws
	try{
		TimeCode tc9 = TimeCode(0, 0, 59) - TimeCode(0, 1, 0);
		assert(false);
	}
	catch(const invalid_argument& e){}

	// large hours
	TimeCode tc10 = TimeCode(2000000, 0, 0) - TimeCode(1999999, 30, 0);
	assert(tc10.ToString() == "0:30:0");
	
	cout << "PASSED!" << endl << endl;
}
// Most of these cases target roll-over, which is where addition gets tricky.
void TestAdd(){
	cout << "Testing Addition" << endl;
	TimeCode tc1 = TimeCode(1, 0, 3);
	TimeCode tc2 = TimeCode(0, 59, 57);
	TimeCode tc3 = tc1 + tc2;
	// 3 + 57 sec rolls over into a minute, which then rolls 59 min over into an hour
	assert(tc3.ToString() == "2:0:0");

	// roll-over example from the spec: 55 + 25 sec = 1 min 20 sec
	TimeCode tc4 = TimeCode(1, 15, 55) + TimeCode(0, 1, 25);
	assert(tc4.ToString() == "1:17:20");

	// simple case with no roll-over, from the spec
	TimeCode tc5 = TimeCode(1, 15, 22) + TimeCode(2, 9, 5);
	assert(tc5.ToString() == "3:24:27");

	// adding zero changes nothing
	TimeCode tc6 = tc1 + TimeCode();
	assert(tc6.ToString() == "1:0:3");

	// the operands themselves must not change
	assert(tc1.ToString() == "1:0:3");
	assert(tc2.ToString() == "0:59:57");

	// hours have no limit
	TimeCode tc7 = TimeCode(2000000, 0, 0) + TimeCode(3000000, 0, 0);
	assert(tc7.ToString() == "5000000:0:0");

	cout << "PASSED!" << endl << endl;
}


// Setters change one part only, and throw instead of rolling over.
void TestSetMinutes()
{
	cout << "Testing SetMinutes" << endl;

	TimeCode tc = TimeCode(8, 5, 9);
	tc.SetMinutes(15); // test valid change
	assert(tc.ToString() == "8:15:9");

	try
	{
		tc.SetMinutes(80);  // test invalid change
		assert(false);
	}
	catch (const invalid_argument &e)
	{
		// cout << e.what() << endl;
	}

	assert(tc.ToString() == "8:15:9");

	// boundaries: 0 and 59 are the smallest and largest valid minutes
	tc.SetMinutes(0);
	assert(tc.ToString() == "8:0:9");
	tc.SetMinutes(59);
	assert(tc.ToString() == "8:59:9");

	// 60 is the smallest invalid value, so it must throw (not roll over into an hour)
	try
	{
		tc.SetMinutes(60);
		assert(false);
	}
	catch (const invalid_argument &e) {}
	assert(tc.ToString() == "8:59:9");

	// with large hours, setting minutes must not touch the hours
	TimeCode big = TimeCode(2000000, 1, 1);
	big.SetMinutes(30);
	assert(big.ToString() == "2000000:30:1");

	cout << "PASSED!" << endl << endl;
}


void TestCopyConstructor(){
	cout << "Testing Copy Constructor" << endl;
	TimeCode tc1 = TimeCode(1, 0, 3);
	TimeCode tc2 = tc1; // calls the copy constructor
	assert(tc2.ToString() == "1:0:3");

	// changing the copy must not change the original, since they're separate objects
	tc2.SetHours(2);
	assert(tc1.ToString() == "1:0:3");
	assert(tc2.ToString() == "2:0:3");	
	cout << "PASSED!" << endl << endl;
}


void TestGetHours(){
	cout << "Testing GetHours" << endl;
	TimeCode tc1 = TimeCode(1, 0, 3);
	TimeCode tc0 = TimeCode(0, 0, 0);
	TimeCode tcBig = TimeCode(2000000);  // hours have no limit; also big enough to overflow without the cast in ComponentsToSeconds
	TimeCode tc2 = TimeCode(0, 120, 0);  // hours that only come from minutes rolling over

	assert(tc1.GetHours() == 1);
	assert(tc0.GetHours() == 0);
	assert(tcBig.GetHours() == 2000000);
	assert(tc2.GetHours() == 2);


	cout << "PASSED!" << endl << endl;
}


void TestGetMinutes(){
	cout << "Testing GetMinutes" << endl;

	TimeCode tc1 = TimeCode(1, 25, 3);
	TimeCode tc0 = TimeCode(0, 0, 0);
	TimeCode tc59 = TimeCode(4, 59, 59);
	TimeCode tc2 = TimeCode(0, 0, 150);    // 150 sec rolls over to 2 min 30 sec
	TimeCode tcRoll = TimeCode(0, 125, 0); // 125 min rolls over to 2 hr 5 min

	assert(tc1.GetMinutes() == 25);
	assert(tc0.GetMinutes() == 0);
	assert(tc59.GetMinutes() == 59);
	assert(tc2.GetMinutes() == 2);
	assert(tcRoll.GetMinutes() == 5);

	cout << "PASSED!" << endl << endl;
}


void TestGetSeconds(){
	cout << "Testing GetSeconds" << endl;

	TimeCode tc1 = TimeCode(1, 25, 3);
	TimeCode tc0 = TimeCode(0, 0, 0);
	TimeCode tc59 = TimeCode(4, 59, 59);
	TimeCode tcRoll = TimeCode(0, 0, 61);  // 61 sec rolls over to 1 min 1 sec
	TimeCode tcSpec = TimeCode(2, 71, 234719572143); // huge seconds example from the spec

	assert(tc1.GetSeconds() == 3);
	assert(tc0.GetSeconds() == 0);
	assert(tc59.GetSeconds() == 59);
	assert(tcRoll.GetSeconds() == 1);
	assert(tcSpec.GetSeconds() == 3);
	assert(tcSpec.ToString() == "65199884:20:3");

	cout << "PASSED!" << endl << endl;
}


// SetHours has no invalid input: no limit on hours, and unsigned rules out negatives.
void TestSetHours(){
	cout << "Testing SetHours" << endl;

	TimeCode tc = TimeCode(8, 5, 9);
	tc.SetHours(3); // minutes and seconds must stay 5 and 9
	assert(tc.ToString() == "3:5:9");

	tc.SetHours(0);
	assert(tc.ToString() == "0:5:9");

	tc.SetHours(2000000); // no upper limit on hours
	assert(tc.ToString() == "2000000:5:9");

	cout << "PASSED!" << endl << endl;
}


void TestSetSeconds(){
	cout << "Testing SetSeconds" << endl;

	TimeCode tc = TimeCode(8, 5, 9);
	tc.SetSeconds(15);
	assert(tc.ToString() == "8:5:15");
	tc.SetSeconds(0);
	assert(tc.ToString() == "8:5:0");
	tc.SetSeconds(59);
	assert(tc.ToString() == "8:5:59");
	try
	{
		tc.SetSeconds(60);  // test invalid change
		assert(false);
	}
	catch (const invalid_argument &e)
	{
		// cout << e.what() << endl;
	}

	assert(tc.ToString() == "8:5:59");

	cout << "PASSED!" << endl << endl;
}


void TestReset(){
	cout << "Testing Reset" << endl;
	TimeCode tc = TimeCode(8, 5, 9);
	tc.reset();
	assert(tc.ToString() == "0:0:0");
	tc.reset();
	assert(tc.ToString() == "0:0:0");

	cout << "PASSED!" << endl << endl;
}


void TestMultiply(){
	cout << "Testing Multiply" << endl;
	TimeCode tc = TimeCode(1, 0, 0);
	TimeCode tc2 = tc * 2;
	assert(tc2.ToString() == "2:0:0");
	TimeCode tc3 = tc * .5; // example from the spec
	assert(tc3.ToString() == "0:30:0");
	TimeCode tc4 = tc * 1;
	assert(tc4.ToString() == "1:0:0");
	TimeCode tc5 = tc * 0;
	assert(tc5.ToString() == "0:0:0");
	assert(tc.ToString() == "1:0:0"); // original is unchanged

	try 
	{ 
		TimeCode tc6 = tc * -1;
		assert(false);
	}
	catch (const invalid_argument &e) {}

	assert(tc.ToString() == "1:0:0");
	cout << "PASSED!" << endl << endl;
}


void TestDivide(){
	cout << "Testing Divide" << endl;
	TimeCode tc = TimeCode(1, 0, 0);
	TimeCode tc2 = tc / 4;
	assert(tc2.ToString() == "0:15:0");
	TimeCode tc3 = tc / .5; // dividing by a fraction makes the time bigger
	assert(tc3.ToString() == "2:0:0");
	TimeCode tc4 = tc / 1;
	assert(tc4.ToString() == "1:0:0");
	assert(tc.ToString() == "1:0:0");

	try
	{
		TimeCode tc5 = tc / -1;
		assert(false);
	}
	catch (const invalid_argument &e) {}

	try
	{
		TimeCode tc6 = tc / 0;
		assert(false);
	}
	catch (const invalid_argument &e) {}

	assert(tc.ToString() == "1:0:0");

	cout << "PASSED!" << endl << endl;
}


// Equality compares total time, not how the TimeCode was built.
void TestEquality(){
	cout << "Testing == and !=" << endl;
	TimeCode tc1 = TimeCode(1, 0, 0);
	TimeCode tc2 = TimeCode(1, 0, 0);
	assert(tc1 == tc2);
	assert(!(tc1 != tc2));

	TimeCode tc3 = TimeCode(0, 60, 0); // same time built differently, so equal after roll-over
	assert(tc1 == tc3);
	assert(!(tc1 != tc3));

	TimeCode tc4 = TimeCode(2, 0, 0);
	assert(tc4 != tc1);
	assert(!(tc4 == tc1));

	TimeCode tc5 = TimeCode(1, 0, 1); // differs by only one second
	assert(tc5 != tc1);
	assert(!(tc5 == tc1));

	cout << "PASSED!" << endl << endl;
}


void TestLessGreater(){
	cout << "Testing <, <=, >, >=" << endl;
	TimeCode small = TimeCode(1, 0, 0);
	TimeCode big = TimeCode(2, 0, 0);

	// check every operator in both directions
	assert(small < big);
	assert(small <= big);
	assert(!(small > big));
	assert(!(small >= big));
	assert(big > small);
	assert(big >= small);
	assert(!(big < small));
	assert(!(big <= small));

	// equal TimeCodes: the "or equal" operators are true, the strict ones are false
	TimeCode same = TimeCode(0, 60, 0);
	assert(small <= same);
	assert(small >= same);
	assert(!(small < same));
	assert(!(small > same));

	// times that differ in only one part, to make sure every part counts
	TimeCode base = TimeCode(5, 30, 30);
	assert(base < TimeCode(6, 30, 30));  // hours only
	assert(base < TimeCode(5, 31, 30));  // minutes only
	assert(base < TimeCode(5, 30, 31));  // seconds only
	assert(base > TimeCode(5, 30, 29));

	// more minutes/seconds don't beat more hours: 2:0:0 is bigger than 1:59:59
	assert(TimeCode(1, 59, 59) < TimeCode(2, 0, 0));

	cout << "PASSED!" << endl << endl;
}



	
int main(){
	
	TestComponentsToSeconds();
	TestGetComponents();
	TestDefaultConstructor();
	TestComponentConstructor();
	TestSetMinutes();
	TestAdd();
	TestSubtract();
	TestCopyConstructor();
	TestGetHours();
	TestGetMinutes();
	TestGetSeconds();
	TestSetHours();
	TestSetSeconds();
	TestReset();
	TestMultiply();
	TestDivide();
	TestEquality();
	TestLessGreater();
	
	cout << "PASSED ALL TESTS!!!" << endl;
	return 0;
}
