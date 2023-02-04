#include "bn_core.h"
#include "bn_sprite_animate_actions.h"
#include "bn_regular_bg_ptr.h"
#include "bn_display.h"
#include "bn_keypad.h"
#include "bn_random.h"
#include "bn_blending.h"

#include "bn_regular_bg_items_bg1.h"
#include "bn_sprite_items_title.h"
#include "bn_sprite_items_steam.h"
#include "bn_sprite_items_barista.h"
#include "bn_sprite_items_drinker.h"
#include "bn_sprite_items_waiting.h"
#include "bn_sprite_items_sylvester.h"
#include "bn_sprite_items_blocker.h"
#include "bn_sprite_items_talkative.h"

#include "ti_person.h"

int main()
{
    bn::core::init();

    // map
    bn::regular_bg_ptr map = bn::regular_bg_items::bg1.create_bg(0, 0);

    // sprite
    bn::sprite_ptr title = bn::sprite_items::title.create_sprite(-10, -22);
    bn::sprite_ptr steam = bn::sprite_items::steam.create_sprite(-70, -8);
    steam.set_blending_enabled(true);
    bn::blending::set_transparency_alpha(0.5);
    bn::sprite_ptr barista = bn::sprite_items::barista.create_sprite(-97, -5);
    bn::sprite_ptr drinker = bn::sprite_items::drinker.create_sprite(-105, 44);
    bn::sprite_ptr talkative = bn::sprite_items::talkative.create_sprite(-83, 42);
    bn::sprite_ptr sylvester = bn::sprite_items::sylvester.create_sprite(-12, 23);

    bn::sprite_ptr blocker = bn::sprite_items::blocker.create_sprite(58, 42);
    blocker.set_z_order(-200);

    // bn::sprite_ptr walk = bn::sprite_items::walk1.create_sprite(-140, 60);
    // walk.set_z_order(-300);
    
    // bn::sprite_ptr shadow = bn::sprite_items::shadow.create_sprite(-140, 75);
    // shadow.set_z_order(50);
    

    int timer = 40;
    bn::random rng = bn::random();

    //animation action
    // bn::sprite_animate_action<8> walkerAction = bn::create_sprite_animate_action_forever(
    //                 walk, 12, bn::sprite_items::walk1.tiles_item(), 8,9,10,11,12,13,14,15);
    bn::sprite_animate_action<7> steamAction = bn::create_sprite_animate_action_once(
                    steam, 5, bn::sprite_items::steam.tiles_item(), 0,1,2,3,4,5,6);
    steam.set_visible(false);
    bn::sprite_animate_action<5> drinkerAction = bn::create_sprite_animate_action_once(
                    drinker, 15, bn::sprite_items::drinker.tiles_item(), 0,0,0,0,0);
    bn::sprite_animate_action<10> sylvesterAction = bn::create_sprite_animate_action_forever(
                    sylvester, 18, bn::sprite_items::sylvester.tiles_item(), 0,1,2,3,4,5,6,7,8,9);

    ti::Person person1 = ti::Person(ti::START::LEFT, ti::TYPE::GREEN_SHIRT);
    ti::Person person2 = ti::Person(ti::START::RIGHT, ti::TYPE::RED_SINGLET);

    while (true)
    {
        // walk.set_x(walk.x() + 0.3);
        // if(walk.x() > 140){
        //     walk.set_x(-140);
        // }
        // shadow.set_x(walk.x());

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
            if(rng.get_int(100) < 50){
                talkative.set_item(bn::sprite_items::talkative, rng.get_int(4));
            }
            timer = 30;
        }

        if(!steamAction.done()){
            steamAction.update();
        }
        if(!drinkerAction.done()){
            drinkerAction.update();
        }
        sylvesterAction.update();
        person1.update();
        person2.update();
        bn::core::update();
    }
}
