#include "test-helpers.h"

using namespace combat_test;

int main() {
    return run([] {
        auto odd = start({RelicId::MAGIC_FLOWER});
        odd.player.maxHp = 80;
        odd.player.curHp = 60;
        odd.player.heal(5);
        require(odd.player.curHp == 68, "Magic Flower must round 7.5 healing to 8");

        auto even = start({RelicId::MAGIC_FLOWER});
        even.player.maxHp = 80;
        even.player.curHp = 60;
        even.player.heal(4);
        require(even.player.curHp == 66, "Magic Flower must preserve exact even healing");

        auto capped = start({RelicId::MAGIC_FLOWER});
        capped.player.maxHp = 80;
        capped.player.curHp = 78;
        capped.player.heal(5);
        require(capped.player.curHp == 80, "Magic Flower healing must respect max HP");

        auto noRelic = start({});
        noRelic.player.maxHp = 80;
        noRelic.player.curHp = 60;
        noRelic.player.heal(5);
        require(noRelic.player.curHp == 65, "The rounding change must be a Magic Flower effect");
    });
}
