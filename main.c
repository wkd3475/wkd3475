#include <stdio.h>
#include <stdlib.h>

#define NELEM 1500

typedef struct Adj
{
    int n;
    struct Adj* next;
} Adj;

void Adj_init(Adj* self)
{
    self->n = 0;
    self->next = NULL;
}

typedef struct Word
{
    char id_num[12];
    char date[33];
    char word[30];

    struct word* next;
    struct word* prev;
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
    int n;

    struct User* next;
    struct User* prev;
    Adj* first;
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
    user->first = NULL;

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

void Friendship(User* self, User* v)
{
    Adj* a = (Adj *)malloc(sizeof(Adj));
    a->n = v->n;
    a->next = self->first;
    self->first = a;
}

void RoadFile(char user[15], char friendship[15], char word[15])
{
    char id_num[12];
    char date[35];
    char name[20];
    char buf[2];
    char box[100];
    int count=0;
    int ch;

    User_init();
    Word_init();

    FILE * fp = fopen(user, "rt");
    if(fp==NULL)
    {
        printf("fail(user)\n");
        return -1;
    }

    while(fgets(box, sizeof(box), fp) != NULL) count++;

    rewind(fp);

    int i;
    for(i=0; i<count/4; i++)
    {
        strcpy(id_num, fgets(id_num, sizeof(id_num), fp));
        strcpy(date, fgets(date, sizeof(date), fp));
        strcpy(name, fgets(name, sizeof(name), fp));
        strcpy(buf, fgets(buf, sizeof(buf), fp));
        newUser(id_num, date, name);
    }

    fclose(fp);

    fp = fopen(word, "rt");

    if(fp==NULL)
    {
        printf("fail(word)\n");
        return -1;
    }

    while(fgets(box, sizeof(box), fp) != NULL) count++;

    rewind(fp);

    for(i=0; i<count/4; i++)
    {
        strcpy(id_num, fgets(id_num, sizeof(id_num), fp));
        strcpy(date, fgets(date, sizeof(date), fp));
        strcpy(word, fgets(name, sizeof(name), fp));
        strcpy(buf, fgets(buf, sizeof(buf), fp));
        newWord(id_num, date, word);
    }

    fclose(fp);
}

int main()
{
    int order;

    while(1)
    {
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
            char file_name[3][15];
            printf("\nUserprofile : ");
            scanf("%s", &file_name[0]);
            printf("\nFriendship : ");
            scanf("%s", &file_name[1]);
            printf("\nWordTweet : ");
            scanf("%s", &file_name[2]);

            RoadFile(file_name[0], file_name[1], file_name[2]);

            Word* a;
            for(a = word_head; a!=word_tail; a=a->next)
            {
                printf("%s", a->id_num);
                printf("%s", a->date);
                printf("%s", a->word);
            }
        }
    }

    system("PAUSE");

    return 0;
}
