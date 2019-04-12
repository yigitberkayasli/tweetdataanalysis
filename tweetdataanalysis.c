#include "avltree.h"

AVLTree read_tweet_data(char *);
AVLTree insert_hashtag(char *, int, AVLTree);
AVLTree fill_hashtag_database(AVLTree, AVLTree);
void display_the_most_trending_hashtag(AVLTree);
void display_index(AVLTree);

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("usage: %s filename\n", argv[0]);
		exit(1); // arg error
	}
	
	AVLTree tweets = read_tweet_data(argv[1]);
	AVLTree hashtags = CreateTree();
	hashtags = MakeEmptyTree(hashtags);
	hashtags = fill_hashtag_database(tweets, hashtags);

	int selection;
	while (1)
	{
		system("cls");
		printf("Welcome to Hashtag Analysis at Twitter.\n");
		printf("\nMenu\n");
		
		printf("1. Display the hashtag index\n");
		printf("2. Display the most trending hashtag\n");
		printf("3. Exit\n");
		printf("Option: ");
		scanf("%d", &selection);
		
		switch(selection)
		{
			case 1:
				display_index(hashtags);
				break;
			case 2:
				display_the_most_trending_hashtag(hashtags);
				break;
			case 3:
				exit(0);
				break;
			case 4:
				DisplayTweetTreeStructure(tweets, 0);
				break;
			default:
				printf("Command not recognized!\n");
				break;
		}
		
		system("pause");
	}
		
	return 0;
}

int checkFile(char fileName[])
{
	FILE *txt = fopen(fileName, "r");
	
	if (txt == NULL) // file is not present
		return 1;
		
	char firstLine;
	fscanf(txt, "%c", &firstLine);
		
	if (firstLine == 0) // file is empty
	{
		fclose(txt);
		return 1;
	}
	
	fclose(txt);
	
	return 0;
}

AVLTree read_tweet_data(char fileName[])
{
	if (checkFile(fileName))
	{
		printf("Error while opening %s!\n", fileName);
		exit(2); // file error
	}
	
	FILE *txt = fopen(fileName, "r");
	
	AVLTree tweets = CreateTree();
	tweets = MakeEmptyTree(tweets);
	Tweet temp_tweet;
	
	char *eof_status;
	
	// getting the first tweet
	temp_tweet = (Tweet) malloc (sizeof(struct TweetNode));
	fscanf(txt,"%d	", &temp_tweet->id);
	eof_status = fgets(temp_tweet->tweet_content, TWEET_LENGTH, txt); // fgets returns a NULL pointer if it reaches the EOF
	temp_tweet->tweet_content[strlen(temp_tweet->tweet_content) - 1] = '\0';
	tweets = InsertTweet(temp_tweet, tweets);
	
	// getting the rest
	while (eof_status != NULL)
	{
		temp_tweet = (Tweet) malloc (sizeof(struct TweetNode));
		fscanf(txt,"%d	", &temp_tweet->id);
		eof_status = fgets(temp_tweet->tweet_content, TWEET_LENGTH, txt);
		temp_tweet->tweet_content[strlen(temp_tweet->tweet_content) - 1] = '\0';
		if (eof_status != NULL)
			tweets = InsertTweet(temp_tweet, tweets);
	}
	
	return tweets;
}

AVLTree insert_hashtag(char hashtag[], int tweet_id, AVLTree t)
{	
	return InsertHashtag(hashtag, tweet_id, t);
}

AVLTree fill_hashtag_database(AVLTree tweets, AVLTree hashtags)
{	
	if (tweets != NULL)
	{
		hashtags = fill_hashtag_database(tweets->left, hashtags);
	
		int i = 0;
		int k;
		
		char temp_hashtag[HASHTAG_LENGTH];
		
		while(tweets->tweet->tweet_content[i] != '\0')
		{
			if (tweets->tweet->tweet_content[i] == '#')
			{
				i++;
				k = 0;
				while ((tweets->tweet->tweet_content[i] >= '0' && tweets->tweet->tweet_content[i] <= '9') || (tweets->tweet->tweet_content[i] >= 'A' && tweets->tweet->tweet_content[i] <= 'Z') || (tweets->tweet->tweet_content[i] >= 'a' && tweets->tweet->tweet_content[i] <= 'z'))
				{
					temp_hashtag[k] = tweets->tweet->tweet_content[i];
					k++; i++;
				}
				temp_hashtag[k] = '\0';
				
				hashtags = insert_hashtag(temp_hashtag, tweets->tweet->id, hashtags);
			}
			i++;
		}
	
		hashtags = fill_hashtag_database(tweets->right, hashtags);
	}
	
	return hashtags;
}

void display_index(AVLTree hashtags)
{
	DisplayHashtagTree(hashtags);
}

void display_the_most_trending_hashtag(AVLTree hashtags)// O(n^2)
{
	HashtagList hashtagsWithMostID = find_the_most_trending_hashtag(hashtags, NULL); // O(n)
	
	printf("The most trending hashtag\n");// O(1)
	printf("-------------------------\n");// O(1)
	
	while (hashtagsWithMostID != NULL)// O(n)
	{
		tweetIDDatabase temp = hashtagsWithMostID->hashtagWithMost->IDs;// O(n)
		printf("%s (Tweets: %d", hashtagsWithMostID->hashtagWithMost->hashtag_content, hashtagsWithMostID->hashtagWithMost->IDs->id);// O(n)
		temp = temp->next;// O(n)
		while (temp != NULL)// O(n^2)
		{
			printf(",%d", temp->id);// O(n^2)
			temp = temp->next;// O(n^2)
		}
		printf(") (ID count: %d)\n", hashtagsWithMostID->hashtagWithMost->ID_count);// O(n)
		
		hashtagsWithMostID = hashtagsWithMostID->next;// O(n)
	}
	
}
