#ifndef STS_COMBAT_TEST_HELPERS_H
#define STS_COMBAT_TEST_HELPERS_H

#include <initializer_list>
#include <iostream>
#include <stdexcept>

#include "combat/BattleContext.h"
#include "game/GameContext.h"
#include "sim/search/Action.h"

namespace combat_test {
using namespace sts;

inline void require(bool condition, const char *message) {
    if (!condition) throw std::runtime_error(message);
}

inline BattleContext start(std::initializer_list<RelicId> relics,
        Card card = Card(CardId::FLEX), int count = 10,
        MonsterEncounter encounter = MonsterEncounter::JAW_WORM) {
    GameContext gc(CharacterClass::IRONCLAD, 100123, 20);
    gc.floorNum = 1;
    gc.curRoom = Room::MONSTER;
    gc.relics = {};
    for (auto relic : relics) gc.relics.add({relic, 0});
    gc.deck = {};
    for (int i = 0; i < count; ++i) gc.deck.obtainRaw(card);
    BattleContext bc;
    bc.init(gc, encounter);
    require(bc.inputState == InputState::PLAYER_NORMAL, "Expected player control");
    return bc;
}

inline void play(BattleContext &bc, int index = 0) {
    search::Action action(search::ActionType::CARD, index, 0);
    require(action.isValidAction(bc), "Test attempted an illegal card action");
    action.execute(bc);
}

inline void endTurn(BattleContext &bc) {
    search::Action(search::ActionType::END_TURN).execute(bc);
}

template <typename Test> int run(Test test) {
    try { test(); std::cout << "PASS\n"; return 0; }
    catch (const std::exception &error) {
        std::cerr << "FAIL: " << error.what() << '\n'; return 1;
    }
}
}
#endif
