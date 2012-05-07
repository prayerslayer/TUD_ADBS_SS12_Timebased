#include "GeneratorLoop.h"
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <string>
using namespace std;

GeneratorLoop::GeneratorLoop() {
	

}

void GeneratorLoop::operator()() {	
	while(true) {
		int secs = rand() % 100000;
		cout << "waiting... " << secs << " seconds" << endl;
		Wait(1);	
	}
}

void GeneratorLoop::Wait(int seconds) {
	clock_t endwait;
	endwait = clock () + seconds * CLOCKS_PER_SEC ;
	while (clock() < endwait) {
		//do nothing
	}
}