#include "test-helpers.h"
#include <array>
#include <vector>

using namespace combat_test;

// Exercise a real lethal card and the normal battle-exit writeback. Only the
// post-combat UI callback is replaced; healing is left entirely to the engine.
int finishBattle(int hp, int maxHp, const std::vector<RelicId> &relics, bool victory) {
    GameContext gc(CharacterClass::IRONCLAD, 100123, 20);
    gc.floorNum = 1;
    gc.curRoom = Room::MONSTER;
    gc.curHp = hp;
    gc.maxHp = maxHp;
    gc.relics = {};
    for (auto relic : relics) gc.relics.add({relic, 0});
    gc.deck = {};
    for (int i = 0; i < 10; ++i) gc.deck.obtainRaw(Card(CardId::STRIKE_RED));
    bool returnedToGame = false;
    gc.regainControlAction = [&returnedToGame](GameContext &) { returnedToGame = true; };
    BattleContext bc;
    bc.init(gc, MonsterEncounter::JAW_WORM);
    if (victory) {
        // Controlled near-death enemy fixture: the action, terminal transition,
        // HP synchronization and relic callbacks still run through the engine.
        bc.monsters.arr[0].curHp = 1;
        bc.monsters.arr[0].block = 0;
        play(bc);
        require(bc.outcome == Outcome::PLAYER_VICTORY, "Strike must end the combat");
        require(bc.player.curHp == hp, "The lethal card must not change player HP");
    } else {
        bc.player.curHp = 0;
        bc.outcome = Outcome::PLAYER_LOSS;
    }
    bc.exitBattle(gc);
    require(returnedToGame == victory, "Only victory should return to the game callback");
    if (!victory) require(gc.outcome == GameOutcome::PLAYER_LOSS, "Defeat must remain a defeat");
    return gc.curHp;
}

int main() {
    return run([] {
        const std::array<std::vector<RelicId>, 8> profiles = {{
            {}, {RelicId::MEAT_ON_THE_BONE}, {RelicId::BURNING_BLOOD}, {RelicId::BLACK_BLOOD},
            {RelicId::BURNING_BLOOD, RelicId::MEAT_ON_THE_BONE},
            {RelicId::MEAT_ON_THE_BONE, RelicId::BURNING_BLOOD},
            {RelicId::BLACK_BLOOD, RelicId::MEAT_ON_THE_BONE},
            {RelicId::MEAT_ON_THE_BONE, RelicId::BLACK_BLOOD}
        }};
        struct Case { int hp, maxHp; std::array<int, 8> expected; };
        const Case cases[] = {
            {39, 80, {39, 51, 45, 51, 57, 57, 63, 63}},
            {40, 80, {40, 52, 46, 52, 58, 58, 64, 64}},
            {41, 80, {41, 41, 47, 53, 47, 47, 53, 53}},
            {40, 81, {40, 52, 46, 52, 58, 58, 64, 64}},
            {41, 81, {41, 41, 47, 53, 47, 47, 53, 53}},
            {10, 20, {10, 20, 16, 20, 20, 20, 20, 20}},
            {80, 80, {80, 80, 80, 80, 80, 80, 80, 80}},
            { 1, 80, { 1, 13,  7, 13, 19, 19, 25, 25}}
        };
        int failures = 0;
        int checked = 0;
        for (const auto &test : cases) {
            for (std::size_t profile = 0; profile < profiles.size(); ++profile) {
                const int actual = finishBattle(test.hp, test.maxHp, profiles[profile], true);
                ++checked;
                if (actual != test.expected[profile]) {
                    ++failures;
                    std::cerr << "HP " << test.hp << '/' << test.maxHp << ", profile " << profile
                              << ": expected " << test.expected[profile] << ", got " << actual << '\n';
                }
            }
        }
        for (const auto &profile : profiles) {
            require(finishBattle(40, 80, profile, false) == 0, "Victory relics must not heal a defeat");
            ++checked;
        }
        std::cout << checked << " battle-exit cases, " << failures << " mismatches\n";
        require(failures == 0, "Meat on the Bone must settle before victory-relic healing, regardless of relic order");
    });
}
