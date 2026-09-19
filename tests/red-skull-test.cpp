#include "test-helpers.h"

using namespace combat_test;

int main() {
    return run([] {
        auto redSkull = start({RelicId::RED_SKULL});
        redSkull.player.maxHp = 80;
        redSkull.player.curHp = 40;
        redSkull.player.strength = 3;
        redSkull.player.heal(5);
        require(redSkull.player.curHp == 45, "Healing should cross the bloodied threshold");
        require(redSkull.player.strength == 0,
                "Red Skull must remove three Strength when healing above half HP");

        auto noCrossing = start({RelicId::RED_SKULL});
        noCrossing.player.maxHp = 80;
        noCrossing.player.curHp = 40;
        noCrossing.player.strength = 3;
        noCrossing.player.heal(0);
        require(noCrossing.player.strength == 3, "Healing that does not cross half HP must not remove Strength");

        auto aboveThreshold = start({RelicId::RED_SKULL});
        aboveThreshold.player.maxHp = 80;
        aboveThreshold.player.curHp = 41;
        aboveThreshold.player.strength = 0;
        aboveThreshold.player.heal(5);
        require(aboveThreshold.player.strength == 0, "Red Skull must not remove Strength when already above half HP");

        auto noRelic = start({});
        noRelic.player.maxHp = 80;
        noRelic.player.curHp = 40;
        noRelic.player.strength = 3;
        noRelic.player.heal(5);
        require(noRelic.player.strength == 3, "The threshold crossing must be a Red Skull effect");
    });
}
