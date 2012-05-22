#ifndef ELEMENT_H
#define ELEMENT_H

#include <string>
using namespace std;

class Element {
	public:
		Element();
		Element(string* content);
		long int GetTimestamp();
		string* GetContent();
		double GetPriority();
		bool IsExpired();
		void SetTimestamp(long int timestamp);
		void SetContent(string* content);
		void SetPriority(double priority);
		void SetExpired(bool expired);
		bool operator==(Element &other);
		bool operator!=(Element &other);
	private:
		double priority;
		string* content;
		long int timestamp;
		bool expired;
		long int GetCurrentTimeInMilli();
};

#endif