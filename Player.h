#ifndef MAIN_PLAYER_H
#define MAIN_PLAYER_H

#include "Image.h"
#include <GLFW/glfw3.h>

struct Point
{
  int x;
  int y;
};

enum class MovementDir
{
  UP,
  DOWN,
  LEFT,
  RIGHT
};

constexpr GLsizei WINDOW_WIDTH = 2560, WINDOW_HEIGHT = 1080;

struct Player
{
  Point coords {.x = 10, .y = 10};
  Image imageForMoving1 = Image("./resources/moving1.png");
  Image imageForMoving2 = Image("./resources/moving2.png");
  Image imageForMoving3 = Image("./resources/moving3.png");
  explicit Player(Point pos = {.x = 10, .y = 10}) :
                 coords(pos), old_coords(coords) {};

  bool Moved() const;
  void ProcessInput(MovementDir dir);
  void Draw(Image &screen);
  void DrawMoving1(Image &screen);

private:
  Image image = Image("./resources/player.png");
  Point old_coords {.x = 10, .y = 10};
  Pixel color {.r = 255, .g = 255, .b = 0, .a = 255};
  int move_speed = 1;

};

#endif //MAIN_PLAYER_H
