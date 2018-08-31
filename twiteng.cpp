#include "twiteng.h"
#include <fstream>
#include <sstream>
#include <cctype>
#include <stdexcept>
#include <algorithm>
#include "tweet.h"
#include "util.h"
 
using namespace std;

TwitEng::TwitEng()
	: trending_(2)
{

}

TwitEng::~TwitEng()
{
	for(map<string, User*>::iterator it = userMap.begin(); it!=userMap.end(); ++it)
	{
		delete (it->second);
	}
}

bool TwitEng::parse(char* filename)
{
	ifstream ifile(filename);
	if(ifile.fail())
	{
		cout<<"cant open the file"<<endl;
		return true;
	}

	string line;
	getline(ifile, line);

	stringstream ss(line);
	int userNumber;
	ss>>userNumber;

	if(ss.fail())
	{
		return true;
	}

	//for each user row
	for(int i=0;i<userNumber;i++)
	{
		// cout<<"----"<<endl;
		getline(ifile, line);
		if(line.empty()) continue;

		stringstream s1(line);
		if(s1.fail())
		{
			return true;
		} 

		string username, followername;
		unsigned long long hashedPw;
		User* newUser;
		map<string, User*>::iterator it;
		s1>>username>>hashedPw;
		it = userMap.find(username);

		//if find user, keep going, else create one
		if(it==userMap.end()){
			newUser = new User(username);

			userMap.insert(pair<string, User*>(username, newUser));
		}else{
			newUser = it->second;
		}

		//update hashedPW
		newUser->updatePassword(hashedPw);

		//for each username, get followers name
		while(true)
		{
			// cout<<"--"<<endl;
			s1>>followername;
			if(s1.fail()){
				break;
			}

			// cout<<username<<" is following :"<<followername<<endl;

			//if find user, keep going, else create one
			map<string, User*>::iterator it_findFollowing = userMap.find(followername);
			if(it_findFollowing==userMap.end()){
				User* newFollower = new User(followername);
				// cout<<"create new user: "<<followername<<endl;

				userMap.insert(pair<string, User*>(followername, newFollower));
				// cout<<"insert.. map size is: " <<userMap.size()<<endl;

				newUser->addFollowing(newFollower);
				newFollower->addFollower(newUser);

				// cout<<username<<" now following: "<<newUser->following().size()<<endl;
				// cout<<username<<" now followed: "<<newUser->followers().size()<<endl;

			}else{
				newUser->addFollowing(it_findFollowing->second);
				(it_findFollowing->second)->addFollower(newUser);

				// cout<<username<<" now following: "<<newUser->following().size()<<endl;
				// cout<<username<<" now followed: "<<newUser->followers().size()<<endl;
			}
		}
	}

	//parse in tweets
	while(getline(ifile,line))
	{
		if(line.empty()) continue;

		stringstream ss(line);

		DateTime thisDate;
		string thisUsername, thisContent;

		ss>>thisDate;
		ss>>thisUsername;

		string temp;
		ss>>temp;
		getline(ss, thisContent);
		thisContent = temp + thisContent;
		thisContent.substr(1,thisContent.length()-1);
		// cout<<thisContent<<endl;

		addTweet(thisUsername, thisDate, thisContent);
	}

	return false;
}

void TwitEng::addTweet(const string& username, const DateTime& time, const string& text)
{
	map<string, User*>::iterator it = userMap.find(username);
	if(it==userMap.end())
	{
		// cout<<"Error: cant find the author of this tweet in the database"<<endl;
		// throw NoneExistError();
	}

	Tweet* newTweet = new Tweet(it->second, time, text);

	//fill the hashtagMap
	stringstream ss_ht(text);
	string garbage, hashtag;
	while(getline(ss_ht, garbage, '#'))
	{
		if(ss_ht.fail())
			break;
		ss_ht>>hashtag;
		if(hashtag=="")
			break;

		map<string, set<Tweet*> >::iterator it_htmap = hashtagMap.find(toUpper(hashtag));

		// cout<<" = = = =" <<endl;

		//if the hastag already in file, add to the set, else create a pair and add
		if(it_htmap==hashtagMap.end())
		{
			set<Tweet*> set_htmap_tweet;
			set_htmap_tweet.insert(newTweet);
			hashtagMap.insert(pair<string, set<Tweet*> >(toUpper(hashtag), set_htmap_tweet));

			//hw8
			TagData newTag(toUpper(hashtag), 1);
			trending_.push(newTag);
			// cout<<"run 1st for: "<<hashtag<<endl;

		}else{
			//hw8
			TagData oldTag(toUpper(hashtag), it_htmap->second.size());

			//original
			(it_htmap->second).insert(newTweet);

			//hw8
			TagData newTag(toUpper(hashtag), it_htmap->second.size());
			trending_.decreaseKey(oldTag, newTag);

			// cout<<"run 2st for: "<<hashtag<<endl;
		}
	}

		
}

vector<Tweet*> TwitEng::search(vector<string>& terms, int strategy)
{
	vector<Tweet*> result;

	if(strategy==0||strategy==1)
	{
		set<Tweet*> intersection;

		for(int unsigned i=0;i<terms.size();i++)
		{
			//for each term in the given vector
			string term = terms[i];

			term = toUpper(term);

			map<string, set<Tweet*> >::iterator it, it_find;
		  it_find = hashtagMap.find(term);
		  if(it_find!=hashtagMap.end())
		  {
		  	//if found
				//combine the new set with the existing result
				if(intersection.size()==0)
				{
					intersection = it_find->second;
				}else{
					set<Tweet*> temp;
					if(strategy==0)
					{
						set_intersection(intersection.begin(), intersection.end(), it_find->second.begin(), it_find->second.end(), inserter(temp, temp.begin()));
					}else
					{
						set_union(intersection.begin(), intersection.end(), it_find->second.begin(), it_find->second.end(), inserter(temp, temp.begin()));
					}
					
					intersection = temp;
				}		  	
		  }
		}

		result.resize(intersection.size());
		copy(intersection.begin(),intersection.end(),result.begin());
	}else{
		// throw StrategyError();
	}
	return result;
	//check if already in the vector
}

string TwitEng::toUpper(string term)
{
	string temp = term;
	convUpper(temp);
	return temp;
	// for(int i=0; i<term.length(); i++)
	// {
	// 	term[i] = toupper(term[i]);
	// }
	// return term;
}

void TwitEng::dumpFeeds()
{
	map<string, User*>::iterator it_usermap;

	for(it_usermap=userMap.begin(); it_usermap!=userMap.end(); ++it_usermap)
	{
		string file = it_usermap->first+".feed";
		const char *filename = file.c_str();
		ofstream ofile(filename);

		ofile<<it_usermap->first<<endl;

		//all the tweets for each user
		vector<Tweet*> userFeed = it_usermap->second->getFeed();
		for(int unsigned i=0;i<userFeed.size();i++)
		{
			ofile<<*(userFeed[i])<<endl;
		}
	}
}

//hw5
User* TwitEng::searchUser(std::string search_username)
{
	std::map<std::string, User* >::iterator it = (userMap).find(search_username);
	if(it!=userMap.end())
	{
		return it->second;
	}else
	{
		return NULL;
	}
}

void TwitEng::storeData(string filename)
{
	ofstream ofile(filename);

	int size_ = userMap.size();
	ofile<<size_<<endl;
	for(map<string, User*>::iterator it = userMap.begin(); it!=userMap.end(); ++it)
	{
		ofile<<it->second->name()<<" "<<it->second->getHashedPW();
		//hw8 TO-DO

		//feeding followings
		set<User*> set_following = it->second->following();
		for(set<User*>::iterator it2 = set_following.begin(); it2 != set_following.end(); ++it2)
		{
			ofile<<" "<<(*it2)->name();
		}
		ofile<<endl;
	}

	for(map<string, User*>::iterator it = userMap.begin(); it!=userMap.end(); ++it)
	{
		list<Tweet*> thisUserTweetList = it->second->tweets();
		for(list<Tweet*>::iterator it2 = thisUserTweetList.begin(); it2!=thisUserTweetList.end(); ++it2)
		{
			ofile<<(**it2)<<endl;
		}
	}

}

//hw8
void TwitEng::trendingFeed(int n)
{	
	Heap<TagData, TagStringEqual, TagIntGreater, TagStringHasher> copy_;

	for(int i=0;i<trending_.size();i++)
	{
		copy_.push(trending_.get(i));
	}

	// cout<<"trending size: "<<trending_.size()<<endl;
	if(n>trending_.size())
		n = trending_.size();
	
	for(int i=0;i<n;i++)
	{
		TagData temp = copy_.top();
		string thisTag = temp.tag;
		convLower(thisTag);
		cout<<thisTag<<" : "<<temp.num<<endl;
		copy_.pop();
	}

}

bool TwitEng::login(std::string username, std::string password)
{
	map<string, User*>::iterator it = userMap.find(username);

	if(it!=userMap.end())
	{
		unsigned long long match = calculateHash(password);
		if(it->second->canLogin(match))
		{
			currentSession_ = it->second;
			return true;
		}else
		{
			return false;
		}

	}else
	{
		return false;
	}
}

bool TwitEng::logout()
{
	if(currentSession_==NULL)
	{
		return false;
	}else
	{
		currentSession_ = NULL;
		return true;
	}
}

User* TwitEng::authState()
{
	return currentSession_;
}



