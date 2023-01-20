#include "bn_core.h"
#include "bn_regular_bg_position_hbe_ptr.h"
#include "bn_affine_bg_attributes_hbe_ptr.h"
#include "bn_regular_bg_ptr.h"
#include "bn_display.h"

#include "bn_regular_bg_items_bg1.h"

bn::fixed loop(bn::fixed number, bn::fixed increment)
{
    number += increment;
    if (number > 256)
    {
        return 0;
    }
    else
    {
        return number;
    }
}

int main()
{
    bn::core::init();

    // map
    bn::regular_bg_ptr map = bn::regular_bg_items::bg1.create_bg(0, 0);

    bn::array<bn::fixed, bn::display::height()> horizontal_deltas;
    bn::regular_bg_position_hbe_ptr horizontal_hbe =
        bn::regular_bg_position_hbe_ptr::create_horizontal(map, horizontal_deltas);

    bn::fixed layer_1 = 0;
    bn::fixed layer_2 = 0;
    bn::fixed layer_3 = 0;
    bn::fixed layer_4 = 0;

    while (true)
    {
        layer_1 = loop(layer_1, 0.3);
        layer_2 = loop(layer_2, 0.2);
        layer_3 = loop(layer_3, 0.1);
        layer_4 = loop(layer_4, 0.5);

        for (int index = 0, limit = 160; index < limit; ++index)
        {
            if (index > 140)
            {
                horizontal_deltas[index] = layer_1;
            }
            else if (index > 127)
            {
                horizontal_deltas[index] = layer_2;
            }
            else if (index > 118)
            {
                horizontal_deltas[index] = layer_3;
            }
            else if (index > 0)
            {
                horizontal_deltas[index] = layer_4;
            }
            else
            {
                horizontal_deltas[index] = 0;
            }
        }

        horizontal_hbe.reload_deltas_ref();

        bn::core::update();
    }
}
