#include "cmdhandler.h"
#include "util.h"
#include "stdexcept"
#include "twiteng.h"
#include <stdexcept>
using namespace std;


//QUIT HANDLER
QuitHandler::QuitHandler()
{
	
}

QuitHandler::QuitHandler(Handler* next)
  : Handler(next)
{

}

bool QuitHandler::canHandle(const std::string& cmd) const
{
	return cmd == "QUIT";
}

Handler::HANDLER_STATUS_T QuitHandler::process(TwitEng* eng, std::istream& instr) const
{
	eng->dumpFeeds();
	return HANDLER_QUIT;
}

//AND HANDLER
AndHandler::AndHandler()
{

}

AndHandler::AndHandler(Handler* next)
	: Handler(next)
{
	
}

bool AndHandler::canHandle(const std::string& cmd) const
{
	return cmd == "AND";
}

Handler::HANDLER_STATUS_T AndHandler::process(TwitEng* eng, std::istream& instr) const
{
	std::vector<std::string> terms;
	std::string word;
	while(instr>>word)
	{
		terms.push_back(word);
	}
	std::vector<Tweet*> result;
	result = eng->search(terms, 0);

	if(result.size()==0)
	{
		std::cout<<"No matches."<<std::endl;
	}else
	{
		std::cout<<result.size()<<" matches:"<<std::endl;
		for(int unsigned i=0;i<result.size();i++)
		{
			std::cout<<*(result[i])<<std::endl;
		}
	}
	return HANDLER_OK;
}

//OR HANDLER
OrHandler::OrHandler()
{

}

OrHandler::OrHandler(Handler* next)
	: Handler(next)
{

}

bool OrHandler::canHandle(const std::string& cmd) const
{
	return cmd == "OR";
}

Handler::HANDLER_STATUS_T OrHandler::process(TwitEng* eng, std::istream& instr) const
{
	std::vector<std::string> terms;
	std::string word;
	while(instr>>word)
	{
		terms.push_back(word);
	}
	std::vector<Tweet*> result;
	result = eng->search(terms, 1);

	if(result.size()==0)
	{
		std::cout<<"No matches."<<std::endl;
	}else
	{
		std::cout<<result.size()<<" matches:"<<std::endl;
		for(int unsigned i=0;i<result.size();i++)
		{
			std::cout<<*(result[i])<<std::endl;
		}
	}

	return HANDLER_OK;
}

//TweetHandler
TweetHandler::TweetHandler(){}

TweetHandler::TweetHandler(Handler* next)
	:Handler(next)
{

}

bool TweetHandler::canHandle(const std::string& cmd) const
{
	return cmd == "TWEET";
}

Handler::HANDLER_STATUS_T TweetHandler::process(TwitEng* eng, std::istream& instr) const
{
	User* thisUser = eng->authState();
	if(thisUser!=NULL)
	{
		std::string tweet_text;;
		// instr>>username;
		string temp;
		instr>>temp;
		getline(instr, tweet_text);
		tweet_text = temp + tweet_text;
		DateTime thisTime;

		eng->addTweet(thisUser->name(), thisTime, tweet_text);
		// try{
			
		// }
		// catch(NoneExistError& e)
		// {
		// 	std::cout<<"cant find the author you want to add tweet to"<<std::endl;
		// }

		return HANDLER_OK;
	}else
	{
		return HANDLER_ERROR;
	}
}

//hw5
FollowHandler::FollowHandler(){}
FollowHandler::FollowHandler(Handler* next)
	:Handler(next)
{

}
bool FollowHandler::canHandle(const std::string& cmd) const
{
	return cmd == "FOLLOW";
}
Handler::HANDLER_STATUS_T FollowHandler::process(TwitEng* eng, std::istream& instr) const
{
	User* currentSession = eng->authState();
	if(currentSession==NULL)
		return HANDLER_ERROR;

	else{
		string name_to_follow;
	
		if(instr>>name_to_follow)
		{
			User* it_follower = currentSession;
			User* it_follow = eng->searchUser(name_to_follow);

			if(it_follow!=NULL)
			{
				(it_follower)->addFollowing(it_follow);
				(it_follow)->addFollower(it_follower);
				return HANDLER_OK;
			}else
			{
				return HANDLER_ERROR;
			}
		}else
		{
			return HANDLER_ERROR;
		}
	}
}

// TO-DO : finish the other handler
SaveHandler::SaveHandler(){}
SaveHandler::SaveHandler(Handler* next)
	:Handler(next)
{

}
bool SaveHandler::canHandle(const std::string& cmd) const
{
	return cmd == "SAVE";
}
Handler::HANDLER_STATUS_T SaveHandler::process(TwitEng* eng, std::istream& instr) const
{
	string filename;
	if(instr>>filename)
	{
		eng->storeData(filename);
		return HANDLER_OK;
	}else
	{
		return HANDLER_ERROR;
	}
}

//TrendingHandler

TrendingHandler::TrendingHandler(){}

TrendingHandler::TrendingHandler(Handler* next)
	:Handler(next)
{

}

bool TrendingHandler::canHandle(const std::string& cmd) const
{
	return cmd == "TRENDING";
}

Handler::HANDLER_STATUS_T TrendingHandler::process(TwitEng* eng, std::istream& instr) const
{
	int rankingRange;
	instr>>rankingRange;
	if(!instr.fail())
	{
		// cout<<"GOOD!:"<<endl;

		eng->trendingFeed(rankingRange);
		return HANDLER_OK;
	}else
	{
		// cout<<"fine"<<endl;
		return HANDLER_ERROR;
	}
}

//LoginHandler
LoginHandler::LoginHandler(){}
LoginHandler::LoginHandler(Handler* next)
	:Handler(next)
{

}

bool LoginHandler::canHandle(const std::string& cmd) const
{
	return cmd == "LOGIN";
}

AndHandler::HANDLER_STATUS_T LoginHandler::process(TwitEng* eng, std::istream& instr) const
{
	string username, password;

	if(instr>>username>>password)
	{
		bool login_result = eng->login(username, password);

		if(login_result)
		{
			cout<<"Login successful."<<endl;
			return HANDLER_OK;
		}else
		{
			cout<<"Invalid username/password."<<endl;
			return HANDLER_ERROR;			
		}

	}else
	{
		return HANDLER_ERROR;
	}
}

//LoginHandler
LogoutHandler::LogoutHandler(){}

LogoutHandler::LogoutHandler(Handler* next)
	:Handler(next)
{

}

bool LogoutHandler::canHandle(const std::string& cmd) const
{
	return cmd == "LOGOUT";
}

AndHandler::HANDLER_STATUS_T LogoutHandler::process(TwitEng* eng, std::istream& instr) const
{
	if(eng->logout())
	{
		return HANDLER_OK;
		cout<<"Logout successful."<<endl;
	}else
	{
		//not logged in, how to log out
		cout<<"Not logged in yet."<<endl;
		return HANDLER_ERROR;
	}
	
}


