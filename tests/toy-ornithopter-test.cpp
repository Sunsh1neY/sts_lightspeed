#include "test-helpers.h"

using namespace combat_test;

static void installBlockPotion(BattleContext &bc) {
    bc.potions[0] = Potion::BLOCK_POTION;
    bc.potionCount = 1;
}

int main() {
    return run([] {
        auto used = start({RelicId::TOY_ORNITHOPTER});
        used.player.maxHp = 80;
        used.player.curHp = 70;
        installBlockPotion(used);
        search::Action(search::ActionType::POTION, 0, 0).execute(used);
        require(used.player.curHp == 75, "Using a potion must heal five HP with Toy Ornithopter");
        require(used.potionCount == 0 && used.potions[0] == Potion::EMPTY_POTION_SLOT,
                "The used potion must be consumed");

        auto capped = start({RelicId::TOY_ORNITHOPTER});
        capped.player.maxHp = 80;
        capped.player.curHp = 78;
        installBlockPotion(capped);
        search::Action(search::ActionType::POTION, 0, 0).execute(capped);
        require(capped.player.curHp == 80, "Toy Ornithopter healing must respect max HP");

        auto noRelic = start({});
        noRelic.player.maxHp = 80;
        noRelic.player.curHp = 70;
        installBlockPotion(noRelic);
        search::Action(search::ActionType::POTION, 0, 0).execute(noRelic);
        require(noRelic.player.curHp == 70, "The potion-use heal must be a Toy Ornithopter effect");

        auto discarded = start({RelicId::TOY_ORNITHOPTER});
        discarded.player.maxHp = 80;
        discarded.player.curHp = 70;
        installBlockPotion(discarded);
        discarded.discardPotion(0);
        require(discarded.player.curHp == 70, "Discarding a potion must not trigger Toy Ornithopter");
    });
}
