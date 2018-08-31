#include "tweet.h"
#include <sstream>

using namespace std;

Tweet::Tweet()
{
}

Tweet::Tweet(User* user, const DateTime& time, const std::string& text)
{
	t_user = user;
	t_username = user->name();
	t_time = time;
	t_content = text;
	
	string garbage, hashtag;
	stringstream s1(text);
	while(true)
	{
		getline(s1, garbage, '#');
		getline(s1, hashtag, ' ');

		if(s1.fail())
			{break;}
		else{
			t_hashtags.insert(hashtag);
		}
	}

	user->addTweet(this);
}

DateTime const & Tweet::time() const
{
	return t_time;
}

std::string const & Tweet::text() const
{
	return t_content;
}

std::set<std::string> Tweet::hashTags() const
{
	return t_hashtags;
}

bool Tweet::operator<(const Tweet& other) const
{
	return (this->t_time)<(other.t_time);
}

//
ostream& operator<<(ostream& os, const Tweet& t)
{
	os<<t.t_time<<" "<<t.t_username<<" "<<t.t_content;
	return os;
}

User* Tweet::user() const
{
	return t_user;
}