#include "vote_manager.h"

static VoteResults results;

void vote_manager_init(void) {
    results.red = 0;
    results.yellow = 0;
    results.green = 0;
}

void add_vote(VoteType color) {
    switch (color) {
        case VOTE_RED:
            results.red++;
            break;
        case VOTE_YELLOW:
            results.yellow++;
            break;
        case VOTE_GREEN:
            results.green++;
            break;
    }
}

VoteResults get_results(void) {
    return results;
}