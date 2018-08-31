#ifndef TWITENG_H
#define TWITENG_H
#include <map>
#include <string>
#include <set>
#include <vector>
#include "user.h"
#include "datetime.h"
#include "tagdata.h"
#include "heap.h"
#include "hash.h"

class TwitEng
{
 public:
  TwitEng();
  ~TwitEng();
  /**
   * Parses the Twitter database and populates internal structures
   * @param filename of the database file
   * @return true if there is an error, false if successful
   */
  bool parse(char* filename);

  /**
   * Allocates a tweet from its parts and adds it to internal structures
   * @param username of the user who made the tweet
   * @param timestamp of the tweet
   * @param text is the actual text of the tweet as a single string
   */
  void addTweet(const std::string& username, const DateTime& time, const std::string& text);

  /**
   * Searches for tweets with the given words and strategy
   * @param words is the hashtag terms in any case without the '#'
   * @param strategy 0=AND, 1=OR
   * @return the tweets that match the search
   */
  std::vector<Tweet*> search(std::vector<std::string>& terms, int strategy);

  /**
   * Dump feeds of each user to their own file
   */
  void dumpFeeds();

  /* You may add other member functions */
  User* searchUser(std::string search_username);
  void trendingFeed(int n);
  void storeData(std::string filename);
  bool login(std::string username, std::string password);
  bool logout();

  User* authState();
 private:
  /* Add any other data members or helper functions here  */
  std::map<std::string, User*> userMap;
  std::map<std::string, std::set<Tweet*> > hashtagMap; //every hashtag capitalized

  std::string toUpper(std::string term);

  Heap<TagData, TagStringEqual, TagIntGreater, TagStringHasher> trending_;
  User* currentSession_;
};

#endif