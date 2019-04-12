#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "linked_list.h"

AVLTree CreateTree(void);
AVLTree MakeEmptyTree(AVLTree);
AVLTree InsertTweet(Tweet, AVLTree);
AVLTree SingleRotateWithLeft(AVLTree);
AVLTree SingleRotateWithRight(AVLTree);
AVLTree DoubleRotateWithLeft(AVLTree);
AVLTree DoubleRotateWithRight(AVLTree);
void DisplayTweetTree(AVLTree);
void DisplayTweetTreeStructure(AVLTree, int);
int AVLTreeHeight(AVLTree);
int Max(int, int);

AVLTree CreateTree(void)
{
	return NULL;
}

AVLTree MakeEmptyTree(AVLTree t)
{
	if (t != NULL)
	{
		MakeEmptyTree(t->left);
		MakeEmptyTree(t->right);
		free(t);
	}
	return NULL;
}

AVLTree InsertTweet(Tweet tweet_to, AVLTree t)
{	
	if (t == NULL)
	{
		
		t = (AVLTree) malloc(sizeof(struct MainNode));
		if (t == NULL)
			printf("Out of memory space!!!\n");
		else
		{
			t->tweet = tweet_to;
			t->hashtag = NULL;
			t->height = 0;
			t->left = t->right = NULL;
		}
	}
	else if (tweet_to->id < t->tweet->id)
	{
		t->left = InsertTweet(tweet_to, t->left);

		if (AVLTreeHeight(t->left) - AVLTreeHeight(t->right) == 2)
			if (tweet_to->id < t->left->tweet->id)
				t = SingleRotateWithLeft(t);
			else
				t = DoubleRotateWithLeft(t);
	}
	else if (tweet_to->id > t->tweet->id)
	{
		t->right = InsertTweet(tweet_to, t->right);
		if (AVLTreeHeight(t->right) - AVLTreeHeight(t->left) == 2)
			if (tweet_to->id > t->right->tweet->id)
				t = SingleRotateWithRight(t);
			else
				t = DoubleRotateWithRight(t);
	}
	/* else id is in the tree already ... do nothing */
	t->height = Max(AVLTreeHeight(t->left), AVLTreeHeight(t->right)) + 1;

	return t;
}

AVLTree InsertHashtag(char hashtag[], int tweet_id, AVLTree t)
{	
	//make it lowercase
	int i;
	for(i = 0; hashtag[i]; i++){
		hashtag[i] = tolower(hashtag[i]);
	}
	
	if (t == NULL)
	{
		t = (AVLTree) malloc(sizeof(struct MainNode));
		if (t == NULL)
			printf("Out of memory space!!!\n");
		else
		{
			t->hashtag = (HashTag) malloc(sizeof(struct HashTagNode));
			t->hashtag->IDs = NULL;
			strcpy(t->hashtag->hashtag_content, hashtag);
			t->hashtag->IDs = InsertHashtagID(tweet_id, t->hashtag->IDs, &t->hashtag->ID_count);
			t->height = 0;
			t->tweet = NULL;
			t->left = t->right = NULL;
		}
	}
	else if (strcmp(hashtag, t->hashtag->hashtag_content) == 0)
	{
		t->hashtag->IDs = InsertHashtagID(tweet_id, t->hashtag->IDs, &t->hashtag->ID_count);
	}
	else if (strcmp(hashtag, t->hashtag->hashtag_content) < 0)
	{
		t->left = InsertHashtag(hashtag, tweet_id, t->left);

		if (AVLTreeHeight(t->left) - AVLTreeHeight(t->right) == 2)
			if (strcmp(hashtag, t->left->hashtag->hashtag_content) < 0)
				t = SingleRotateWithLeft(t);
			else
				t = DoubleRotateWithLeft(t);
	}
	else if (strcmp(hashtag, t->hashtag->hashtag_content) > 0)
	{
		t->right = InsertHashtag(hashtag, tweet_id, t->right);

		if (AVLTreeHeight(t->right) - AVLTreeHeight(t->left) == 2)
			if (strcmp(hashtag, t->right->hashtag->hashtag_content) > 0)
				t = SingleRotateWithRight(t);
			else
				t = DoubleRotateWithRight(t);
	}
	t->height = Max(AVLTreeHeight(t->left), AVLTreeHeight(t->right)) + 1;

	return t;
}

void DisplayTweetTree(AVLTree t)
{
	if (t != NULL)
	{
		DisplayTweetTree(t->left);
		printf("ID: %d\n", t->tweet->id);
		puts(t->tweet->tweet_content);
		DisplayTweetTree(t->right);
	}
}

void DisplayHashtagTree(AVLTree t)
{
	if (t != NULL)
	{
		DisplayHashtagTree(t->left);
		
		tweetIDDatabase temp = t->hashtag->IDs;
		printf("%s (Tweets: %d", t->hashtag->hashtag_content, t->hashtag->IDs->id);
		temp = temp->next;
		while (temp != NULL)
		{
			printf(",%d", temp->id);
			temp = temp->next;
		}
		printf(") (ID count: %d)\n", t->hashtag->ID_count);
		DisplayHashtagTree(t->right);
	}
}

void DisplayTweetTreeStructure(AVLTree t, int depth)
{
	int i;

	if (t != NULL)
	{
		DisplayTweetTreeStructure(t->right, depth + 1);

		for (i = 0; i < depth; i++)
			printf("     ");
		printf("%d\n", t->tweet->id);

		DisplayTweetTreeStructure(t->left, depth + 1);
	}
}

void DisplayHashtagTreeStructure(AVLTree t, int depth)
{
	int i;

	if (t != NULL)
	{
		DisplayHashtagTreeStructure(t->right, depth + 1);

		for (i = 0; i < depth; i++)
			printf("     ");
		puts(t->hashtag->hashtag_content);

		DisplayHashtagTreeStructure(t->left, depth + 1);
	}
}

int AVLTreeHeight(AVLTree t)
{
	if (t == NULL)
		return -1;
	else
		return t->height;
}

AVLTree SingleRotateWithLeft(AVLTree k2)
{
	AVLTree k1;
	k1 = k2->left;
	k2->left = k1->right;
	k1->right = k2;
	k2->height = Max(AVLTreeHeight(k2->left), AVLTreeHeight(k2->right)) + 1;
	k1->height = Max(AVLTreeHeight(k1->left), AVLTreeHeight(k1->right)) + 1;
	return k1;
}

AVLTree SingleRotateWithRight(AVLTree k2)
{
	AVLTree k1;
	k1 = k2->right;
	k2->right = k1->left;
	k1->left = k2;
	k2->height = Max(AVLTreeHeight(k2->left), AVLTreeHeight(k2->right)) + 1;
	k1->height = Max(AVLTreeHeight(k1->left), AVLTreeHeight(k1->right)) + 1;
	return k1;
}

AVLTree DoubleRotateWithLeft(AVLTree k3)
{
	k3->left = SingleRotateWithRight(k3->left);
	k3 = SingleRotateWithLeft(k3);
	return k3;
}

AVLTree DoubleRotateWithRight(AVLTree k3)
{
	k3->right = SingleRotateWithLeft(k3->right);
	k3 = SingleRotateWithRight(k3);
	return k3;
}

int Max(int x, int y)
{
	if (x >= y)
		return x;
	else
		return y;
}
