#include "test-helpers.h"

using namespace combat_test;

int main() {
    return run([] {
        auto chain = start({RelicId::VELVET_CHOKER}, Card(CardId::HAVOC, true), 20);
        play(chain);
        std::cout << "Cards played by Havoc chain: " << chain.player.cardsPlayedThisTurn << '\n';
        require(chain.player.cardsPlayedThisTurn == 6, "Choker must stop Havoc autoplay at six cards");
        require(chain.actionQueue.isEmpty() && chain.cardQueue.isEmpty(), "Autoplay must finish resolving");

        auto unrestricted = start({}, Card(CardId::HAVOC, true), 20);
        play(unrestricted);
        require(unrestricted.player.cardsPlayedThisTurn > 6, "No Choker must allow the longer chain");

        auto manual = start({RelicId::VELVET_CHOKER, RelicId::BAG_OF_PREPARATION}, Card(CardId::FLEX), 15);
        require(manual.player.energy == 4, "Choker must still grant energy");
        for (int i = 0; i < 6; ++i) play(manual);
        require(!search::Action(search::ActionType::CARD, 0, 0).isValidAction(manual),
                "The seventh manual card must remain illegal");
        endTurn(manual);
        require(manual.player.cardsPlayedThisTurn == 0, "The count must reset next turn");
        play(manual);
        require(manual.player.cardsPlayedThisTurn == 1, "Cards must be playable again next turn");

        // A queued Double Tap copy uses purgeOnUse and must retain its replay semantics.
        auto replay = start({RelicId::VELVET_CHOKER}, Card(CardId::FLEX), 10);
        for (int i = 0; i < 4; ++i) play(replay);
        replay.cards.createTempCardInHand(CardInstance(CardId::DOUBLE_TAP));
        play(replay, replay.cards.cardsInHand - 1);
        require(replay.player.cardsPlayedThisTurn == 5, "Double Tap must be the fifth card");
        replay.cards.createTempCardInHand(CardInstance(CardId::STRIKE_RED));
        play(replay, replay.cards.cardsInHand - 1);
        require(replay.player.cardsPlayedThisTurn == 7, "The sixth card's queued copy must still resolve");
    });
}
