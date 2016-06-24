#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NELEM 1500

int total_user_num=0;
int total_friendship_record=0;
int total_tweet=0;

typedef struct Friend
{
    char id_num[12];
    struct Friend* next;
} Friend;

typedef struct Word
{
    char id_num[12];
    char date[33];
    char word[30];

    struct Word* next;
    struct Word* prev;
} Word;

Word *word_head, *word_tail;

void Word_init()
{
    word_head = (Word*)malloc(sizeof(Word));
    word_tail = (Word*)malloc(sizeof(Word));
    word_head->next = word_tail;
    word_tail->prev = word_head;
    word_head->prev = NULL;
    word_tail->next = NULL;
}

Word* newWord(char id_num[], char date[], char text[])
{
    Word* word = (Word*)malloc(sizeof(Word));
    strcpy(word->id_num, id_num);
    strcpy(word->date, date);
    strcpy(word->word, text);
    word->prev = word_head;
    word->next = word_head->next;
    word_head->next = word;

    return word;
}

typedef struct User
{
    char id_num[12];
    char date[33];
    char name[15];

    struct User* next;
    struct User* prev;
    Friend* friends;
} User;

User *user_head, *user_tail;

User* newUser(char id_num[], char date[], char name[])
{
    User* user = (User*)malloc(sizeof(User));
    strcpy(user->id_num, id_num);
    strcpy(user->date, date);
    strcpy(user->name, name);
    user->prev = user_head;
    user->next = user_head->next;
    user_head->next = user;
    user->friends = NULL;

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

    total_friendship_record=count;

    int i;
    for(i=0; i<count/3; i++)
    {
        strcpy(id_1, fgets(id_1, sizeof(id_1), fp_friend));
        strcpy(id_2, fgets(id_2, sizeof(id_2), fp_friend));
        strcpy(buf, fgets(buf, sizeof(buf), fp_friend));


        User* a;
        User* b;
        for(a=user_head; a!=NULL; a=a->next)
        {
            if(!strcmp(a->id_num, id_1))
                break;
        }

        for(b=user_head; b!=NULL; b=b->next)
        {
            if(!strcmp(b->id_num, id_2))
                break;
        }

        Friend* f1;
        Friend* f2;

        strcpy(f1->id_num, b->id_num);
        strcpy(f2->id_num, a->id_num);

        f1->next=a->friends->next;
        a->friends=f1;

        f2->next=b->friends->next;
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

    total_tweet=count;

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

int main()
{
    int order;

    RoadFile_user("user.txt");
    //RoadFile_friend("friend.txt");
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
        printf("---------------------------------------------------------\n");
        printf("order : ");

        scanf("%d", &order);
        if(order==0)
        {
            printf("Total users: %d\n", total_user_num);
            printf("Total friendship records: %d\n", total_friendship_record);
            printf("Total tweets: %d\n", total_tweet);
        }
        else if(order==1)
        {

        }
    }

    system("PAUSE");

    return 0;
}
