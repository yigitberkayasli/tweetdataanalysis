#include "structs.h"

tweetIDDatabase InsertHashtagID(int ID, tweetIDDatabase t, int *ID_count)
{
	int is_present = 0;
	
	if (t == NULL)
	{
		t = (tweetIDDatabase) malloc(sizeof(struct TweetIDStore));
		t->id = ID;
		*ID_count = 1;
		t->next = NULL;
	} else
	{
		tweetIDDatabase temp = t;
		while (1)
		{
			if (temp->id == ID)
				is_present = 1;
				
			if (temp->next == NULL)
				break;
			temp = temp->next;
		}
		if (!is_present)
		{
			temp->next = (tweetIDDatabase) malloc(sizeof(struct TweetIDStore));
			temp = temp->next;
			temp->id = ID;
			temp->next = NULL;
			*ID_count = *ID_count + 1;
		}
	}
	
	return t;
}


void freeHashtagList(HashtagList hashtags)
{
	if (hashtags->next != NULL)
		freeHashtagList(hashtags->next);
	free(hashtags);
}

HashtagList find_the_most_trending_hashtag(AVLTree hashtags, HashtagList hashtagsWithMostID)
{	
	if (hashtags != NULL)
	{
		hashtagsWithMostID = find_the_most_trending_hashtag(hashtags->left, hashtagsWithMostID);
		
		if (hashtagsWithMostID == NULL)
		{
			hashtagsWithMostID = (HashtagList) malloc(sizeof(struct LinkedListHashtag));
			hashtagsWithMostID->hashtagWithMost = hashtags->hashtag;
			hashtagsWithMostID->next = NULL;
		} else
		{
			if (hashtags->hashtag->ID_count == hashtagsWithMostID->hashtagWithMost->ID_count)
			{
				HashtagList temp = hashtagsWithMostID;
				
				while (temp->next != NULL)
					temp = temp->next;
					
				temp->next = (HashtagList) malloc(sizeof(struct LinkedListHashtag));
				temp = temp->next;
				temp->hashtagWithMost = hashtags->hashtag;
				temp->next = NULL;
			} else if (hashtags->hashtag->ID_count > hashtagsWithMostID->hashtagWithMost->ID_count)
			{
				freeHashtagList(hashtagsWithMostID);
				hashtagsWithMostID = (HashtagList) malloc(sizeof(struct LinkedListHashtag));
				hashtagsWithMostID->hashtagWithMost = hashtags->hashtag;
				hashtagsWithMostID->next = NULL;
			}
		}
		
		hashtagsWithMostID = find_the_most_trending_hashtag(hashtags->right, hashtagsWithMostID);	
	}
	
	return hashtagsWithMostID;
}
