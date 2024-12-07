#include <memory>
#include <unordered_map>
#include <vector>

#include "level.h"
#include "room.h"
#include "utils.h"

class Generator
{
   private:
    std::shared_ptr<Level> level;

   public:
    // Getters
    std::shared_ptr<Level> get_level() const
    {
        return level;
    }

    // Setters

    void set_level(std::shared_ptr<Level> new_level)
    {
        level = new_level;
    }

    // Functions

    void rogue(int room_num, vec2 min_room_size, vec2 max_room_size, int room_padding);
    void automata(int iterations = 10, int chance = 40, int birth_threshold = 5, int survival_threshold = 4);
    void remove_inaccessible();
};

extern std::vector<std::shared_ptr<Generator>> generators;
