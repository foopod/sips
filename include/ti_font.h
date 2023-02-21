/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef TI_VARIABLE_8x8_SPRITE_FONT_H
#define TI_VARIABLE_8x8_SPRITE_FONT_H

#include "bn_sprite_font.h"
#include "bn_utf8_characters_map.h"
#include "bn_sprite_items_font.h"

namespace ti {

constexpr bn::utf8_character variable_8x8_sprite_font_utf8_characters[] = {
    "á", "é", "í", "ó", "ú", "ü", "ñ", "¡", "¿", "α", "β"};

constexpr int8_t variable_8x8_sprite_font_character_widths[] = {
    8, 3, 5, 7, 7, 8, 7, 3, 6, 6, 7, 7, 4, 7, 3, 6, 8, 7, 8, 8, 8, 8,
    8, 8, 8, 8, 4, 4, 5, 7, 5, 7, 8, 7, 7, 7, 7, 7, 7, 7, 7, 3, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 4, 6, 4, 7, 7, 3, 7,
    7, 7, 7, 7, 6, 7, 7, 3, 5, 7, 4, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 5, 3, 5, 7, 7, 7, 4, 7, 7, 7, 7, 3, 7, 8, 8,
};

constexpr bn::span<const bn::utf8_character>
    variable_8x8_sprite_font_utf8_characters_span(
        variable_8x8_sprite_font_utf8_characters);

constexpr auto variable_8x8_sprite_font_utf8_characters_map =
    bn::utf8_characters_map<variable_8x8_sprite_font_utf8_characters_span>();
 
constexpr bn::sprite_font variable_8x8_sprite_font(
    bn::sprite_items::font,
    variable_8x8_sprite_font_utf8_characters_map.reference(),
    variable_8x8_sprite_font_character_widths);
} // namespace ti

#endif