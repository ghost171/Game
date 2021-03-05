#include "Player.h"


bool Player::Moved() const
{
  if(coords.x == old_coords.x && coords.y == old_coords.y)
    return false;
  else
    return true;
}

void Player::ProcessInput(MovementDir dir)
{
  int move_dist = move_speed * 32;
  switch(dir)
  {
    case MovementDir::UP:
      old_coords.y = coords.y;
      coords.y += move_dist;
      break;
    case MovementDir::DOWN:
      old_coords.y = coords.y;
      coords.y -= move_dist;
      break;
    case MovementDir::LEFT:
      old_coords.x = coords.x;
      coords.x -= move_dist;
      break;
    case MovementDir::RIGHT:
      old_coords.x = coords.x;
      coords.x += move_dist;
      break;
    default:
      break;
  }
}

void Player::DrawRight(Image &screen)
{
  if(Moved())
  {
    
    for(int y = old_coords.y; y <= old_coords.y + 32; ++y)
    {
      for(int x = old_coords.x; x <= old_coords.x + 32; ++x)
      {
        if (backgroundColor.a > 0.1) {
          screen.PutPixel(x, y, backgroundColor);
        }
      }
    }
    old_coords = coords;
    
  }

  for(int y = coords.y; y <= coords.y + 32; ++y)
  {
    for(int x = coords.x; x <= coords.x + 32; ++x)
    {
      Pixel current_pixel = image.GetPixel((x - coords.x), (y - coords.y));
      if (current_pixel.a > 0.1) {
        screen.PutPixel(x, y, current_pixel);
      }
    }
  }
}

void Player::DrawLeft(Image &screen)
{
  if(Moved())
  {
    
    for(int y = old_coords.y; y <= old_coords.y + 32; ++y)
    {
      for(int x = old_coords.x; x <= old_coords.x + 32; ++x)
      {
        if (backgroundColor.a > 0.1) {
          screen.PutPixel(x, y, backgroundColor);
        }
      }
    }
    old_coords = coords;
    
  }

  for(int y = coords.y; y <= coords.y + 32; ++y)
  {
    for(int x = coords.x; x <= coords.x + 32; ++x)
    {
      Pixel current_pixel = image.GetPixel(32 - (x - coords.x), (y - coords.y));
      if (current_pixel.a > 0.1) {
        screen.PutPixel(x, y, current_pixel);
      }
    }
  }
}