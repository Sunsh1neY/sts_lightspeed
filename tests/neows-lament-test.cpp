#include "test-helpers.h"

using namespace combat_test;

static BattleContext startWithLament(int remaining) {
    GameContext gc(CharacterClass::IRONCLAD, 100123, 20);
    gc.floorNum = 1;
    gc.curRoom = Room::MONSTER;
    gc.relics = {};
    gc.relics.add({RelicId::NEOWS_LAMENT, remaining});
    gc.deck = {};
    for (int i = 0; i < 10; ++i) gc.deck.obtainRaw(Card(CardId::FLEX));
    BattleContext bc;
    bc.init(gc, MonsterEncounter::GREMLIN_LEADER);
    require(bc.inputState == InputState::PLAYER_NORMAL, "Expected player control");
    return bc;
}

int main() {
    return run([] {
        auto lament = startWithLament(1);
        require(lament.monsters.monsterCount == 4, "Gremlin Leader must reserve its four combat slots");
        require(lament.monsters.arr[0].idx == -1, "The first Gremlin Leader slot is an uninitialized placeholder");
        require(lament.monsters.arr[0].curHp == 0,
                "Neow's Lament must not assign HP to an uninitialized placeholder");
        for (int i = 1; i < lament.monsters.monsterCount; ++i) {
            require(lament.monsters.arr[i].idx >= 0 && lament.monsters.arr[i].curHp == 1,
                    "Neow's Lament must set one HP only on actual combat entities");
        }

        auto noLament = start({});
        require(noLament.monsters.arr[0].idx >= 0 && noLament.monsters.arr[0].curHp > 0,
                "The control encounter must contain a real enemy");
    });
}
