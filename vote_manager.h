#ifndef VOTE_MANAGER_H
#define VOTE_MANAGER_H

// Enum som representerar de olika röstfärgerna
typedef enum {
    VOTE_RED,
    VOTE_YELLOW,
    VOTE_GREEN
} VoteType;

// Struct som lagrar antalet röster för varje färg
typedef struct {
    int red;
    int yellow;
    int green;
} VoteResults;

// Initierar (nollställer) alla räknare
void vote_manager_init(void);

// Lägger till en röst av angiven färg
void add_vote(VoteType color);

// Returnerar nuvarande röstresultat
VoteResults get_results(void);

#endif
