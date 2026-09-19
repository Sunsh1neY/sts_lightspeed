#include "test-helpers.h"

using namespace combat_test;

int main() {
    return run([] {
        auto shuffle = start({RelicId::UNCEASING_TOP}, Card(CardId::FLEX), 5);
        for (int i = 0; i < 5; ++i) play(shuffle);
        std::cout << "After empty-hand shuffle: hand=" << shuffle.cards.cardsInHand
                  << ", queued actions=" << shuffle.actionQueue.size << '\n';
        require(shuffle.cards.cardsInHand == 1, "Top must finish the queued shuffle and draw before player control");
        require(shuffle.actionQueue.isEmpty(), "No pending draw action may remain at player control");
        require(shuffle.cards.drawPile.size() == 4 && shuffle.cards.discardPile.empty(), "Expected one shuffled card drawn");
        play(shuffle);
        require(shuffle.cards.cardsInHand == 1 && shuffle.cards.drawPile.size() == 3,
                "Top must also draw immediately from a nonempty draw pile");

        auto blocked = start({RelicId::UNCEASING_TOP}, Card(CardId::BATTLE_TRANCE), 10);
        play(blocked);
        require(blocked.player.hasStatus<PS::NO_DRAW>(), "Battle Trance must prevent further draws");
        while (blocked.cards.cardsInHand) play(blocked);
        require(blocked.cards.drawPile.size() == 2 && blocked.actionQueue.isEmpty(),
                "No Draw must return control without drawing or looping");
        endTurn(blocked);
        require(blocked.cards.cardsInHand == 5 && !blocked.player.hasStatus<PS::NO_DRAW>(),
                "No Draw must clear next turn");

        auto plain = start({}, Card(CardId::FLEX), 5);
        for (int i = 0; i < 5; ++i) play(plain);
        require(plain.cards.cardsInHand == 0 && plain.cards.discardPile.size() == 5,
                "Without Top the empty hand must remain empty");

        // Top with no drawable cards must not loop; Thorns keeps the battle viable.
        auto empty = start({RelicId::UNCEASING_TOP, RelicId::BRONZE_SCALES}, Card(CardId::OFFERING), 1);
        play(empty);
        require(empty.inputState == InputState::PLAYER_NORMAL && empty.actionQueue.isEmpty(),
                "Empty piles must safely return control");
        require(empty.cards.cardsInHand == 0 && empty.cards.exhaustPile.size() == 1,
                "Exhausted cards must not be drawn");
    });
}
