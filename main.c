#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NELEM 1500
#define BIG 10000

int total_user_num=0;
int total_friendship_record=0;
int total_tweet=0;

typedef struct Friend
{
    struct User* user;
    struct Friend* next;
} Friend;

Friend* newFriend()
{
    Friend* f=(Friend*)malloc(sizeof(Friend));
    f->next=NULL;
    f->user=NULL;

    return f;
}

typedef struct Word
{
    char date[33];
    char word[30];

    struct User* writer;
    struct Word* next;
    struct Word* prev;
} Word;

typedef struct User
{
    char id_num[12];
    char date[33];
    char name[15];
    int tweet_num;

    struct User* next;
    struct User* prev;
    Friend* friends;
} User;

typedef struct List
{
    char word[100];
    int n;

    struct List* prev;
    struct List* next;
} List;

Word *word_head, *word_tail;
User *user_head, *user_tail;
List *list_head, *list_tail;

void List_init()
{
    list_head = (List*)malloc(sizeof(List));
    list_tail = (List*)malloc(sizeof(List));
    list_head->next=list_tail;
    list_tail->prev=list_head;
    list_head->prev=NULL;
    list_tail->next=NULL;
}

void Word_init()
{
    word_head = (Word*)malloc(sizeof(Word));
    word_tail = (Word*)malloc(sizeof(Word));
    word_head->next = word_tail;
    word_tail->prev = word_head;
    word_head->prev = NULL;
    word_tail->next = NULL;
}

List* newList(char word[])
{
    List* list = (List*)malloc(sizeof(List));
    strcpy(list->word, word);
    list->n=0;
    list->prev = list_head;
    list->next=list_head->next;
    list_head->next=list;

    return list;
}

Word* newWord(char id_num[], char date[], char text[])
{
    Word* word = (Word*)malloc(sizeof(Word));
    strcpy(word->date, date);
    strcpy(word->word, text);
    word->prev = word_head;
    word->next = word_head->next;
    word_head->next = word;

    User* a;
    for(a=user_head->next; a!=NULL; a=a->next)
    {
        if(!strcmp(a->id_num, id_num))
        {
            word->writer=a;
            a->tweet_num++;
            break;
        }
    }

    return word;
}

User* newUser(char id_num[], char date[], char name[])
{
    User* user = (User*)malloc(sizeof(User));
    Friend* f = newFriend();
    strcpy(user->id_num, id_num);
    strcpy(user->date, date);
    strcpy(user->name, name);
    user->prev = user_head;
    user->next = user_head->next;
    user_head->next = user;
    user->friends = f;
    user->tweet_num=0;

    return user;
}

void User_init()
{
    user_head = (User*)malloc(sizeof(User));
    user_tail = (User*)malloc(sizeof(User));

    user_head->next = user_tail;
    user_tail->prev = user_head;
    user_head->prev = NULL;
    user_tail->next = NULL;

    user_head->friends = NULL;
    user_tail->friends = NULL;
}

void RoadFile_user(char user[])
{
    char id_num[12];
    char date[35];
    char name[20];
    char buf[2];
    char box[100];
    int count=0;

    User_init();

    FILE * fp_user = fopen(user, "rt");
    if(fp_user==NULL)
    {
        printf("fail(user)\n");
        return -1;
    }

    while(fgets(box, sizeof(box), fp_user) != NULL) count++;

    total_user_num=count;

    rewind(fp_user);

    int i;
    for(i=0; i<count/4; i++)
    {
        strcpy(id_num, fgets(id_num, sizeof(id_num), fp_user));
        strcpy(date, fgets(date, sizeof(date), fp_user));
        strcpy(name, fgets(name, sizeof(name), fp_user));
        strcpy(buf, fgets(buf, sizeof(buf), fp_user));
        newUser(id_num, date, name);
    }

    fclose(fp_user);
}

void RoadFile_friend(char file[])
{
    char id_1[12];
    char id_2[12];
    char buf[2];
    char box[100];
    int count=0;

    FILE * fp_friend = fopen(file, "rt");

    if(fp_friend==NULL)
    {
        printf("fail(friend)\n");
        return -1;
    }

    while(fgets(box, sizeof(box), fp_friend) != NULL) count++;

    rewind(fp_friend);

    total_friendship_record=count/3;
    int i;
    for(i=0; i<count/3; i++)
    {
        strcpy(id_1, fgets(id_1, sizeof(id_1), fp_friend));
        strcpy(id_2, fgets(id_2, sizeof(id_2), fp_friend));
        strcpy(buf, fgets(buf, sizeof(buf), fp_friend));

        User* a;
        User* b;
        for(a=user_head->next; a->next!=NULL; a=a->next)
        {
            if(!strcmp(a->id_num, id_1))
                break;
        }

        for(b=user_head->next; b->next!=NULL; b=b->next)
        {
            if(!strcmp(b->id_num, id_2))
                break;
        }


        Friend* f1=newFriend();
        Friend* f2=newFriend();

        f1->user=b;
        f2->user=a;

        f1->next=a->friends;
        a->friends=f1;

        f2->next=b->friends;
        b->friends=f2;
    }
    fclose(fp_friend);
}

void RoadFile_word(char word[])
{
    char id_num[12];
    char date[35];
    char text[30];
    char buf[2];
    char box[100];
    int count=0;

    Word_init();

    FILE * fp_word = fopen(word, "rt");

    if(fp_word==NULL)
    {
        printf("fail(word)\n");
        return -1;
    }

    while(fgets(box, sizeof(box), fp_word) != NULL) count++;

    total_tweet=count/4;

    rewind(fp_word);

    int i;
    for(i=0; i<count/4; i++)
    {
        strcpy(id_num, fgets(id_num, sizeof(id_num), fp_word));
        strcpy(date, fgets(date, sizeof(date), fp_word));
        strcpy(text, fgets(text, sizeof(text), fp_word));
        strcpy(buf, fgets(buf, sizeof(buf), fp_word));
        newWord(id_num, date, text);
    }

    fclose(fp_word);
}

int getMinFriendNum()
{
    int min=BIG;


    User* a;
    for(a=user_head->next; a->next!=NULL; a=a->next)
    {
        int num=0;
        Friend* f;

        for(f=a->friends; f->next!=NULL; f=f->next)
        {
            num ++;
        }

        if(num<min)
            min = num;
    }

    return min;
}

int getMaxFriendNum()
{
    int max=0;

    User* a;
    for(a=user_head->next; a->next!=NULL; a=a->next)
    {
        int num=0;
        Friend* f;

        for(f=a->friends; f->next!=NULL; f=f->next)
        {
            num ++;
        }

        if(num>max)
            max = num;
    }

    return max;
}

int MinTweet()
{
    int min=BIG;

    User* a;
    for(a=user_head->next; a->next!=NULL; a=a->next)
    {
        if(a->tweet_num<min)
            min=a->tweet_num;
    }

    return min;
}

int MaxTweet()
{
    int max=0;

    User* a;
    for(a=user_head->next; a!=NULL; a=a->next)
    {
        if(a->tweet_num>max)
            max=a->tweet_num;
    }

    return max;
}

int main()
{
    int order;

    RoadFile_user("user.txt");
    RoadFile_friend("friend.txt");
    RoadFile_word("word.txt");

    while(1)
    {
        printf("---------------------------------------------------------\n");
        printf("0. Read data files\n");
        printf("1. Display statistics\n");
        printf("2. Top 5 most tweeted words\n");
        printf("3. Top 5 most tweeted users\n");
        printf("4. Find users who tweeted a word\n");
        printf("5. Find all people who are friends of the above users\n");
        printf("6. Delete all mentions of a word\n");
        printf("7. Delete all users who mentioned a word\n");
        printf("8. Find strongly connected components\n");
        printf("9. Find shortest path from a given user\n");
        printf("99. Quit\n");
        printf("---------------------------------------------------------\n");
        printf("Select Menu : ");

        scanf("%d", &order);
        if(order==0)
        {
            printf("Total users: %d\n", total_user_num);
            printf("Total friendship records: %d\n", total_friendship_record);
            printf("Total tweets: %d\n", total_tweet);
        }
        else if(order==1)
        {
            printf("Average number of friends: %f\n", (double)(total_friendship_record*2.0)/total_user_num);
            printf("Minimum friends: %d\n", getMinFriendNum());
            printf("Maxmum number of friends: %d\n", getMaxFriendNum());
            printf("\n");
            printf("Average tweets per user: %f\n", (double)((double)total_tweet/(double)total_user_num));
            printf("Minium tweets per user: %d\n", MinTweet());
            printf("Maximu tweets per user: %d\n", MaxTweet());
        }
        else if(order==2)
        {
            List_init();
            int i=0;

            Word* a;
            for(a=word_head->next; a->next!=NULL; a=a->next)
            {
                List* l;
                for(l=list_head->next; l->next!=NULL; l=l->next)
                {
                    if(!strcmp(l->word, a->word))
                    {
                        l->n++;
                        break;
                    }
                    else
                    {
                        List* new_l = newList(a->word);
                        new_l->n++;
                        i++;
                    }
                }
            }

            /*List* x;
            int t, min=BIG;
            List* minList = newList("NULL");
            for(t=0; t<i; t++)
            {
                for(x=list_head->next; x->next!=NULL; x=x->next)
                {
                    if(x->n<min)
                    {
                        x->prev->next=x->next;
                        x->next->prev=x->prev;
                        x->next=list_tail->next;
                        x->prev=list_tail->prev;
                        list_tail->prev->next=x;
                        list_tail=x;
                    }
                }
            }*/

        }
    }
    return 0;
}
