#include "ti_person.h"

#include "bn_sprite_builder.h"
#include "bn_math.h"
#include "bn_log.h"
#include "bn_sprite_animate_actions.h"

#include "bn_sprite_items_walk1.h"
#include "bn_sprite_items_walk2.h"
#include "bn_sprite_items_walk3.h"
#include "bn_sprite_items_walk4.h"
#include "bn_sprite_items_walk5.h"
#include "bn_sprite_items_walk6.h"
#include "bn_sprite_items_walk7.h"
#include "bn_sprite_items_walk8.h"
#include "bn_sprite_items_shadow.h"

namespace
{
  bn::sprite_ptr _create_sprite(bn::fixed_point position, bool is_left, bn::sprite_item sprite)
  {
    bn::sprite_builder builder(sprite);
    builder.set_position(position);
    builder.set_z_order(-300);
    builder.set_horizontal_flip(is_left);
    return builder.release_build();
  }

  bn::sprite_ptr _create_shadow(bn::fixed_point position)
  {
    bn::sprite_builder builder(bn::sprite_items::shadow);
    builder.set_position(position);
    builder.set_z_order(50);
    builder.set_blending_enabled(true);
    bn::blending::set_transparency_alpha(0.5);
    return builder.release_build();
  }

  bn::fixed_point _get_next_step(bn::fixed_point from, bn::fixed_point to, bn::fixed speed){

    bn::fixed diff_x = from.x() - to.x();
    bn::fixed diff_y = from.y() - to.y();

    if (bn::abs(diff_x) > 2 || bn::abs(diff_y) > 2){
        bn::fixed angle = bn::degrees_atan2(diff_y.integer(), diff_x.integer());
        bn::pair<bn::fixed, bn::fixed> xy = bn::degrees_sin_and_cos(angle);

        return bn::fixed_point(from.x() - speed * xy.second, from.y() - speed * xy.first);
    } else {
        return to;
    }
  }
}

namespace ti
{

  Person::Person(START start, TYPE type)
      : _shadow(bn::sprite_items::shadow.create_sprite(0,0))
  {
    bn::fixed_point pos = bn::fixed_point(-160, 60);
    _state = STATE::WALKING_RIGHT;
    if(start == START::RIGHT){
        pos.set_x(160);
        _state = STATE::WALKING_LEFT;
    }

    setStyle(type, start, pos);

    if(start == START::RIGHT){
        _sprite.value().set_horizontal_flip(true);
    } else {
        _sprite.value().set_horizontal_flip(false);
    }

    pos.set_y(pos.y() + 15);
    _shadow = _create_shadow(pos);
  }

  void Person::setStyle(TYPE type, START start, bn::fixed_point pos)
  {
    _type = type;

    if(type == TYPE::GREEN_SHIRT){
        _sprite_item = bn::sprite_items::walk1; 
    } else if(type == TYPE::RED_SHIRT){
        _sprite_item = bn::sprite_items::walk2;
    } else if(type == TYPE::BLUE_SHIRT){
        _sprite_item = bn::sprite_items::walk3;
    } else if(type == TYPE::RED_SINGLET){
        _sprite_item = bn::sprite_items::walk4;
    } else if(type == TYPE::PALE_GREEN_SHIRT){
        _sprite_item = bn::sprite_items::walk5;
    } else if(type == TYPE::DWIGHT){
        _sprite_item = bn::sprite_items::walk6;
    } else if(type == TYPE::GIRL1){
        _sprite_item = bn::sprite_items::walk8;
    } else if(type == TYPE::GIRL2){
        _sprite_item = bn::sprite_items::walk7;
    }

    _sprite = _create_sprite(pos, start == START::LEFT, _sprite_item.value());
    _action = bn::create_sprite_animate_action_forever(_sprite.value(), 12, 
                _sprite_item.value().tiles_item(), 0,1,2,3,4,5,6,7);
  }

  void Person::update()
  {
    bn::fixed_point counter = bn::fixed_point(-86, 14);
    bn::fixed_point door = bn::fixed_point(88,36);
    bn::fixed_point outside = bn::fixed_point(100,60);
    bn::fixed_point left = bn::fixed_point(-140,60);
    bn::fixed_point right = bn::fixed_point(180,60);

    // BN_LOG(static_cast<int>(_state));

    if(_state == STATE::WALKING_RIGHT){
        bn::fixed_point next_step = _get_next_step(_sprite.value().position(), outside, _speed);
        _sprite.value().set_position(next_step);
        if(outside.x() == next_step.x() && outside.y() == next_step.y()){
            _state = STATE::ENTERING;
            _sprite.value().set_horizontal_flip(true);
        }
    } else if(_state == STATE::WALKING_LEFT){
        bn::fixed_point next_step = _get_next_step(_sprite.value().position(), outside, _speed);
        _sprite.value().set_position(next_step);
        if(outside.x() == next_step.x() && outside.y() == next_step.y()){
            _state = STATE::ENTERING;
            _sprite.value().set_horizontal_flip(true);
        }
    } else if(_state == STATE::ENTERING){
        bn::fixed_point next_step = _get_next_step(_sprite.value().position(), door, _speed);
        _sprite.value().set_position(next_step);
        if(door.x() == next_step.x() && door.y() == next_step.y()){
            _state = STATE::WALKING_TO_COUNTER;
            _sprite.value().set_z_order(-100);
            _sprite.value().set_horizontal_flip(true);
        }
    } else if(_state == STATE::WALKING_TO_COUNTER){
        bn::fixed_point next_step = _get_next_step(_sprite.value().position(), counter, _speed);
        _sprite.value().set_position(next_step);

        if(counter.x() == next_step.x() && counter.y() == next_step.y()){
            _state = STATE::WAITING;
            _action = bn::create_sprite_animate_action_forever(_sprite.value(), 20, 
                _sprite_item.value().tiles_item(), 16,16,16,16,17,17,17,17);
            _sprite.value().set_horizontal_flip(true);
        }
    } else if(_state == STATE::WAITING){
        _wait_time = _wait_time += 1;
        if( _wait_time > _wait_max){
            _wait_time = 0;
            _state = STATE::WALKING_FROM_COUNTER;
            _action = bn::create_sprite_animate_action_forever(_sprite.value(), 12, 
                _sprite_item.value().tiles_item(), 8,9,10,11,12,13,14,15);
            _sprite.value().set_horizontal_flip(false);
        }
    } else if(_state == STATE::WALKING_FROM_COUNTER){
        bn::fixed_point next_step = _get_next_step(_sprite.value().position(), door, _speed);
        _sprite.value().set_position(next_step);

        if(door.x() == next_step.x() && door.y() == next_step.y()){
            _state = STATE::EXITING;
            _sprite.value().set_horizontal_flip(false);
        }
    } else if(_state == STATE::EXITING){
        bn::fixed_point next_step = _get_next_step(_sprite.value().position(), outside, _speed);
        _sprite.value().set_position(next_step);
        if(outside.x() == next_step.x() && outside.y() == next_step.y()){
            bool is_left = _random.get_int(10) > 5;
            if(is_left){
                _state = STATE::WALKING_LEFT_W_COFFEE;
                _sprite.value().set_horizontal_flip(true);
            } else {
                _state = STATE::WALKING_RIGHT_W_COFFEE;
                _sprite.value().set_horizontal_flip(false);
            }
        }
        _sprite.value().set_z_order(-300);
    } else if(_state == STATE::WALKING_LEFT_W_COFFEE){
        bn::fixed_point next_step = _get_next_step(_sprite.value().position(), left, _speed);
        _sprite.value().set_position(next_step);

        if(left.x() == next_step.x() && left.y() == next_step.y()){
            _state = STATE::WALKING_RIGHT;
            int next_type = static_cast<int>(_type) + 1;
            if(next_type > 6){
                next_type = 0;
            }
            setStyle(static_cast<TYPE>(next_type), START::LEFT, _sprite.value().position());
            _sprite.value().set_horizontal_flip(false);
        }
    } else if(_state == STATE::WALKING_RIGHT_W_COFFEE){
        bn::fixed_point next_step = _get_next_step(_sprite.value().position(), right, _speed);
        _sprite.value().set_position(next_step);

        if(right.x() == next_step.x() && right.y() == next_step.y()){
            _state = STATE::WALKING_LEFT;
            int next_type = static_cast<int>(_type) + 1;
            if(next_type > 6){
                next_type = 0;
            }
            setStyle(static_cast<TYPE>(next_type), START::RIGHT, _sprite.value().position());
            _sprite.value().set_horizontal_flip(true);
        }
    }

    _shadow.set_x(_sprite.value().x());
    _shadow.set_y(_sprite.value().y() + 15);

    if(_action.has_value() && !_action.value().done()){
        _action.value().update();
    }
  }
}