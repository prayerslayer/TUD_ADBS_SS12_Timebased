#include "GeneratorLoop.h"
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <string>
using namespace std;

GeneratorLoop::GeneratorLoop(int wait) {
	secs_to_wait = wait;
}

void GeneratorLoop::operator()() {	
	Wait(secs_to_wait);
}

void GeneratorLoop::Wait(int seconds) {
	clock_t endwait;
	endwait = clock () + seconds * CLOCKS_PER_SEC ;
	while (clock() < endwait) {
		//do nothing
	}
}