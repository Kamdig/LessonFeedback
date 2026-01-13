#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include "vote_manager.h"

static void print_results(VoteResults r) {
    printf("Current results:\n");
    printf("  Red   : %d\n", r.red);
    printf("  Yellow: %d\n", r.yellow);
    printf("  Green : %d\n", r.green);
}

int main(void) {
    // Initiera (nollställ) alla röster
    vote_manager_init();

    printf("Vote counter\n");
    printf("Press R (Red), Y (Yellow), G (Green)\n");
    printf("Press Q to exit.\n\n");

    bool running = true;
    char line[64];

    while (running) {
        printf("Your vote (R/Y/G/Q): ");

        // Läs en rad från tangentbordet
        if (fgets(line, sizeof(line), stdin) == NULL) {
            // Om något går fel med inmatningen avslutar vi
            printf("\nNo more input. Exiting.\n");
            break;
        }

        // Ta första tecknet i raden
        char c = line[0];

        // Ignorera tom rad
        if (c == '\n' || c == '\0') {
            continue;
        }

        // Gör om till stor bokstav så både r och R fungerar
        c = (char)toupper((unsigned char)c);

        // Avsluta om användaren trycker Q
        if (c == 'Q') {
            running = false;
            continue;
        }

        bool valid = true;

        // Tolka vilken färg som valts
        switch (c) {
            case 'R':
                add_vote(VOTE_RED);
                break;

            case 'Y':
                add_vote(VOTE_YELLOW);
                break;

            case 'G':
                add_vote(VOTE_GREEN);
                break;

            default:
                printf("Ogiltig inmatning. Använd R, Y, G eller Q.\n");
                valid = false;
                break;
        }

        // Om rösten var giltig, skriv ut resultatet
        if (valid) {
            VoteResults results = get_results();
            print_results(results);
            printf("\n");
        }
    }

    return 0;
}
