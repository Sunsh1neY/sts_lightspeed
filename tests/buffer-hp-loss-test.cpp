#include "test-helpers.h"

using namespace combat_test;

int main() {
    return run([] {
        auto buffered = start({RelicId::FOSSILIZED_HELIX});
        const int before = buffered.player.curHp;
        require(buffered.player.hasStatus<PS::BUFFER>(), "Fossilized Helix must grant Buffer");
        buffered.player.loseHp(buffered, 5, false);
        require(buffered.player.curHp == before, "Buffer must intercept direct positive HP loss");
        require(!buffered.player.hasStatus<PS::BUFFER>(), "Buffer must be consumed by the intercepted loss");

        auto bufferedRod = start({RelicId::FOSSILIZED_HELIX, RelicId::TUNGSTEN_ROD});
        const int rodBefore = bufferedRod.player.curHp;
        bufferedRod.player.loseHp(bufferedRod, 5, false);
        require(bufferedRod.player.curHp == rodBefore,
                "Buffer must intercept before Tungsten Rod reduces direct HP loss");
        require(!bufferedRod.player.hasStatus<PS::BUFFER>(), "The intercepted Buffer must be consumed");

        auto rodOnly = start({RelicId::TUNGSTEN_ROD});
        const int rodOnlyBefore = rodOnly.player.curHp;
        rodOnly.player.loseHp(rodOnly, 5, false);
        require(rodOnly.player.curHp == rodOnlyBefore - 4,
                "Without Buffer, Tungsten Rod must reduce five HP loss to four");

        auto ordinaryDamage = start({RelicId::FOSSILIZED_HELIX});
        const int damageBefore = ordinaryDamage.player.curHp;
        ordinaryDamage.player.damage(ordinaryDamage, 5);
        require(ordinaryDamage.player.curHp == damageBefore,
                "The direct HP-loss fix must preserve the existing attack Buffer path");
    });
}
