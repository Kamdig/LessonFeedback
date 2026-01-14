#include <gtest/gtest.h>
#include "../src/vote_manager.h"

class MainTest : public ::testing::Test {
protected:
    void SetUp() override {
        vote_manager_init();
    }
};

TEST_F(MainTest, green2TimesWork) {
    add_vote(VOTE_GREEN);
    add_vote(VOTE_GREEN);

    VoteResults results = get_results();

    EXPECT_EQ(results.green, 2);
    EXPECT_EQ(results.red, 0);
    EXPECT_EQ(results.yellow, 0);
}

TEST_F(MainTest, resetVotesWork) {
    add_vote(VOTE_RED);
    add_vote(VOTE_YELLOW);
    add_vote(VOTE_GREEN);

    vote_manager_init();

    VoteResults results = get_results();

    EXPECT_EQ(results.green, 0);
    EXPECT_EQ(results.red, 0);
    EXPECT_EQ(results.yellow, 0);
}