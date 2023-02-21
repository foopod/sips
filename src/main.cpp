#include "bn_core.h"
#include "bn_sprite_animate_actions.h"
#include "bn_regular_bg_ptr.h"
#include "bn_display.h"
#include "bn_keypad.h"
#include "bn_random.h"
#include "bn_blending.h"
#include "bn_music.h"
#include "bn_music_items.h"
#include "bn_sprite_palette_ptr.h"
#include "bn_deque.h"
#include "bn_log.h"
#include "bn_sprite_text_generator.h"
#include "bn_string.h"

#include "bn_regular_bg_items_bg1.h"
#include "bn_regular_bg_items_overlay.h"
#include "bn_sprite_items_title.h"
#include "bn_sprite_items_steam.h"
#include "bn_sprite_items_barista.h"
#include "bn_sprite_items_drinker.h"
#include "bn_sprite_items_waiting.h"
#include "bn_sprite_items_sylvester.h"
#include "bn_sprite_items_blocker.h"
#include "bn_sprite_items_talkative.h"
#include "bn_sprite_items_reflect.h"
#include "bn_sprite_items_pigeon.h"
#include "bn_sprite_items_pigeon2.h"
#include "bn_sprite_items_swallow.h"
#include "bn_sprite_items_typist.h"
#include "bn_sprite_items_clock.h"
#include "bn_sprite_items_till.h"
#include "bn_sprite_items_cursor.h"
#include "bn_sprite_items_bonsai.h"
#include "bn_sprite_items_vines.h"
#include "bn_sprite_items_rug1.h"
#include "bn_sprite_items_topiary.h"
#include "bn_sprite_items_painting.h"
#include "bn_sprite_items_cactus1.h"
#include "bn_sprite_items_cookies.h"
#include "bn_sprite_items_popularity_bar.h"
#include "bn_sprite_items_cash.h"
#include "bn_sprite_items_twinkle.h"
#include "bn_sprite_palette_items_white_text_palette.h"
#include "bn_sprite_palette_items_black_text_palette.h"

#include "bn_sound_items.h"

#include "ti_person.h"
#include "ti_font.h"

namespace {
    bn::fixed_point get_cursor_pos(int index){
        return bn::fixed_point(12,-61 + index*12);
    };

    void redraw_wishlist(bn::sprite_text_generator &text_generator, bn::vector<bn::sprite_ptr, 60> &text_sprites, bn::vector<int, 16> &prices){
        text_sprites.clear();
        text_generator.set_left_alignment();
        text_generator.generate(20, -72, "To Buy", text_sprites);
        text_generator.generate(20, -60, "Clock", text_sprites);
        text_generator.generate(20, -48, "Cookies", text_sprites);
        text_generator.generate(20, -36, "Bonsai", text_sprites);
        text_generator.generate(20, -24, "Vines", text_sprites);
        text_generator.generate(20, -12, "Topiary", text_sprites);
        text_generator.generate(20, 0, "Art", text_sprites);
        text_generator.generate(20, 12, "Cactus", text_sprites);
        text_generator.generate(20, 24, "Kitty", text_sprites);
        text_generator.generate(20, 36, "Wi-fi", text_sprites);
        text_generator.set_right_alignment();
        text_generator.generate(112, -72, "$", text_sprites);
        for (int i = 0; i < prices.size(); i++){
            if(prices.at(i) == 0){
                text_generator.generate(116, -60 + i*12, "--", text_sprites);
            } else {
                text_generator.generate(116, -60 + i*12, bn::to_string<8>(prices.at(i)), text_sprites);
            }
        }
        return;
    }
}

int main()
{
    bn::core::init();

    bn::sprite_text_generator text_generator(ti::variable_8x8_sprite_font);
    bn::vector<bn::sprite_ptr, 60> text_sprites;
    bn::vector<bn::sprite_ptr, 4> cash_text_sprites;
    text_generator.set_bg_priority(0);

    int cash = 35;
    int popularity_level = 1;
    bn::sprite_ptr popularity_bar = bn::sprite_items::popularity_bar.create_sprite(-79, -73, popularity_level);

    int is_menu_shown = false;
    int cursor_index = 0;
    bn::sprite_ptr cursor = bn::sprite_items::cursor.create_sprite(get_cursor_pos(cursor_index));
    cursor.set_bg_priority(0);

    bn::vector<bn::sprite_ptr, 16> upgrades;
    bn::vector<int, 16> prices;
    prices.push_back(30);
    prices.push_back(15);
    prices.push_back(70);
    prices.push_back(20);
    prices.push_back(40);
    prices.push_back(55);
    prices.push_back(22);
    prices.push_back(100);
    prices.push_back(125);
    bn::vector<bn::sprite_ptr, 8> popularity_bonuses;

    upgrades.push_back(bn::sprite_items::clock.create_sprite(-18, -18));
    upgrades.push_back(bn::sprite_items::cookies.create_sprite(-55, 9));
    upgrades.push_back(bn::sprite_items::bonsai.create_sprite(-86, -23));
    upgrades.push_back(bn::sprite_items::vines.create_sprite(-70, -19));
    upgrades.push_back(bn::sprite_items::topiary.create_sprite(112, 48));
    upgrades.push_back(bn::sprite_items::painting.create_sprite(-53, -8));
    upgrades.push_back(bn::sprite_items::cactus1.create_sprite(-116, 19));
    upgrades.push_back(bn::sprite_items::sylvester.create_sprite(-12, 23));
    upgrades.push_back(bn::sprite_items::typist.create_sprite(-35, 42));
    for(bn::sprite_ptr sprite : upgrades){
        sprite.set_visible(false);
    }

    bn::music_items::wild_strawberry.play();
    bn::music::set_volume(1);

    bn::sound_items::bustle.play(0.1);
    int bustle_timer = 0;

    // map
    bn::regular_bg_ptr map = bn::regular_bg_items::bg1.create_bg(0, 0);
    bn::regular_bg_ptr map2 = bn::regular_bg_items::overlay.create_bg(0, 0);
    map2.set_priority(1);
    map2.set_visible(false);

    // sprite
    bn::sprite_ptr title = bn::sprite_items::title.create_sprite(4, -22);
    bn::sprite_ptr steam = bn::sprite_items::steam.create_sprite(-70, -8);
    steam.set_blending_enabled(true);
    bn::blending::set_transparency_alpha(0.5);
    bn::sprite_ptr barista = bn::sprite_items::barista.create_sprite(-97, -5);
    bn::sprite_ptr drinker = bn::sprite_items::drinker.create_sprite(-105, 44);
    bn::sprite_ptr talkative = bn::sprite_items::talkative.create_sprite(-83, 42);
    bn::sprite_ptr pigeon = bn::sprite_items::pigeon.create_sprite(46, -71);
    bn::sprite_ptr pigeon2 = bn::sprite_items::pigeon2.create_sprite(60, -71);
    pigeon2.set_horizontal_flip(true);
    bn::sprite_ptr till = bn::sprite_items::till.create_sprite(-65, 3);
    bn::sprite_ptr cash_sprite = bn::sprite_items::cash.create_sprite(-65, -7);
    cash_sprite.set_visible(false);
    bn::sprite_ptr twinkle = bn::sprite_items::twinkle.create_sprite(-65, -7);
    twinkle.set_visible(false);

    // bn::sprite_ptr swallow = bn::sprite_items::swallow.create_sprite(140, -5);
    // bool is_left = true;
    // bool is_paused = true;

    bn::sprite_ptr reflect1 = bn::sprite_items::reflect.create_sprite(5, 20);
    reflect1.set_blending_enabled(true);
    reflect1.set_z_order(-500);
    // bn::sprite_ptr reflect2 = bn::sprite_items::reflect.create_sprite(-88, -6);
    // reflect2.set_blending_enabled(true);
    // reflect2.set_horizontal_flip(true);
    // reflect2.set_vertical_flip(true);
    // reflect2.set_z_order(-500);

    bn::sprite_ptr blocker = bn::sprite_items::blocker.create_sprite(58, 42);
    blocker.set_z_order(-40);

    int timer = 40;
    bn::random rng = bn::random();

    //animation action
    bn::sprite_animate_action<7> steamAction = bn::create_sprite_animate_action_once(
                    steam, 5, bn::sprite_items::steam.tiles_item(), 6,6,6,6,6,6,6);
    steam.set_visible(false);
    bn::sprite_animate_action<5> drinkerAction = bn::create_sprite_animate_action_once(
                    drinker, 15, bn::sprite_items::drinker.tiles_item(), 0,0,0,0,0);
    bn::sprite_animate_action<10> sylvesterAction = bn::create_sprite_animate_action_forever(
                    upgrades.at(7), 18, bn::sprite_items::sylvester.tiles_item(), 0,1,2,3,4,5,6,7,8,9);

    bn::sprite_animate_action<5> pigeonAction = bn::create_sprite_animate_action_once(
                    pigeon, 18, bn::sprite_items::pigeon.tiles_item(), 0,1,0,1,0);
    bn::sprite_animate_action<5> pigeon2Action = bn::create_sprite_animate_action_once(
                    pigeon2, 18, bn::sprite_items::pigeon2.tiles_item(), 0,1,0,1,0);

    bn::sprite_animate_action<15> reflectAction1 = bn::create_sprite_animate_action_once(
                    reflect1, 4, bn::sprite_items::reflect.tiles_item(), 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14);

    bn::sprite_animate_action<7> twinkle_action = bn::create_sprite_animate_action_once(
                    twinkle, 4, bn::sprite_items::twinkle.tiles_item(), 0,1,2,3,4,5,6);

    // bn::sprite_animate_action<15> reflectAction2 = bn::create_sprite_animate_action_once(
    //                 reflect2, 4, bn::sprite_items::reflect.tiles_item(), 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14);

    // bn::sprite_animate_action<2> swallowAction = bn::create_sprite_animate_action_forever(
    //                 swallow, 4, bn::sprite_items::swallow.tiles_item(), 0,0);

    bn::sprite_animate_action<2> typistAction = bn::create_sprite_animate_action_forever(
                    upgrades.at(8), 8, bn::sprite_items::typist.tiles_item(), 0,1);

    bn::sprite_animate_action<12> clockAction = bn::create_sprite_animate_action_forever(
                    upgrades.at(0), 300, bn::sprite_items::clock.tiles_item(), 0,1,2,3,4,5,6,7,8,9,10,11);

    bn::deque<int, 8> order_queue = {};
    bool waiting_spot = false;
    bool purchased_this_frame = false;

    bn::vector<ti::Person, 16> people;
    people.push_back(ti::Person(ti::START::RIGHT, ti::TYPE::GREEN_SHIRT, 0));
    people.push_back(ti::Person(ti::START::LEFT, ti::TYPE::GREEN_SHIRT, 1));
    people.push_back(ti::Person(ti::START::RIGHT, ti::TYPE::GREEN_SHIRT, 2));
    people.push_back(ti::Person(ti::START::LEFT, ti::TYPE::GREEN_SHIRT, 3));
    people.push_back(ti::Person(ti::START::RIGHT, ti::TYPE::GREEN_SHIRT, 4));
    people.push_back(ti::Person(ti::START::LEFT, ti::TYPE::GREEN_SHIRT, 5));
    people.push_back(ti::Person(ti::START::RIGHT, ti::TYPE::GREEN_SHIRT, 6));
    people.push_back(ti::Person(ti::START::LEFT, ti::TYPE::GREEN_SHIRT, 7));
    people.push_back(ti::Person(ti::START::RIGHT, ti::TYPE::GREEN_SHIRT, 8));
    people.push_back(ti::Person(ti::START::LEFT, ti::TYPE::GREEN_SHIRT, 9));

    while (true)
    {
        if(is_menu_shown){
            cursor.set_visible(true);
            if(bn::keypad::up_pressed()){
                cursor_index = cursor_index - 1;
                if(cursor_index < 0){
                    cursor_index = 0;
                }
            }
            if(bn::keypad::down_pressed()){
                cursor_index = cursor_index + 1;
                if(cursor_index > upgrades.size() - 1){
                    cursor_index = upgrades.size() - 1;
                }
            }
            cursor.set_position(get_cursor_pos(cursor_index));
            if(bn::keypad::a_pressed()){
                const int selected_price = prices.at(cursor_index);
                if(selected_price > 0 && selected_price <= cash){
                    cash = cash - selected_price; 
                    upgrades.at(cursor_index).set_visible(!upgrades.at(cursor_index).visible());
                    prices.at(cursor_index) = 0;
                    redraw_wishlist(text_generator, text_sprites, prices);
                    popularity_level = popularity_level + 1;
                    popularity_bar.set_item(bn::sprite_items::popularity_bar, popularity_level);
                    is_menu_shown = false;
                    map2.set_visible(false);
                    text_sprites.clear();
                    twinkle.set_position(upgrades.at(cursor_index).position());
                    twinkle.set_visible(true);
                    bn::sound_items::sparkle.play(0.8);
                    twinkle_action = bn::create_sprite_animate_action_once(
                        twinkle, 4, bn::sprite_items::twinkle.tiles_item(), 0,1,2,3,4,5,6);
                }
            }
        } else {
            cursor.set_visible(false);
            if(bn::keypad::a_pressed()){
                if(!is_menu_shown){
                    cursor_index = 0;
                    is_menu_shown = true;
                    map2.set_visible(true);
                    redraw_wishlist(text_generator, text_sprites, prices);
                }
            }
        }

        if(bn::keypad::b_pressed() && is_menu_shown){
            is_menu_shown = false;
            map2.set_visible(false);
            text_sprites.clear();
        }
        
        if(true){
            cash_text_sprites.clear();
            text_generator.set_palette_item(bn::sprite_palette_items::white_text_palette);
            text_generator.set_right_alignment();
            text_generator.generate(-21, -71, "$" + bn::to_string<8>(cash), cash_text_sprites);
            text_generator.set_palette_item(bn::sprite_palette_items::black_text_palette);
        }

        if (bustle_timer > 60*29){
            bustle_timer = 0;
            bn::sound_items::bustle.play(0.1 + bn::fixed(popularity_level)/20);
        } else {
            bustle_timer = bustle_timer + 1;
        }
        timer = timer - 1;
        if (timer < 0){
            if(rng.get_int(100) > 60){
                barista.set_item(bn::sprite_items::barista, rng.get_int(5));
            }
            if(rng.get_int(100) > 60){
                till.set_item(bn::sprite_items::till, rng.get_int(3));
            }
            if(rng.get_int(100) > 92){
                if(steamAction.done()){
                    bn::sound_items::steam.play(0.6);
                    steamAction = bn::create_sprite_animate_action_once(
                            steam, 5, bn::sprite_items::steam.tiles_item(), 0,1,2,3,4,5,6);
                    steam.set_visible(true);
                }
            }
            if(rng.get_int(100) > 90){
                if(drinkerAction.done()){
                    drinkerAction = bn::create_sprite_animate_action_once(
                        drinker, 15, bn::sprite_items::drinker.tiles_item(), 0,1,2,1,0);
                }
            }
            if(rng.get_int(100) < 90){
                talkative.set_item(bn::sprite_items::talkative, rng.get_int(4));
            }
            
            if(typistAction.done()){
                if(rng.get_int(100) > 80){
                    typistAction = bn::create_sprite_animate_action_forever(
                        upgrades.at(8), 8, bn::sprite_items::typist.tiles_item(), 0,1);
                }
            } else {
                if(rng.get_int(100) > 80){
                    typistAction = bn::create_sprite_animate_action_once(
                        upgrades.at(8), 8, bn::sprite_items::typist.tiles_item(), 2,2);
                }
            }

            if(rng.get_int(100) > 80){
                if(pigeonAction.done()){
                    pigeonAction = bn::create_sprite_animate_action_once(
                        pigeon, 15, bn::sprite_items::pigeon.tiles_item(), 0,1,0,1,0);
                }
            }
            if(rng.get_int(100) < 20){
                if(pigeon2Action.done()){
                    pigeon2Action = bn::create_sprite_animate_action_once(
                        pigeon2, 15, bn::sprite_items::pigeon2.tiles_item(), 0,1,0,1,0);
                }
            }
            // if(is_paused){
            //     if(rng.get_int(100) < 10){
            //         is_paused = false;
            //         is_left = !is_left;
            //         swallowAction = bn::create_sprite_animate_action_forever(
            //             swallow, 8, bn::sprite_items::swallow.tiles_item(), 1,2);
            //     }
            // }
            timer = 30;
        }

        // if(!is_paused){
        //     if(is_left){
        //         swallow.set_horizontal_flip(true);
        //         swallow.set_x(swallow.x() - 0.8);
        //         if(swallow.x() < 69){
        //             is_paused = true;
        //             swallow.set_x(69);
        //             swallowAction = bn::create_sprite_animate_action_forever(
        //                 swallow, 4, bn::sprite_items::swallow.tiles_item(), 0,0);
        //         }
        //     } else {
        //         swallow.set_horizontal_flip(false);
        //         swallow.set_x(swallow.x() + 0.8);
        //         if(swallow.x() > 140){
        //             is_paused = true;
        //             swallow.set_x(140);
        //             swallowAction = bn::create_sprite_animate_action_forever(
        //                 swallow, 4, bn::sprite_items::swallow.tiles_item(), 0,0);
        //         }
        //     }
        // }
        if(!twinkle_action.done()){
            twinkle_action.update();
        }
        if(!steamAction.done()){
            steamAction.update();
        }
        if(!drinkerAction.done()){
            drinkerAction.update();
        }
        if(!reflectAction1.done()){
            reflectAction1.update();
            // reflectAction2.update();
        } else {
            if(rng.get_int(1000) > 998){
                reflectAction1 = bn::create_sprite_animate_action_once(
                    reflect1, 4, bn::sprite_items::reflect.tiles_item(), 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14);
                // reflectAction2 = bn::create_sprite_animate_action_once(
                //     reflect2, 4, bn::sprite_items::reflect.tiles_item(), 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14);
            }
        }
        if(!pigeonAction.done()){
           pigeonAction.update(); 
        }
        if(!pigeon2Action.done()){
           pigeon2Action.update(); 
        }
        // swallowAction.update();
        sylvesterAction.update();
        if(!typistAction.done()){
            typistAction.update();
        }
        bn::vector<int, 16> types;
        for(int i = 0; i < 14; i++){
            types.push_back(i);
        }
        for(int i = 0; i < popularity_level; i++){
            for(int j = 0; j < types.size(); j++){
                if((int) people.at(i).get_type() == types.at(j)){
                    types.erase(types.begin() + j);
                }
            }
        }
        for(int i = 0; i < people.size(); i++){
            if(popularity_level > i){
                people.at(i).update(order_queue, waiting_spot, purchased_this_frame, types);
            }
        }
        clockAction.update();

        if(cash_sprite.visible()){
            cash_sprite.set_y(cash_sprite.y() - 0.2);
            if(cash_sprite.y() < -14){
                cash_sprite.set_visible(false);
                cash_sprite.set_y(-7);
            }
        }

        if(purchased_this_frame){
            cash += rng.get_int(3) + 3;
            cash_sprite.set_visible(true);
            bn::sound_items::cash.play(0.8);
            purchased_this_frame = false;
        }
        bn::core::update();
        rng.get();
    }
}
