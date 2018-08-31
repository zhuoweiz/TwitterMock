#include "datetime.h"
#include <ctime>
#include <sstream>
#include <string>
#include <iomanip>

using namespace std;

DateTime::DateTime()
{
	reset();
}

DateTime::DateTime(int hh, int mm, int ss, int year, int month, int day)
{
	hour = hh;
	min = mm;
	sec = ss;
	this->year = year;
	this->month = month;
	this->day = day;
}

bool DateTime::operator<(const DateTime& other) const
{
	if(year<other.year) return true;
	else if(year>other.year) return false;
	else{
		if(month<other.month) return true;
		else if(month>other.month) return false;
		else{
			if(day<other.day) return true;
			else if(day>other.day) return false;
			else{
				if(hour<other.hour) return true;
				else if(hour>other.hour) return false;
				else{
					if(min<other.min) return true;
					else if(min > other.min) return false;
					else{
						if(sec<other.sec) return true;
						else return false;
					}
				}
			}
		}
	}

}

ostream& operator<<(ostream& os, const DateTime& other)
{

	os<<other.year<<"-";
	os<< setfill('0') << setw(2) << other.month;
	os<<"-";
	os<< setfill('0') << setw(2) << other.day;
	os<<" ";
	os<< setfill('0') << setw(2) << other.hour;
	os<<":";
	os<< setfill('0') << setw(2) <<other.min;
	os <<":";
	os<< setfill('0') << setw(2) <<other.sec;
	return os;
}

istream& operator>>(istream& is, DateTime& dt)
{
	string date,time;
	is>>date>>time;

	stringstream s1(date);
	stringstream s2(time);

	char t;

	if(s1.fail()||s2.fail()) 
	{
		dt.reset();
		return is;
	}

	if(date[4]!='-'||date[7]!='-')
	{
		dt.reset();
		return is;
	}

	if(time[2]!=':'||time[5]!=':')
	{
		dt.reset();
		return is;
	}

	if(date.length()!=10||time.length()!=8)
	{
		dt.reset();
		return is;
	}

	s1>>dt.year>>t>>dt.month>>t>>dt.day;
	s2>>dt.hour>>t>>dt.min>>t>>dt.sec;

	return is;
}

void DateTime::reset()
{
	time_t currentTime;
	struct tm *localTime;

	time(&currentTime);
	localTime = localtime(&currentTime);

	day = localTime->tm_mday;
	month = localTime->tm_mon + 1;
	year = localTime->tm_year + 1900;
	hour = localTime->tm_hour;
	min = localTime->tm_min;
	sec = localTime->tm_sec;
}