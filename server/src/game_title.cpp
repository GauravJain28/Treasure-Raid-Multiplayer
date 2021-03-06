#include "game.hpp"
#include <SDL2/SDL_mixer.h>
#include <time.h>
#include <sstream>
#include "global.hpp"
#include "enemy.hpp"
#include "font.hpp"
#include "coins.hpp"
#include "image.hpp"
#include "input.hpp"
#include "image.hpp"
#include "map.hpp"
#include "sound_manager.hpp"
#include "player.hpp"


void Game::game_title(){
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderClear(renderer);

  const Point title_pos = Point{200, 100};
  const Point p1_mode_pos = Point{185, 300};
  const Point vs_mode_pos = Point{180, 420};
  const char *title_str = "Treasure Raid";
  const char *p1_mode_str = "SINGLE PLAYER MODE";
  const char *vs_mode_str = "DOUBLE PLAYER MODE";
  const SDL_Rect dst = {0, 0, screen::width, screen::height};
  const SDL_Rect p1_str_dst = {350, 290, 155, 80};
  const SDL_Rect p2_str_dst = {350, 410, 155, 80};

  SDL_Texture *trans_texture = image_manager_->get(image::fullbg);
  SDL_RenderCopy(renderer, trans_texture, nullptr, &dst);
  
  switch (game_count_) {
    
    case 0: {
      text(font_size::x96, rgb::tc, title_pos, title_str);
      if (blink_count_ < 30) {
        text(font_size::x48, rgb::tc, Point{240,540}, "press enter to continue");
        ++blink_count_;
      } else if (blink_count_ < 60) {
        ++blink_count_;
      } else {
        blink_count_ = 0;
      }

      switch (menu_option_){

        case 0:{
          SDL_SetRenderDrawColor(renderer, 255, 235, 175, 255);
          SDL_RenderFillRect(renderer, &p1_str_dst);
      
          text(font_size::x64, rgb::blink, Point{360,300}, "START");
          text(font_size::x64, rgb::blink, Point{375,420}, "HELP");

          if (input_manager_->edge_key_p(player_type::p1, input_device::x)
          || input_manager_->press_key_p(player_type::p2, input_device::x)
          || input_manager_->edge_key_p(player_type::p1, input_device::enter)) {
            ++game_count_;
            blink_count_ = 0;
          }
          break;
        }

        case 1:{
          SDL_SetRenderDrawColor(renderer, 255, 235, 175, 255);
          SDL_RenderFillRect(renderer, &p2_str_dst);
          text(font_size::x64, rgb::blink, Point{360,300}, "START");
          text(font_size::x64, rgb::blink, Point{375,420}, "HELP");

          if (input_manager_->edge_key_p(player_type::p1, input_device::x)
          || input_manager_->press_key_p(player_type::p2, input_device::x)
          || input_manager_->edge_key_p(player_type::p1, input_device::enter)) {
            game_count_=4;
            blink_count_ = 0;
          }
          
          break;
        }

        default:
          break;
      }

      //move to next or prev option
      if (input_manager_->press_key_p(player_type::p1, input_device::down)
      || input_manager_->press_key_p(player_type::p2, input_device::down)) {
          menu_option_=1;
      } else if (input_manager_->press_key_p(player_type::p1, input_device::up)
      || input_manager_->press_key_p(player_type::p2, input_device::up)) {
          menu_option_=0;
      }

      text(font_size::x36, rgb::foot, Point{200,780}, "developed by: gaurav jain and t abishek");

      break;
    }
    case 1: {
      text(font_size::x96, rgb::tc, title_pos, title_str);
      if (!input_manager_->press_key_p(player_type::p1, input_device::x)
          && !input_manager_->press_key_p(player_type::p2, input_device::x)
          && !input_manager_->press_key_p(player_type::p1, input_device::enter)) {
        ++game_count_;
      }
      break;
    }
    case 2: {
      text(font_size::x96, rgb::tc, Point{120,100}, "Select Game Mode");
      if (blink_count_ < 30) {
        text(font_size::x48, rgb::tc, Point{240,540}, "press enter to continue");
        text(font_size::x48, rgb::tc, Point{245,600}, "press space to go back");
        ++blink_count_;
      } else if (blink_count_ < 60) {
        ++blink_count_;
      } else {
        blink_count_ = 0;
      }

      switch (game_mode_) {
        case 0: {
          const SDL_Rect dst = {170, 290, 500, 80};
          SDL_SetRenderDrawColor(renderer, 255, 235, 175, 255);
          SDL_RenderFillRect(renderer, &dst);
          text(font_size::x64, rgb::blink, p1_mode_pos, p1_mode_str);
          text(font_size::x64, rgb::blink, vs_mode_pos, vs_mode_str);
          break;
        }
        case 1: {
          const SDL_Rect dst = {170, 410, 500, 80};
          SDL_SetRenderDrawColor(renderer, 255, 235, 175, 255);
          SDL_RenderFillRect(renderer, &dst);
          text(font_size::x64, rgb::blink, p1_mode_pos, p1_mode_str);
          text(font_size::x64, rgb::blink, vs_mode_pos, vs_mode_str);
          break;
        }
      }

      //continue to next/prev screen
      if (input_manager_->press_key_p(player_type::p1, input_device::x)
          || input_manager_->press_key_p(player_type::p2, input_device::x)
          || input_manager_->press_key_p(player_type::p1, input_device::enter)) {
        ++game_count_;
      }
      else if (input_manager_->press_key_p(player_type::p1, input_device::x)
          || input_manager_->press_key_p(player_type::p2, input_device::x)
          || input_manager_->press_key_p(player_type::p1, input_device::space)) {
        game_count_=0;
      }

      //select between the game modes
      if (input_manager_->press_key_p(player_type::p1, input_device::down)
          || input_manager_->press_key_p(player_type::p2, input_device::down)) {
        game_mode_ = 1;
      } else if (input_manager_->press_key_p(player_type::p1, input_device::up)
                 || input_manager_->press_key_p(player_type::p2, input_device::up)) {
        game_mode_ = 0;
      }
      break;
    }
    case 3: {
    //   switch (game_mode_) {
    //     case 0: {
    //       SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    //       //SDL_RenderFillRect(renderer, &p1_str_dst);
    //       text(font_size::x36, rgb::black, p1_mode_pos, p1_mode_str);
    //       text(font_size::x36, rgb::black, vs_mode_pos, vs_mode_str);
    //       break;
    //     }
    //     case 1: {
    //       SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    //       //SDL_RenderFillRect(renderer, &vs_str_dst);
    //       text(font_size::x36, rgb::black, p1_mode_pos, p1_mode_str);
    //       text(font_size::x36, rgb::black, vs_mode_pos, vs_mode_str);
    //       break;
    //     }
    //   }

      // initialize globals
      
      map->init(testf, peer1,game_mode_);
      coin->init(map.get(), game_mode_);
        //enemy_->init();
      p1->init(map.get(), true);
      p2->init(map.get(), true);
      enemy->init();
      p1->set_life(2);
      p2->set_life(2);
      p1->set_score(0);
      p2->set_score(0);
      p1->set_damaged(false);
      p2->set_damaged(false);
        //p1_->set_power_mode(0);
        //p2_->set_power_mode(0);

      game_count_ = 0;
      game_state_ = game_state::start;
    
      srand((unsigned int)time(nullptr));

      Mix_PlayMusic(sound_manager_->get_music(), -1);
      break;
    }

    case 4: {
      text(font_size::x96, rgb::tc, title_pos, title_str);
      if (!input_manager_->press_key_p(player_type::p1, input_device::x)
          && !input_manager_->press_key_p(player_type::p2, input_device::x)
          && !input_manager_->press_key_p(player_type::p1, input_device::enter)) {
        ++game_count_;
      }
      break;
    }
    case 5:{
      text(font_size::x96, rgb::tc, Point{360,100}, "Help");
      if (blink_count_ < 30) {
        text(font_size::x48, rgb::tc, Point{250,600}, "press space to go back");
        ++blink_count_;
      } else if (blink_count_ < 60) {
        ++blink_count_;
      } else {
        blink_count_ = 0;
      }

      //continue to next screen
      if (input_manager_->press_key_p(player_type::p1, input_device::x)
          || input_manager_->press_key_p(player_type::p2, input_device::x)
          || input_manager_->press_key_p(player_type::p1, input_device::space)) {
        game_count_=0;
      }

      text(font_size::x64, rgb::blink, Point{60,200}, "Controls");
        SDL_Texture *trans_texture = image_manager_->get(image::up);
        SDL_Rect dst = {295, 280, 80, 80};
        SDL_RenderCopy(renderer, trans_texture, nullptr, &dst);
        trans_texture = image_manager_->get(image::down);
        dst = {380, 280, 80, 80};
        SDL_RenderCopy(renderer, trans_texture, nullptr, &dst);
        trans_texture = image_manager_->get(image::left);
        dst = {465, 280, 80, 80};
        SDL_RenderCopy(renderer, trans_texture, nullptr, &dst);
        trans_texture = image_manager_->get(image::right);
        dst = {550, 280, 80, 80};
        SDL_RenderCopy(renderer, trans_texture, nullptr, &dst);
        SDL_DestroyTexture(trans_texture);
        text(font_size::x64, rgb::white, Point{220,280}, "A : ");

        SDL_Texture *trans_texture1 = image_manager_->get(image::w);
        SDL_Rect dst1 = {295, 400, 80, 80};
        SDL_RenderCopy(renderer, trans_texture1, nullptr, &dst1);
        trans_texture1 = image_manager_->get(image::s);
        dst1 = {380, 400, 80, 80};
        SDL_RenderCopy(renderer, trans_texture1, nullptr, &dst1);
        trans_texture1 = image_manager_->get(image::a);
        dst1 = {465, 400, 80, 80};
        SDL_RenderCopy(renderer, trans_texture1, nullptr, &dst1);
        trans_texture1 = image_manager_->get(image::d);
        dst1 = {550, 400, 80, 80};
        SDL_RenderCopy(renderer, trans_texture1, nullptr, &dst1);
        SDL_DestroyTexture(trans_texture1);
        text(font_size::x64, rgb::white, Point{220,400}, "B : "); 

        text(font_size::x48, rgb::white, Point{180,500}, "press X to freeze the mummies...");   
      
      break;
    }


    default:
      break;
  }

  SDL_DestroyTexture(trans_texture);
}