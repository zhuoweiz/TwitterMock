#include "user.h"
#include "tweet.h"
#include <algorithm>
#include <iostream>

User::User(std::string name)
	:u_username(name)
{
}

User::~User(){
	for(std::list<Tweet*>::iterator it = u_tweets.begin(); it!= u_tweets.end();++it)
	{
		delete *it;
	}
}

std::string User::name() const
{
	return u_username;
}

std::set<User*> User::followers() const
{
	return u_followers;
}

std::set<User*> User::following() const
{
	return u_following;
}

std::list<Tweet*> User::tweets() const
{
	return u_tweets;
}

void User::addFollower(User* u)
{
	u_followers.insert(u);
	// std::cout<<u_username<< " get new follower: "<<u->name()<<std::endl;
}

void User::addFollowing(User* u)
{
	u_following.insert(u);
	// std::cout<<u_username<< " get new following: "<<u->name()<<std::endl;
}

void User::addTweet(Tweet* t)
{
	u_tweets.push_back(t);
}

bool TweetComp(Tweet* t1, Tweet* t2)
{
	return !((*t1)<(*t2));
}

std::vector<Tweet*> User::getFeed()
{
	std::vector<Tweet*> result;
	for(std::list<Tweet*>::iterator it = u_tweets.begin(); it!=u_tweets.end(); ++it)
	{
		result.push_back(*it);
	}

	for(std::set<User*>::iterator it = u_following.begin(); it != u_following.end(); ++it)
	{
		std::list<Tweet*> temp = (*it)->tweets();
		for(std::list<Tweet*>::iterator it2 = temp.begin(); it2!=temp.end();++it2)
		{
			result.push_back(*it2);
		}
	}

	// for(std::vector<Tweet*>::iterator it = result.begin();it!=result.end();++it)
	// {
	// 	std::cout<<"FEEDING: "<<(**it)<<std::endl;
	// }
	
	std::sort(result.begin(), result.end(), TweetComp);

	return result;
}

//hw8
void User::updatePassword(unsigned long long hashedPW)
{
	this->hashedPW_ = hashedPW;
}

bool User::canLogin(unsigned long long hashedPW)
{
	return this->hashedPW_ == hashedPW;
}

unsigned long long User::getHashedPW()
{
	return hashedPW_;
}
