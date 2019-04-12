#define TWEET_LENGTH 280
#define HASHTAG_LENGTH 30

typedef struct TweetIDStore *tweetIDDatabase;
struct TweetIDStore {
	int id;
	tweetIDDatabase next;
};

typedef struct HashTagNode *HashTag;
struct HashTagNode {
	tweetIDDatabase IDs;
	int ID_count;
	char hashtag_content[HASHTAG_LENGTH];
};

typedef struct LinkedListHashtag *HashtagList;
struct LinkedListHashtag{
	HashTag hashtagWithMost;
	HashtagList next;
};

typedef struct TweetNode *Tweet;
struct TweetNode {
	int id;
	char tweet_content[TWEET_LENGTH];
};

typedef struct MainNode *AVLTree;
struct MainNode {
	Tweet tweet;
	HashTag hashtag;
	AVLTree left;
	AVLTree right;
	int height;
};
