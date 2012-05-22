#include "Element.h"
#include <ctime>
#include <string>
#include <cstdlib>
#include <iostream>

using namespace std;

//constructor

Element::Element() {
	content = NULL;
	timestamp = 0;
	expired = true;
	priority = 0;
}

Element::Element(string* initial_content) {
	content = initial_content;
	timestamp = GetCurrentTimeInMilli();
	expired = false;
	priority = 0;
}

//member functions

long int Element::GetCurrentTimeInMilli() {
	return time(NULL);
}

bool Element::operator==(Element &other) {
	// Compare the values, and return a bool result.
	return timestamp == other.GetTimestamp();
}

bool Element::operator!=(Element &other) {
	return timestamp != other.GetTimestamp();
}

//setters

void Element::SetContent(string* c) {
	content = c;
}

void Element::SetPriority(double p) {
	priority = p;
}

void Element::SetTimestamp(long int ts) {
	timestamp = ts;
}

void Element::SetExpired(bool time_is_over) {
	expired = time_is_over;
}

//getters

string* Element::GetContent() {
	return content;
}

double Element::GetPriority() {
	return priority;
}

long int Element::GetTimestamp() {
	return timestamp;
}

bool Element::IsExpired() {
	return expired;
}