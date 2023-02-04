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
#include "bn_log.h"

#include "bn_regular_bg_items_bg1.h"
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

#include "ti_person.h"

int main()
{
    bn::core::init();

    bn::music_items::wild_strawberry.play();
    bn::music::set_volume(1);

    // map
    bn::regular_bg_ptr map = bn::regular_bg_items::bg1.create_bg(0, 0);

    // sprite
    bn::sprite_ptr title = bn::sprite_items::title.create_sprite(4, -22);
    bn::sprite_ptr steam = bn::sprite_items::steam.create_sprite(-70, -8);
    steam.set_blending_enabled(true);
    bn::blending::set_transparency_alpha(0.5);
    bn::sprite_ptr barista = bn::sprite_items::barista.create_sprite(-97, -5);
    bn::sprite_ptr drinker = bn::sprite_items::drinker.create_sprite(-105, 44);
    bn::sprite_ptr talkative = bn::sprite_items::talkative.create_sprite(-83, 42);
    bn::sprite_ptr sylvester = bn::sprite_items::sylvester.create_sprite(-12, 23);
    bn::sprite_ptr pigeon = bn::sprite_items::pigeon.create_sprite(46, -71);
    bn::sprite_ptr pigeon2 = bn::sprite_items::pigeon2.create_sprite(60, -71);
    pigeon2.set_horizontal_flip(true);

    bn::sprite_ptr reflect1 = bn::sprite_items::reflect.create_sprite(3, 18);
    reflect1.set_blending_enabled(true);
    reflect1.set_z_order(-500);
    bn::sprite_ptr reflect2 = bn::sprite_items::reflect.create_sprite(-88, -5);
    reflect2.set_blending_enabled(true);
    reflect2.set_horizontal_flip(true);
    reflect2.set_vertical_flip(true);
    reflect2.set_z_order(-500);

    bn::sprite_ptr blocker = bn::sprite_items::blocker.create_sprite(58, 42);
    blocker.set_z_order(-200);

    int timer = 40;
    bn::random rng = bn::random();

    //animation action
    bn::sprite_animate_action<7> steamAction = bn::create_sprite_animate_action_once(
                    steam, 5, bn::sprite_items::steam.tiles_item(), 0,1,2,3,4,5,6);
    steam.set_visible(false);
    bn::sprite_animate_action<5> drinkerAction = bn::create_sprite_animate_action_once(
                    drinker, 15, bn::sprite_items::drinker.tiles_item(), 0,0,0,0,0);
    bn::sprite_animate_action<10> sylvesterAction = bn::create_sprite_animate_action_forever(
                    sylvester, 18, bn::sprite_items::sylvester.tiles_item(), 0,1,2,3,4,5,6,7,8,9);

    bn::sprite_animate_action<5> pigeonAction = bn::create_sprite_animate_action_once(
                    pigeon, 18, bn::sprite_items::pigeon.tiles_item(), 0,1,0,1,0);
    bn::sprite_animate_action<5> pigeon2Action = bn::create_sprite_animate_action_once(
                    pigeon2, 18, bn::sprite_items::pigeon2.tiles_item(), 0,1,0,1,0);

    bn::sprite_animate_action<15> reflectAction1 = bn::create_sprite_animate_action_once(
                    reflect1, 4, bn::sprite_items::reflect.tiles_item(), 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14);

    bn::sprite_animate_action<15> reflectAction2 = bn::create_sprite_animate_action_once(
                    reflect2, 4, bn::sprite_items::reflect.tiles_item(), 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14);

    ti::Person person1 = ti::Person(ti::START::LEFT, ti::TYPE::GREEN_SHIRT);
    ti::Person person2 = ti::Person(ti::START::RIGHT, ti::TYPE::DWIGHT);

    while (true)
    {
        timer = timer - 1;
        if (timer < 0){
            if(rng.get_int(100) > 60){
                barista.set_item(bn::sprite_items::barista, rng.get_int(5));
            }
            if(rng.get_int(100) > 80){
                if(steamAction.done()){
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
            timer = 30;
        }

        if(!steamAction.done()){
            steamAction.update();
        }
        if(!drinkerAction.done()){
            drinkerAction.update();
        }
        if(!reflectAction1.done()){
            reflectAction1.update();
            reflectAction2.update();
        } else {
            if(rng.get_int(1000) > 995){
                reflectAction1 = bn::create_sprite_animate_action_once(
                    reflect1, 4, bn::sprite_items::reflect.tiles_item(), 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14);
                reflectAction2 = bn::create_sprite_animate_action_once(
                    reflect2, 4, bn::sprite_items::reflect.tiles_item(), 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14);
            }
        }
        if(!pigeonAction.done()){
           pigeonAction.update(); 
        }
        if(!pigeon2Action.done()){
           pigeon2Action.update(); 
        }
        sylvesterAction.update();
        person1.update();
        person2.update();
        bn::core::update();
        rng.get();
    }
}
