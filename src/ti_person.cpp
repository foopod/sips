#include "ti_person.h"

#include "bn_sprite_builder.h"
#include "bn_math.h"
#include "bn_log.h"

#include "bn_sprite_items_walk1.h"
#include "bn_sprite_items_walk2.h"
#include "bn_sprite_items_walk3.h"
#include "bn_sprite_items_walk4.h"
#include "bn_sprite_items_walk5.h"
#include "bn_sprite_items_walk6.h"
#include "bn_sprite_items_walk7.h"
#include "bn_sprite_items_walk8.h"
#include "bn_sprite_items_walk9.h"
#include "bn_sprite_items_walk10.h"
#include "bn_sprite_items_walk11.h"
#include "bn_sprite_items_walk12.h"
#include "bn_sprite_items_walk13.h"
#include "bn_sprite_items_walk14.h"
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
    builder.set_z_order(-5);
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

  Person::Person(START start, TYPE type, int id)
      : _shadow(bn::sprite_items::shadow.create_sprite(0,0)), _id(id)
  {
    LOCATIONS.push_back(bn::fixed_point(-60, 12));
    LOCATIONS.push_back(bn::fixed_point(-55, 15));
    LOCATIONS.push_back(bn::fixed_point(-50, 18));
    LOCATIONS.push_back(bn::fixed_point(-45, 21));
    LOCATIONS.push_back(bn::fixed_point(-40, 24));

    bn::random rng = bn::random();
    for(int i = 0; i < _id; i++){
        rng.get();
    }
    _speed += rng.get_fixed(0.2) - 0.1;

    bn::fixed_point pos = bn::fixed_point(-160, 60);
    _state = STATE::WALKING_LEFT_W_COFFEE;
    if(start == START::RIGHT){
        pos.set_x(160);
        _state = STATE::WALKING_RIGHT_W_COFFEE;
    } else if(start == START::COUNTER){
        pos = bn::fixed_point(-86, 14);
        _state = STATE::WAITING;
    }

    setStyle(type, start, pos);

    if(start == START::RIGHT){
        _sprite.value().set_horizontal_flip(true);
    } else if(start == START::COUNTER) {
        _sprite.value().set_horizontal_flip(true);
        _sprite.value().set_z_order(-100);
        _action = bn::create_sprite_animate_action_forever(_sprite.value(), 20, 
                _sprite_item.value().tiles_item(), 16,16,16,16,17,17,17,17);
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
    } else if(type == TYPE::GIRL3){
        _sprite_item = bn::sprite_items::walk9;
    } else if(type == TYPE::PERSON1){
        _sprite_item = bn::sprite_items::walk10;
    } else if(type == TYPE::PERSON2){
        _sprite_item = bn::sprite_items::walk11;
    } else if(type == TYPE::PERSON3){
        _sprite_item = bn::sprite_items::walk12;
    } else if(type == TYPE::PERSON4){
        _sprite_item = bn::sprite_items::walk13;
    } else if(type == TYPE::PERSON5){
        _sprite_item = bn::sprite_items::walk14;
    }

    _sprite = _create_sprite(pos, start != START::RIGHT, _sprite_item.value());
    _action = bn::create_sprite_animate_action_forever(_sprite.value(), 12, 
                _sprite_item.value().tiles_item(), 0,1,2,3,4,5,6,7);
  }

  TYPE Person::get_type(){
    return _type;
  }

  void Person::update(bn::deque<int, 8> &order_queue, bool &waiting_spot, bool &purchased_this_frame, bn::vector<int, 16> &types)
  {

    if(_state == STATE::WALKING_RIGHT){
        bn::fixed_point next_step = _get_next_step(_sprite.value().position(), OUTSIDE, _speed);
        _sprite.value().set_position(next_step);
        if(OUTSIDE.x() == next_step.x() && OUTSIDE.y() == next_step.y()){
            _state = STATE::ENTERING;
            _sprite.value().set_horizontal_flip(true);
        }
    } else if(_state == STATE::WALKING_LEFT){
        bn::fixed_point next_step = _get_next_step(_sprite.value().position(), OUTSIDE, _speed);
        _sprite.value().set_position(next_step);
        if(OUTSIDE.x() == next_step.x() && OUTSIDE.y() == next_step.y()){
            _state = STATE::ENTERING;
            _sprite.value().set_horizontal_flip(true);
        }
    } else if(_state == STATE::ENTERING){
        bn::fixed_point next_step = _get_next_step(_sprite.value().position(), DOOR, _speed);
        _sprite.value().set_position(next_step);
        if(DOOR.x() == next_step.x() && DOOR.y() == next_step.y()){
            _state = STATE::WALKING_TO_ORDER;
            _sprite.value().set_z_order(-100);
            _sprite.value().set_horizontal_flip(true);
        }
    } else if(_state == STATE::WALKING_TO_ORDER){
        bn::fixed_point next_step = _get_next_step(_sprite.value().position(), TILL, _speed);

        // check if in queue
        int index = -1;
        for(int i = 0; i < order_queue.size(); i++){
            if(order_queue.at(i) == _id){
                index = i;
            }
        }

        if(index == -1){
            if(order_queue.size() >= 5){
                //angry and walks out
                _state = STATE::WALKING_TO_DOOR;
                _sprite.value().set_horizontal_flip(false);
                return;
            } else {
                // add to queue
                order_queue.push_back(_id);
            }
        } else {
            // walk to spot in queue
            next_step = _get_next_step(_sprite.value().position(), LOCATIONS.at(index), _speed);
        }

        if(_sprite.value().position().x() == next_step.x() && _sprite.value().position().y() == next_step.y()){
            _state = STATE::WAITING_TO_ORDER;
            _action = bn::create_sprite_animate_action_forever(_sprite.value(), 20, 
                _sprite_item.value().tiles_item(), 16,16,16,16,17,17,17,17);
            _sprite.value().set_horizontal_flip(true);
        }

        _sprite.value().set_position(next_step);
    } else if(_state == STATE::WAITING_TO_ORDER){
        // get position in queue
        int index = -1;
        for(int i = 0; i < order_queue.size(); i++){
            if(order_queue.at(i) == _id){
                index = i;
            }
        }
        
        // try to move up
        bn::fixed_point next_step = _get_next_step(_sprite.value().position(), LOCATIONS.at(index), _speed);

        // if next start ordering
        if(_sprite.value().position().x() == next_step.x() && _sprite.value().position().y() == next_step.y()){
            if(index == 0){
                _state = STATE::ORDERING;
            }
            _action = bn::create_sprite_animate_action_forever(_sprite.value(), 20, 
                _sprite_item.value().tiles_item(), 16,16,16,16,17,17,17,17);
        } else {
            _action = bn::create_sprite_animate_action_forever(_sprite.value(), 12, 
                    _sprite_item.value().tiles_item(), 0,1,2,3,4,5,6,7);
        }
        _sprite.value().set_position(next_step);
    } else if(_state == STATE::ORDERING){
        _wait_time = _wait_time += 1;
        if( _wait_time > _wait_max){
            purchased_this_frame = true;
            _wait_time = 0;
            _state = STATE::WALKING_TO_COUNTER;
            order_queue.pop_front();
            _action = bn::create_sprite_animate_action_forever(_sprite.value(), 12, 
                _sprite_item.value().tiles_item(), 0,1,2,3,4,5,6,7);
            _sprite.value().set_horizontal_flip(true);
        }
    } else if(_state == STATE::WALKING_TO_COUNTER){
        bn::fixed_point counter = COUNTER1;
        if(waiting_spot){
            counter = COUNTER2;
        }
        bn::fixed_point next_step = _get_next_step(_sprite.value().position(), counter, _speed);
        _sprite.value().set_position(next_step);

        if(counter.x() == next_step.x() && counter.y() == next_step.y()){
            _state = STATE::WAITING;
            waiting_spot = !waiting_spot;
            _action = bn::create_sprite_animate_action_forever(_sprite.value(), 20, 
                _sprite_item.value().tiles_item(), 16,16,16,16,17,17,17,17);
            _sprite.value().set_horizontal_flip(true);
        }
    } else if(_state == STATE::WAITING){
        _wait_time = _wait_time += 1;
        if( _wait_time > _wait_max + 60){
            _wait_time = 0;
            _state = STATE::WALKING_TO_DOOR;
            _action = bn::create_sprite_animate_action_forever(_sprite.value(), 12, 
                _sprite_item.value().tiles_item(), 8,9,10,11,12,13,14,15);
            _sprite.value().set_horizontal_flip(false);
        }
    } else if(_state == STATE::WALKING_TO_DOOR){
        bn::fixed_point next_step = _get_next_step(_sprite.value().position(), DOOR, _speed);
        _sprite.value().set_position(next_step);

        if(DOOR.x() == next_step.x() && DOOR.y() == next_step.y()){
            _state = STATE::EXITING;
            _sprite.value().set_horizontal_flip(false);
        }
    } else if(_state == STATE::EXITING){
        bn::fixed_point next_step = _get_next_step(_sprite.value().position(), OUTSIDE, _speed);
        _sprite.value().set_position(next_step);
        if(OUTSIDE.x() == next_step.x() && OUTSIDE.y() == next_step.y()){
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
        bn::fixed_point next_step = _get_next_step(_sprite.value().position(), LEFT, _speed);
        _sprite.value().set_position(next_step);

        if(LEFT.x() == next_step.x() && LEFT.y() == next_step.y()){
            _state = STATE::WALKING_RIGHT;
            int next_type = static_cast<int>(_type) + 1;
            if(next_type > 8){
                next_type = 0;
            }
            setStyle(static_cast<TYPE>(next_type), START::LEFT, _sprite.value().position());
            _sprite.value().set_horizontal_flip(false);
        }
    } else if(_state == STATE::WALKING_RIGHT_W_COFFEE){
        bn::fixed_point next_step = _get_next_step(_sprite.value().position(), RIGHT, _speed);
        _sprite.value().set_position(next_step);

        if(RIGHT.x() == next_step.x() && RIGHT.y() == next_step.y()){
            _state = STATE::WALKING_LEFT;
            int type_index = _random.get_int(types.size());
            int next_type = types.at(type_index);
            types.erase(types.begin() + type_index);
            setStyle(static_cast<TYPE>(next_type), START::RIGHT, _sprite.value().position());
            _sprite.value().set_horizontal_flip(true);
        }
    }

    _sprite.value().set_z_order(-_sprite.value().y().integer());

    _shadow.set_x(_sprite.value().x());
    _shadow.set_y(_sprite.value().y() + 15);

    if(_action.has_value() && !_action.value().done()){
        _action.value().update();
    }
    _random.get();
  }
}