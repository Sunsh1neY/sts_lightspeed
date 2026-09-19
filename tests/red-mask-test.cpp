#include "test-helpers.h"

using namespace combat_test;

int main() {
    return run([] {
        auto bc = start({RelicId::RED_MASK});
        require(bc.monsters.arr[0].getStatus<MS::WEAK>() == 1, "Red Mask must apply one Weak");
        endTurn(bc);
        std::cout << "Weak after first enemy turn: " << bc.monsters.arr[0].getStatus<MS::WEAK>() << '\n';
        require(bc.monsters.arr[0].getStatus<MS::WEAK>() == 0,
                "Red Mask Weak must expire after the first enemy turn");
        endTurn(bc);
        require(bc.monsters.arr[0].getStatus<MS::WEAK>() == 0, "Red Mask must not reapply");

        auto sentries = start({RelicId::RED_MASK}, Card(CardId::FLEX), 10,
                MonsterEncounter::THREE_SENTRIES);
        for (int i = 0; i < sentries.monsters.monsterCount; ++i) {
            require(sentries.monsters.arr[i].getStatus<MS::WEAK>() == 0, "Artifact must block Weak");
            require(sentries.monsters.arr[i].getStatus<MS::ARTIFACT>() == 0, "Weak must consume Artifact");
        }
        auto plain = start({});
        require(plain.monsters.arr[0].getStatus<MS::WEAK>() == 0, "No relic must mean no opening Weak");
    });
}
