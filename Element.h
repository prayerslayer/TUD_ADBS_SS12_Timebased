#ifndef ELEMENT_H
#define ELEMENT_H

#include <string>
using namespace std;

class Element {
	public:
		Element();
		Element(string* content);
		long int GetTimestamp() const;
		string* GetContent() const;
		double GetPriority() const;
		bool IsExpired() const;
		void SetTimestamp(long int timestamp);
		void SetContent(string* content);
		void SetPriority(double priority);
		void SetExpired(bool expired);
		bool operator==(const Element &other) const;
		bool operator!=(const Element &other) const;
		bool operator<(const Element &other) const;
		bool operator>(const Element &other) const;
	private:
		double priority;
		string* content;
		long int timestamp;
		bool expired;
		long int GetCurrentTimeInMilli();
};

#endif