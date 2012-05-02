#ifndef ELEMENT_H
#define ELEMENT_H

#include <string>
using namespace std;

class Element {
	public:
		Element(string content);
		long int GetTimestamp();
		string GetContent();
		double GetPriority();
		void SetTimestamp(long int timestamp);
		void SetContent(string content);
		void SetPriority(double priority);
	private:
		double priority;
		string content;
		long int timestamp;
		double GeneratePriority();
		long int GetCurrentTimeInMilli();
};

#endif