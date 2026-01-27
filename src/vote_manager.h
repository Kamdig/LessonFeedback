#ifndef VOTE_MANAGER_H
#define VOTE_MANAGER_H

typedef enum {
    VOTE_RED,
    VOTE_YELLOW,
    VOTE_GREEN

} VoteType;

typedef struct {
    int red;
    int yellow;
    int green;

} VoteResults;

void vote_manager_init(void);
void add_vote(Votetype color);
VoteResults get_results(void);

#endif


