#include "Element.h"
#include <ctime>
#include <string>
#include <cstdlib>
#include <iostream>

using namespace std;

//constructor

Element::Element(string initial_content) {
	content = initial_content;
	timestamp = GetCurrentTimeInMilli();
	priority = GeneratePriority();
}

//member functions

double Element::GeneratePriority() {
	return ((double)rand()/(double)RAND_MAX);
}

long int Element::GetCurrentTimeInMilli() {
	return time(NULL);
}

//setters

void Element::SetContent(string c) {
	content = c;
}

void Element::SetPriority(double p) {
	priority = p;
}

void Element::SetTimestamp(long int ts) {
	timestamp = ts;
}

//getters

string Element::GetContent() {
	return content;
}

double Element::GetPriority() {
	return priority;
}

long int Element::GetTimestamp() {
	return timestamp;
}