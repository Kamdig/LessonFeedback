#include "vote_manager.h"

// Intern variabel som lagrar alla röster (syns bara i denna fil)
static VoteResults results;

// Nollställer alla röstvärden
void vote_manager_init(void) {
    results.red = 0;
    results.yellow = 0;
    results.green = 0;
}

// Ökar räknaren för vald färg
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

// Returnerar en kopia av nuvarande röstresultat
VoteResults get_results(void) {
    return results;
}
