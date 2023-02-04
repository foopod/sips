#ifndef TI_PERSON_H
#define TI_PERSON_H

#include "bn_optional.h"
#include "bn_sprite_ptr.h"
#include "bn_blending.h"
#include "bn_random.h"
#include "bn_sprite_animate_actions.h"

#include "bn_sprite_items_walk1.h"
#include "bn_sprite_items_walk2.h"

namespace ti
{
    enum class STATE
    {
        WALKING_LEFT = 1,
        WALKING_LEFT_W_COFFEE = 2,
        WALKING_RIGHT = 3,
        WALKING_RIGHT_W_COFFEE = 4,
        ENTERING = 5,
        WALKING_TO_COUNTER = 6,
        WALKING_FROM_COUNTER = 7,
        EXITING = 8,
        WAITING = 9,
    };

    enum class TYPE
    {
        GREEN_SHIRT = 0,
        RED_SHIRT = 1,
        BLUE_SHIRT = 2,
        RED_SINGLET = 3,
        PALE_GREEN_SHIRT = 4,
        DWIGHT = 5,
        GIRL1 = 6,

    };

    enum class START
    {
        LEFT,
        RIGHT
    };
}

namespace ti
{

    class Person
    {
        private:
            bn::optional<bn::sprite_ptr> _sprite;
            bn::optional<bn::sprite_animate_action<8>> _action;
            bn::sprite_ptr _shadow;
            bn::optional<bn::sprite_item> _sprite_item;
            bn::fixed _speed = 0.3;
            bn::random _random = bn::random();
            TYPE _type = TYPE::GREEN_SHIRT;
            int _wait_max = 260;
            int _wait_time = 0;
            STATE _state = STATE::WAITING;
            void setStyle(TYPE type, START start, bn::fixed_point pos);
        public:
            Person(START start, TYPE type);
            void update();

    };
}

#endif