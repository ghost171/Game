#include "common.h"
#include "Image.h"
#include "Player.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <unistd.h>
using namespace std;

#define GLFW_DLL
#include <GLFW/glfw3.h>

char BUFFER_TILE = '.';

float threshold = 0.019;

struct InputState
{
  bool keys[1024]{}; //массив состояний кнопок - нажата/не нажата
  GLfloat lastX = 400, lastY = 300; //исходное положение мыши
  bool firstMouse = true;
  bool captureMouse         = true;  // Мышка захвачена нашим приложением или нет?
  bool capturedMouseJustNow = false;
} static Input;


GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
unsigned int microsecond = 10000;


void OnKeyboardPressed(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
		break;
  case GLFW_KEY_1:
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    break;
  case GLFW_KEY_2:
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    break;
	default:
		if (action == GLFW_PRESS)
      Input.keys[key] = true;
		else if (action == GLFW_RELEASE)
      Input.keys[key] = false;
	}
}

void movingRight(Player &player, Image &screenBuffer, GLFWwindow *window, Image &imageForMoving) {

  for(int y = player.coords.y; y <= player.coords.y + 32; ++y)
  {
    for(int x = player.coords.x; x <= player.coords.x + 32; ++x)
    {
      if (imageForMoving.GetPixel((x - player.coords.x), (y - player.coords.y)).a > 0.1) {
        screenBuffer.PutPixel(x, y, imageForMoving.GetPixel((x - player.coords.x), (y - player.coords.y)));
      }
    }
  }
  usleep(10000);
  glfwPollEvents();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
  glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
  glfwSwapBuffers(window);
}

void movingLeft(Player &player, Image &screenBuffer, GLFWwindow *window, Image &imageForMoving) {

  for(int y = player.coords.y; y <= player.coords.y + 32; ++y)
  {
    for(int x = player.coords.x; x <= player.coords.x + 32; ++x)
    {
      if (imageForMoving.GetPixel(32 - (x - player.coords.x), (y - player.coords.y)).a > 0.1) {
        screenBuffer.PutPixel(x, y, imageForMoving.GetPixel(32 - (x - player.coords.x), (y - player.coords.y)));
      }
    }
  }
  usleep(10000);
  glfwPollEvents();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
  glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
  glfwSwapBuffers(window);
}

void processPlayerMovement(Player &player, Image &screenBuffer, GLFWwindow* window, char labirint[20][80])
{
  int j = (int)(float(player.coords.x) / 32);
  int i = (int)(float(player.coords.y) / 32 );
  if (Input.keys[GLFW_KEY_E]) {
    if (labirint[i - 1][j] == 'D') {
      labirint[i - 1][j] = 'O';
      return;
    }
    if (labirint[i + 1][j] == 'D') {
      labirint[i + 1][j] = 'O';
      return;
    }
    if (labirint[i][j + 1] == 'D') {
      labirint[i][j + 1] = 'O';
      return;
    }
    if (labirint[i][j - 1] == 'D') {
      labirint[i][j - 1] = 'O';
      return;
    }
    if (labirint[i - 1][j] == 'O') {
      labirint[i - 1][j] = 'D';
      return;
    }
    if (labirint[i + 1][j] == 'O') {
      labirint[i + 1][j] = 'D';
      return;
    }
    if (labirint[i][j + 1] == 'O') {
      labirint[i][j + 1] = 'D';
      return;
    }
    if (labirint[i][j - 1] == 'O') {
      labirint[i][j - 1] = 'D';
      return;
    }
  } else if (Input.keys[GLFW_KEY_W] || Input.keys[GLFW_KEY_UP]) {
    if (i != 0 && labirint[i - 1][j] != '#' && labirint[i - 1][j] != 'D') {
      //if (deltaTime < threshold) {
        //labirint[i][j] = BUFFER_TILE;
        //BUFFER_TILE = labirint[i - 1][j];
        //labirint[i - 1][j] = '@';
      //}
      player.ProcessInput(MovementDir::DOWN);
      movingRight(player, screenBuffer, window, player.imageForMoving1);
      movingRight(player, screenBuffer, window, player.imageForMoving2);
      movingRight(player, screenBuffer, window, player.imageForMoving3);
      movingRight(player, screenBuffer, window, player.imageForMoving2);
      movingRight(player, screenBuffer, window, player.imageForMoving1);
    }
    return; 
  }
  else if (Input.keys[GLFW_KEY_S] || Input.keys[GLFW_KEY_DOWN]) {
    if (i != 19 && labirint[i + 1][j] != '#' && labirint[i + 1][j] != 'D') {
      /*if (deltaTime < threshold) {
        labirint[i][j] = BUFFER_TILE;
        BUFFER_TILE = labirint[i + 1][j];
        labirint[i + 1][j] = '@';
      }*/
      player.ProcessInput(MovementDir::UP);
      movingLeft(player, screenBuffer, window, player.imageForMoving1);
      movingLeft(player, screenBuffer, window, player.imageForMoving2);
      movingLeft(player, screenBuffer, window, player.imageForMoving3);
      movingLeft(player, screenBuffer, window, player.imageForMoving2);
      movingLeft(player, screenBuffer, window, player.imageForMoving1);
    }
    return;
  }
  if (Input.keys[GLFW_KEY_A] || Input.keys[GLFW_KEY_LEFT]) {
    if (j != 0 && labirint[i][j - 1] != '#' && labirint[i][j - 1] != 'D') {
      player.ProcessInput(MovementDir::LEFT);
      movingLeft(player, screenBuffer, window, player.imageForMoving1);
      movingLeft(player, screenBuffer, window, player.imageForMoving2);
      movingLeft(player, screenBuffer, window, player.imageForMoving3);
      movingLeft(player, screenBuffer, window, player.imageForMoving2);
      movingLeft(player, screenBuffer, window, player.imageForMoving1);
    }
    return;
  }
  else if (Input.keys[GLFW_KEY_D] || Input.keys[GLFW_KEY_RIGHT]) {
    if (j != 79 && labirint[i][j + 1] != '#' && labirint[i][j + 1] != 'D') {
      player.ProcessInput(MovementDir::RIGHT);
      movingRight(player, screenBuffer, window, player.imageForMoving1);
      movingRight(player, screenBuffer, window, player.imageForMoving2);
      movingRight(player, screenBuffer, window, player.imageForMoving3);
      movingRight(player, screenBuffer, window, player.imageForMoving2);
      movingRight(player, screenBuffer, window, player.imageForMoving1);
    }
    return;
  }
}

void OnMouseButtonClicked(GLFWwindow* window, int button, int action, int mods)
{
  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    Input.captureMouse = !Input.captureMouse;

  if (Input.captureMouse)
  {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    Input.capturedMouseJustNow = true;
  }
  else
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

}

void OnMouseMove(GLFWwindow* window, double xpos, double ypos)
{
  if (Input.firstMouse)
  {
    Input.lastX = float(xpos);
    Input.lastY = float(ypos);
    Input.firstMouse = false;
  }

  GLfloat xoffset = float(xpos) - Input.lastX;
  GLfloat yoffset = Input.lastY - float(ypos);

  Input.lastX = float(xpos);
  Input.lastY = float(ypos);
}


void OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
{
  // ...
}


int initGL()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		return -1;
	}

	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

  std::cout << "Controls: "<< std::endl;
  std::cout << "press right mouse button to capture/release mouse cursor  "<< std::endl;
  std::cout << "W, A, S, D - movement  "<< std::endl;
  std::cout << "E - open the door" << endl; 
  std::cout << "press ESC to exit" << std::endl;

	return 0;
}

void shimmering (Image &screenBuffer, GLFWwindow *window, Image &imageForShimmering, int i, int j) {
  for(int y = 0; y <= 32; ++y) {
    for(int x = 0; x <= 32; ++x) {
      if (backgroundColor.a <= 0.1) {
        screenBuffer.PutPixel(j * 32 + x, i * 32 + y, imageForShimmering.GetPixel(x, y));
      }
    }
  }
  //usleep(microsecond * 4);
  glfwPollEvents();
  //player.Draw(screenBuffer);
  //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
  glRasterPos2f(-1, 1);
  glPixelZoom(1, -1);
  //glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
  glfwSwapBuffers(window);
}

int level2(Image &screenBuffer, GLFWwindow *window, Player player) {

  char labirint[20][80];
  int xOfExit, yOfExit;

  Image enter("./resources/enter.png");
  Image wall("./resources/wall.png");
  Image floor("./resources/floor.png");
  Image doorOpen("./resources/open_door.png");
  Image doorClose("./resources/close_door.png");
  Image pent1("./resources/pent1.png");
  Image pent2("./resources/pent2.png");
  Image pent3("./resources/pent3.png");
  Image exit1("./resources/next_level.png");
  Image victory("./resources/victory.jpg");

  ifstream MyFile1("Labirint2.txt");
    int i = 0;
    unsigned char text1[1600];
    while(!MyFile1.eof()) {
      MyFile1 >> text1[i];
      i++;
    }

    for (int i = 0; i < 20; i++) {
      for (int j = 0; j < 80; j++) {
        labirint[i][j] = text1[i * 80 + j]; 
      }
    }
    for (int i = 0; i < 20; i++) {
      for (int j = 0; j < 80; j++) {
        if (labirint[i][j] == '@') {
          player.coords.x = j * 32;
          player.coords.y = i * 32;
          player.old_coords.x = j * 32;
          player.old_coords.y = i * 32;
          for(int y = 0; y <= 32; ++y)
          {
            for(int x = 0; x <= 32; ++x)
            {
              Pixel current_pixel = enter.GetPixel(x, 32 - y);
              if (current_pixel.r + current_pixel.g + current_pixel.b <= 300) {
                current_pixel.a = 255;
              }
              screenBuffer.PutPixel(j * 32 + x, i * 32 + y, current_pixel);
            }
          }
        }
      }
    }
    while (!glfwWindowShouldClose(window))
    {
      for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 80; j++) {
          if (labirint[i][j] == '#') {
            for(int y = 0; y <= 32; ++y)
            {
              for(int x = 0; x <= 32; ++x)
              {
                 Pixel current_pixel = wall.GetPixel(x, y);
                if (current_pixel.r + current_pixel.g + current_pixel.b <= 300) {
                  current_pixel.a = 255;
                }
                screenBuffer.PutPixel(j * 32 + x, i * 32 + y, current_pixel);
              }
            }
            continue;
          }
          if (labirint[i][j] == '@') {
            for(int y = 0; y <= 32; ++y)
            {
              for(int x = 0; x <= 32; ++x)
              {
                Pixel current_pixel = enter.GetPixel(x, y);
                if (current_pixel.r + current_pixel.g + current_pixel.b <= 300) {
                  current_pixel.a = 255;
                }
                screenBuffer.PutPixel(j * 32 + x, i * 32 + y, current_pixel);
              }
            }
            continue;
          }
          if (labirint[i][j] == '.') {
            for(int y = 0; y <= 32; ++y)
            {
              for(int x = 0; x <= 32; ++x)
              {
                Pixel current_pixel = floor.GetPixel(x, y);
                if (current_pixel.r + current_pixel.g + current_pixel.b <= 300) {
                  current_pixel.a = 255;
                }
                screenBuffer.PutPixel(j * 32 + x, i * 32 + y, current_pixel);
              }
            }
            continue;
          }
          if (labirint[i][j] == 'x') {
            for(int y = 0; y <= 32; ++y)
            {
              for(int x = 0; x <= 32; ++x)
              {
                xOfExit = j;
                yOfExit = i;
                Pixel current_pixel = exit1.GetPixel(x, y);
                if (current_pixel.r + current_pixel.g + current_pixel.b <= 300) {
                  current_pixel.a = 255;
                }
                screenBuffer.PutPixel(j * 32 + x, i * 32 + y, current_pixel);
              }
            }
            continue;
          }
          if (labirint[i][j] == 'D') {
            for(int y = 0; y <= 32; ++y)
            {
              for(int x = 0; x <= 32; ++x)
              {
                Pixel current_pixel = doorClose.GetPixel(x, y);
                if (current_pixel.r + current_pixel.g + current_pixel.b <= 300) {
                  current_pixel.a = 255;
                }
                screenBuffer.PutPixel(j * 32 + x, i * 32 + y, current_pixel);
              }
            }
          }
          if (labirint[i][j] == 'O') {
            for(int y = 0; y <= 32; ++y)
            {
              for(int x = 0; x <= 32; ++x)
              {
                Pixel current_pixel = doorOpen.GetPixel(x, y);
                if (current_pixel.r + current_pixel.g + current_pixel.b <= 300) {
                  current_pixel.a = 255;
                }
                screenBuffer.PutPixel(j * 32 + x, i * 32 + y, current_pixel);
              }
            }
          }
        }
      }
      if (((int)(float(player.coords.x) / 32)) == xOfExit && ((int)(float(player.coords.y) / 32)) == yOfExit) {
        while (!glfwWindowShouldClose(window))
        {
          for (int y = 0; y < 32 * 20 ; y++) {
            for (int x = 0; x < 32 * 80; x++) {
              screenBuffer.PutPixel(x, y, backgroundColor);
            }
          }
          for (int x = 0; x < 900; x++) {
            for (int y = 0; y < 560; y++) {
              screenBuffer.PutPixel(x + 300, y, victory.GetPixel(x, y));
            }
          }
          glfwPollEvents();
          //processPlayerMovement(player, screenBuffer, window, labirint);
          //player.Draw(screenBuffer);
          glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
          //glRasterPos2f(-1, 1);
          //glPixelZoom(1, -1);
          glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
          glfwSwapBuffers(window);
          usleep(5000000);
          return 1;
        }
        glfwPollEvents();
        processPlayerMovement(player, screenBuffer, window, labirint);
        player.DrawRight(screenBuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
        //glRasterPos2f(-1, 1);
        //glPixelZoom(1, -1);
        glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
        glfwSwapBuffers(window);
      }
    glfwPollEvents();
    processPlayerMovement(player, screenBuffer, window, labirint);
    player.DrawRight(screenBuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
    //glRasterPos2f(-1, 1);
    //glPixelZoom(1, -1);
    glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
    glfwSwapBuffers(window);
  }
  return 0;
}

int nextLevelWindow(Image &screenBuffer, GLFWwindow *window, Player player) {
  Image nextLevelTheme("./resources/next_level_theme.png");
  while (!glfwWindowShouldClose(window))
  {
    for (int y = 0; y < 32 * 20 ; y++) {
      for (int x = 0; x < 32 * 80; x++) {
        screenBuffer.PutPixel(x, y, backgroundColor);
      }
    }
    for (int x = 0; x < 512; x++) {
      for (int y = 0; y < 205; y++) {
        screenBuffer.PutPixel(x, y, nextLevelTheme.GetPixel(x, y));
      }
    }
    glfwPollEvents();
    //processPlayerMovement(player, screenBuffer, window, labirint);
    //player.Draw(screenBuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
    glRasterPos2f(-1, 1);
    glPixelZoom(1, -1);
    glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
    glfwSwapBuffers(window);
    usleep(microsecond * 10);
    if (level2(screenBuffer, window, player) == 1) {
      return 1;
    }
  }
  return 0;
}

void loadTorch(GLFWwindow *window, Image &screenBuffer, int i, int j, Player &player) {
  Image torch1("./resources/torch1.png");
  Image torch2("./resources/torch2.png");
  Image torch3("./resources/torch3.png");
  for (int y = 0; y <= 48; y++) {
    for (int x = 0; x <= 16; x++) {
      Pixel current_pixel = torch1.GetPixel(x, y);
      if (current_pixel.a > 0.1) {
        screenBuffer.PutPixel(j * 16 + x, i * 16 + y, torch1.GetPixel(x, y));
      }
    }
  }
  for (int y = 0; y <= 48; y++) {
    for (int x = 0; x <= 16; x++) {
      Pixel current_pixel = screenBuffer.GetPixel((j + 1) * 16 + x, i * 16 + y);
      current_pixel.r += 15;
      current_pixel.g += 15;
      current_pixel.b += 15;
      screenBuffer.PutPixel((j + 1) * 16 + x, i * 16 + y, current_pixel);
    }
  }


  for (int y = 0; y <= 48; y++) {
    for (int x = 0; x <= 16; x++) {
      Pixel current_pixel = screenBuffer.GetPixel((j - 1) * 16 + x, i * 16 + y);
      current_pixel.r += 15;
      current_pixel.g += 15;
      current_pixel.b += 15;
      screenBuffer.PutPixel((j - 1) * 16 + x, i * 16 + y, current_pixel);
    }
  }

  for (int y = 0; y <= 48; y++) {
    for (int x = 0; x <= 16; x++) {
      Pixel current_pixel = screenBuffer.GetPixel(j * 16 + x, (i + 1) * 16 + y);
      current_pixel.r += 15;
      current_pixel.g += 15;
      current_pixel.b += 15;
      screenBuffer.PutPixel(j * 16 + x, (i + 1) * 16 + y, current_pixel);
    }
  }

  for (int y = 0; y <= 48; y++) {
    for (int x = 0; x <= 16; x++) {
      Pixel current_pixel = screenBuffer.GetPixel(j * 16 + x, (i - 1) * 16 + y);
      current_pixel.r += 15;
      current_pixel.g += 15;
      current_pixel.b += 15;
      screenBuffer.PutPixel(j * 16 + x, (i - 1) * 16 + y, current_pixel);
    }
  }

  for (int y = 0; y <= 48; y++) {
    for (int x = 0; x <= 16; x++) {
      Pixel current_pixel = screenBuffer.GetPixel((j - 1) * 16 + x, (i - 1) * 16 + y);
      current_pixel.r += 15;
      current_pixel.g += 15;
      current_pixel.b += 15;
      screenBuffer.PutPixel((j - 1) * 16 + x, (i - 1) * 16 + y, current_pixel);
    }
  }

  for (int y = 0; y <= 48; y++) {
    for (int x = 0; x <= 16; x++) {
      Pixel current_pixel = screenBuffer.GetPixel((j + 1) * 16 + x, (i + 1) * 16 + y);
      current_pixel.r += 15;
      current_pixel.g += 15;
      current_pixel.b += 15;
      screenBuffer.PutPixel((j + 1) * 16 + x, (i + 1) * 16 + y, current_pixel);
    }
  }

  for (int y = 0; y <= 48; y++) {
    for (int x = 0; x <= 16; x++) {
      Pixel current_pixel = screenBuffer.GetPixel((j - 1) * 16 + x, (i + 1) * 16 + y);
      current_pixel.r += 15;
      current_pixel.g += 15;
      current_pixel.b += 15;
      screenBuffer.PutPixel((j - 1) * 16 + x, (i + 1) * 16 + y, current_pixel);
    }
  }

  for (int y = 0; y <= 48; y++) {
    for (int x = 0; x <= 16; x++) {
      Pixel current_pixel = screenBuffer.GetPixel((j + 1) * 16 + x, (i - 1) * 16 + y);
      current_pixel.r += 15;
      current_pixel.g += 15;
      current_pixel.b += 15;
      screenBuffer.PutPixel((j + 1) * 16 + x, (i - 1) * 16 + y, current_pixel);
    }
  }

  for (int y = 0; y <= 48; y++) {
    for (int x = 0; x <= 16; x++) {
      Pixel current_pixel = screenBuffer.GetPixel((j + 2) * 16 + x, i * 16 + y);
      current_pixel.r += 5;
      current_pixel.g += 5;
      current_pixel.b += 5;
      screenBuffer.PutPixel((j + 2) * 16 + x, i * 16 + y, current_pixel);
    }
  }

  for (int y = 0; y <= 48; y++) {
    for (int x = 0; x <= 16; x++) {
      Pixel current_pixel = screenBuffer.GetPixel((j + 2) * 16 + x, (i + 1) * 16 + y);
      current_pixel.r += 5;
      current_pixel.g += 5;
      current_pixel.b += 5;
      screenBuffer.PutPixel((j + 2) * 16 + x, (i + 1) * 16 + y, current_pixel);
    }
  }
  
  for (int y = 0; y <= 48; y++) {
    for (int x = 0; x <= 16; x++) {
      Pixel current_pixel = screenBuffer.GetPixel((j + 2) * 16 + x, (i + 2) * 16 + y);
      current_pixel.r += 5;
      current_pixel.g += 5;
      current_pixel.b += 5;
      screenBuffer.PutPixel((j + 2) * 16 + x, (i + 2) * 16 + y, current_pixel);
    }
  }

  for (int y = 0; y <= 48; y++) {
    for (int x = 0; x <= 16; x++) {
      Pixel current_pixel = screenBuffer.GetPixel((j + 2) * 16 + x, (i - 1) * 16 + y);
      current_pixel.r += 5;
      current_pixel.g += 5;
      current_pixel.b += 5;
      screenBuffer.PutPixel((j + 2) * 16 + x, (i - 1) * 16 + y, current_pixel);
    }
  }

  for (int y = 0; y <= 48; y++) {
    for (int x = 0; x <= 16; x++) {
      Pixel current_pixel = screenBuffer.GetPixel((j + 2) * 16 + x, (i - 2) * 16 + y);
      current_pixel.r += 5;
      current_pixel.g += 5;
      current_pixel.b += 5;
      screenBuffer.PutPixel((j + 2) * 16 + x, (i - 2) * 16 + y, current_pixel);
    }
  }


  for (int y = 0; y <= 48; y++) {
    for (int x = 0; x <= 16; x++) {
      Pixel current_pixel = screenBuffer.GetPixel((j + 1) * 16 + x, (i - 2) * 16 + y);
      current_pixel.r += 5;
      current_pixel.g += 5;
      current_pixel.b += 5;
      screenBuffer.PutPixel((j + 1) * 16 + x, (i - 2) * 16 + y, current_pixel);
    }
  }

  for (int y = 0; y <= 48; y++) {
    for (int x = 0; x <= 16; x++) {
      Pixel current_pixel = screenBuffer.GetPixel((j) * 16 + x, (i - 2) * 16 + y);
      current_pixel.r += 5;
      current_pixel.g += 5;
      current_pixel.b += 5;
      screenBuffer.PutPixel((j) * 16 + x, (i - 2) * 16 + y, current_pixel);
    }
  }

  for (int y = 0; y <= 48; y++) {
    for (int x = 0; x <= 16; x++) {
      Pixel current_pixel = screenBuffer.GetPixel((j - 1) * 16 + x, (i - 2) * 16 + y);
      current_pixel.r += 5;
      current_pixel.g += 5;
      current_pixel.b += 5;
      screenBuffer.PutPixel((j - 1) * 16 + x, (i - 2) * 16 + y, current_pixel);
    }
  }

  for (int y = 0; y <= 48; y++) {
    for (int x = 0; x <= 16; x++) {
      Pixel current_pixel = screenBuffer.GetPixel((j - 2) * 16 + x, (i - 2) * 16 + y);
      current_pixel.r += 5;
      current_pixel.g += 5;
      current_pixel.b += 5;
      screenBuffer.PutPixel((j - 2) * 16 + x, (i - 2) * 16 + y, current_pixel);
    }
  }

  for (int y = 0; y <= 48; y++) {
    for (int x = 0; x <= 16; x++) {
      Pixel current_pixel = screenBuffer.GetPixel((j - 2) * 16 + x, (i - 1) * 16 + y);
      current_pixel.r += 5;
      current_pixel.g += 5;
      current_pixel.b += 5;
      screenBuffer.PutPixel((j - 2) * 16 + x, (i - 1) * 16 + y, current_pixel);
    }
  }

  for (int y = 0; y <= 48; y++) {
    for (int x = 0; x <= 16; x++) {
      Pixel current_pixel = screenBuffer.GetPixel((j - 2) * 16 + x, (i) * 16 + y);
      current_pixel.r += 5;
      current_pixel.g += 5;
      current_pixel.b += 5;
      screenBuffer.PutPixel((j - 2) * 16 + x, (i) * 16 + y, current_pixel);
    }
  }

  for (int y = 0; y <= 48; y++) {
    for (int x = 0; x <= 16; x++) {
      Pixel current_pixel = screenBuffer.GetPixel((j - 2) * 16 + x, (i + 1) * 16 + y);
      current_pixel.r += 5;
      current_pixel.g += 5;
      current_pixel.b += 5;
      screenBuffer.PutPixel((j - 2) * 16 + x, (i + 1) * 16 + y, current_pixel);
    }
  }

  for (int y = 0; y <= 48; y++) {
    for (int x = 0; x <= 16; x++) {
      Pixel current_pixel = screenBuffer.GetPixel((j - 2) * 16 + x, (i + 2) * 16 + y);
      current_pixel.r += 5;
      current_pixel.g += 5;
      current_pixel.b += 5;
      screenBuffer.PutPixel((j - 2) * 16 + x, (i + 2) * 16 + y, current_pixel);
    }
  }

  for (int y = 0; y <= 48; y++) {
    for (int x = 0; x <= 16; x++) {
      Pixel current_pixel = screenBuffer.GetPixel((j - 1) * 16 + x, (i + 2) * 16 + y);
      current_pixel.r += 5;
      current_pixel.g += 5;
      current_pixel.b += 5;
      screenBuffer.PutPixel((j - 1) * 16 + x, (i + 2) * 16 + y, current_pixel);
    }
  }

  for (int y = 0; y <= 48; y++) {
    for (int x = 0; x <= 16; x++) {
      Pixel current_pixel = screenBuffer.GetPixel((j) * 16 + x, (i + 2) * 16 + y);
      current_pixel.r += 5;
      current_pixel.g += 5;
      current_pixel.b += 5;
      screenBuffer.PutPixel((j) * 16 + x, (i + 2) * 16 + y, current_pixel);
    }
  }

  for (int y = 0; y <= 48; y++) {
    for (int x = 0; x <= 16; x++) {
      Pixel current_pixel = screenBuffer.GetPixel((j + 1) * 16 + x, (i + 2) * 16 + y);
      current_pixel.r += 5;
      current_pixel.g += 5;
      current_pixel.b += 5;
      screenBuffer.PutPixel((j + 1) * 16 + x, (i + 2) * 16 + y, current_pixel);
    }
  }
  // kek


  //usleep(microsecond * 4);
  //glfwPollEvents();
  //player.Draw(screenBuffer);
  //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
  //glRasterPos2f(-1, 1);
  //glPixelZoom(1, -1);
  //glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
  glfwSwapBuffers(window);
  if (deltaTime > 0.027) {
    for (int y = 0; y <= 48; y++) {
      for (int x = 0; x <= 16; x++) {
        Pixel current_pixel = torch2.GetPixel(x, y);
        if (current_pixel.a > 0.1) {
            screenBuffer.PutPixel(j * 16 + x, i * 16 + y, current_pixel);
        }
      }
    }
    //usleep(microsecond * 4);
    //glfwPollEvents();
    //player.Draw(screenBuffer);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
    //glRasterPos2f(-1, 1);
    //glPixelZoom(1, -1);
    //glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
    glfwSwapBuffers(window);
  }
  if (deltaTime > 0.031) {
    for (int y = 0; y <= 48; y++) {
      for (int x = 0; x <= 16; x++) {
        Pixel current_pixel = torch3.GetPixel(x, y);
        if (current_pixel.a > 0.1) {
            screenBuffer.PutPixel(j * 16 + x, i * 16 + y, current_pixel);
        }
      }
    }
    //usleep(microsecond * 4);
    //glfwPollEvents();
    //player.Draw(screenBuffer);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
    //glRasterPos2f(-1, 1);
    //glPixelZoom(1, -1);
    //glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
    glfwSwapBuffers(window);
  }
}

void AttackRight(Image &screenBuffer, GLFWwindow *window, Player &player) {
  Image magic1("./resources/Attack1.png");
  Image magic2("./resources/Attack2.png");
  Image magic3("./resources/Attack3.png");
  Image magic4("./resources/Attack4.png");
  int i = player.coords.y / 32;
  int j = player.coords.x / 32;
  usleep(100000);
  for (int y = 0; y <= 32; y++) {
    for (int x = 0; x <= 32; x++) {
      Pixel current_pixel = magic1.GetPixel(x, y);
      if (current_pixel.a > 0.1) {
        screenBuffer.PutPixel(j * 32 + x, i * 32 + y, magic1.GetPixel(x, y));
      }
    }
  }
  usleep(100000);
  glfwPollEvents();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
  glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
  glfwSwapBuffers(window);

  for (int y = 0; y <= 32; y++) {
    for (int x = 0; x <= 32; x++) {
      Pixel current_pixel = magic2.GetPixel(x, y);
      if (current_pixel.a > 0.1) {
        screenBuffer.PutPixel(j * 32 + x, i * 32 + y, magic2.GetPixel(x, y));
      }
    }
  }
  usleep(100000);
  glfwPollEvents();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
  glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
  glfwSwapBuffers(window);

  for (int y = 0; y <= 32; y++) {
    for (int x = 0; x <= 32; x++) {
      Pixel current_pixel = magic3.GetPixel(x, y);
      if (current_pixel.a > 0.1) {
        screenBuffer.PutPixel(j * 32 + x, i * 32 + y, magic3.GetPixel(x, y));
      }
    }
  }
  usleep(100000);
  glfwPollEvents();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
  glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
  glfwSwapBuffers(window);

  for (int y = 0; y <= 32; y++) {
    for (int x = 0; x <= 32; x++) {
      Pixel current_pixel = magic4.GetPixel(x, y);
      if (current_pixel.a > 0.1) {
        screenBuffer.PutPixel(j * 32 + x, i * 32 + y, magic4.GetPixel(x, y));
      }
    }
  }
  usleep(100000);
  glfwPollEvents();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
  glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
  glfwSwapBuffers(window);

}

void AttackLeft(Image &screenBuffer, GLFWwindow *window, Player &player) {
  Image magic1("./resources/Attack1.png");
  Image magic2("./resources/Attack2.png");
  Image magic3("./resources/Attack3.png");
  Image magic4("./resources/Attack4.png");
  int i = player.coords.y / 32;
  int j = player.coords.x / 32;
  usleep(100000);
  for (int y = 0; y <= 32; y++) {
    for (int x = 0; x <= 32; x++) {
      Pixel current_pixel = magic1.GetPixel(32 - x, y);
      if (current_pixel.a > 0.1) {
        screenBuffer.PutPixel(j * 32 + x, i * 32 + y, magic1.GetPixel(32 - x, y));
      }
    }
  }
  usleep(100000);
  glfwPollEvents();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
  glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
  glfwSwapBuffers(window);

  for (int y = 0; y <= 32; y++) {
    for (int x = 0; x <= 32; x++) {
      Pixel current_pixel = magic2.GetPixel(32 - x, y);
      if (current_pixel.a > 0.1) {
        screenBuffer.PutPixel(j * 32 + x, i * 32 + y, magic2.GetPixel(32 - x, y));
      }
    }
  }
  usleep(100000);
  glfwPollEvents();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
  glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
  glfwSwapBuffers(window);

  for (int y = 0; y <= 32; y++) {
    for (int x = 0; x <= 32; x++) {
      Pixel current_pixel = magic3.GetPixel(32 - x, y);
      if (current_pixel.a > 0.1) {
        screenBuffer.PutPixel(j * 32 + x, i * 32 + y, magic3.GetPixel(32 - x, y));
      }
    }
  }
  usleep(100000);
  glfwPollEvents();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
  glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
  glfwSwapBuffers(window);

  for (int y = 0; y <= 32; y++) {
    for (int x = 0; x <= 32; x++) {
      Pixel current_pixel = magic4.GetPixel(32 - x, y);
      if (current_pixel.a > 0.1) {
        screenBuffer.PutPixel(j * 32 + x, i * 32 + y, magic4.GetPixel(32 - x, y));
      }
    }
  }
  usleep(100000);
  glfwPollEvents();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
  glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
  glfwSwapBuffers(window);

}

void shootingRight(Image &screenBuffer, GLFWwindow *window, Player &player) {
  Image magic1("./resources/Magic1.png");
  Image magic2("./resources/Magic2.png");
  Image magic3("./resources/Magic3.png");
  Image magic4("./resources/Magic4.png");
  Image magic5("./resources/Magic5.png");
  int i = player.coords.y / 32;
  int j = player.coords.x / 32;
  usleep(100000);
  for (int y = 0; y <= 32; y++) {
    for (int x = 0; x <= 32; x++) {
      Pixel current_pixel = magic1.GetPixel(x, y);
      if (current_pixel.a > 0.1) {
        screenBuffer.PutPixel(j * 32 + x, i * 32 + y, magic1.GetPixel(x, y));
      }
    }
  }
  usleep(100000);
  glfwPollEvents();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
  glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
  glfwSwapBuffers(window);

  for (int y = 0; y <= 32; y++) {
    for (int x = 0; x <= 32; x++) {
      Pixel current_pixel = magic2.GetPixel(x, y);
      if (current_pixel.a > 0.1) {
        screenBuffer.PutPixel(j * 32 + x, i * 32 + y, magic2.GetPixel(x, y));
      }
    }
  }
  usleep(100000);
  glfwPollEvents();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
  glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
  glfwSwapBuffers(window);

  for (int y = 0; y <= 32; y++) {
    for (int x = 0; x <= 32; x++) {
      Pixel current_pixel = magic3.GetPixel(x, y);
      if (current_pixel.a > 0.1) {
        screenBuffer.PutPixel(j * 32 + x, i * 32 + y, magic3.GetPixel(x, y));
      }
    }
  }
  usleep(100000);
  glfwPollEvents();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
  glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
  glfwSwapBuffers(window);

  for (int y = 0; y <= 32; y++) {
    for (int x = 0; x <= 32; x++) {
      Pixel current_pixel = magic4.GetPixel(x, y);
      if (current_pixel.a > 0.1) {
        screenBuffer.PutPixel(j * 32 + x, i * 32 + y, magic4.GetPixel(x, y));
      }
    }
  }
  usleep(100000);
  glfwPollEvents();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
  glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
  glfwSwapBuffers(window);

  for (int y = 0; y <= 32; y++) {
    for (int x = 0; x <= 32; x++) {
      Pixel current_pixel = magic5.GetPixel(x, y);
      if (current_pixel.a > 0.1) {
        screenBuffer.PutPixel(j * 32 + x, i * 32 + y, magic5.GetPixel(x, y));
      }
    }
  }
  usleep(100000);
  glfwPollEvents();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
  glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
  glfwSwapBuffers(window);

  for (int y = 0; y <= 32; y++) {
    for (int x = 0; x <= 32; x++) {
      Pixel current_pixel = magic1.GetPixel(x, y);
      if (current_pixel.a > 0.1) {
        screenBuffer.PutPixel(j * 32 + x, i * 32 + y, magic1.GetPixel(x, y));
      }
    }
  }
  usleep(100000);
  glfwPollEvents();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
  glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
  glfwSwapBuffers(window);

}

bool Moved(int x, int oldX, int y, int oldY) {
  if(x == oldX && y == oldY)
    return false;
  else
    return true;
}

void shootingLeft(Image &screenBuffer, GLFWwindow *window, Player &player, char Labirint1[20][80]) {
  Image magic1("./resources/Magic1.png");
  Image magic2("./resources/Magic2.png");
  Image magic3("./resources/Magic3.png");
  Image magic4("./resources/Magic4.png");
  Image magic5("./resources/Magic5.png");
  int i = player.coords.y / 32;
  int j = player.coords.x / 32;
  for (int y = 0; y <= 32; y++) {
    for (int x = 0; x <= 32; x++) {
      Pixel current_pixel = magic1.GetPixel(32 - x, y);
      if (current_pixel.a > 0.1) {
        screenBuffer.PutPixel(j * 32 + x, i * 32 + y, magic1.GetPixel(32 - x, y));
      }
    }
  }
  usleep(100000);
  glfwPollEvents();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
  glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
  glfwSwapBuffers(window);

  for (int y = 0; y <= 32; y++) {
    for (int x = 0; x <= 32; x++) {
      Pixel current_pixel = magic2.GetPixel(32 - x, y);
      if (current_pixel.a > 0.1) {
        screenBuffer.PutPixel(j * 32 + x, i * 32 + y, magic2.GetPixel(32 - x, y));
      }
    }
  }
  usleep(100000);
  glfwPollEvents();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
  glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
  glfwSwapBuffers(window);

  for (int y = 0; y <= 32; y++) {
    for (int x = 0; x <= 32; x++) {
      Pixel current_pixel = magic3.GetPixel(32 - x, y);
      if (current_pixel.a > 0.1) {
        screenBuffer.PutPixel(j * 32 + x, i * 32 + y, magic3.GetPixel(32 - x, y));
      }
    }
  }
  usleep(100000);
  glfwPollEvents();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
  glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
  glfwSwapBuffers(window);

  for (int y = 0; y <= 32; y++) {
    for (int x = 0; x <= 32; x++) {
      Pixel current_pixel = magic4.GetPixel(32 - x, y);
      if (current_pixel.a > 0.1) {
        screenBuffer.PutPixel(j * 32 + x, i * 32 + y, magic4.GetPixel(32 - x, y));
      }
    }
  }
  usleep(100000);
  glfwPollEvents();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
  glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
  glfwSwapBuffers(window);

  for (int y = 0; y <= 32; y++) {
    for (int x = 0; x <= 32; x++) {
      Pixel current_pixel = magic5.GetPixel(32 - x, y);
      if (current_pixel.a > 0.1) {
        screenBuffer.PutPixel(j * 32 + x, i * 32 + y, magic5.GetPixel(32 - x, y));
      }
    }
  }
  usleep(100000);
  glfwPollEvents();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
  glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
  glfwSwapBuffers(window);

  for (int y = 0; y <= 32; y++) {
    for (int x = 0; x <= 32; x++) {
      Pixel current_pixel = magic1.GetPixel(32 - x, y);
      if (current_pixel.a > 0.1) {
        screenBuffer.PutPixel(j * 32 + x, i * 32 + y, magic1.GetPixel(32 - x, y));
      }
    }
  }
  usleep(100000);
  glfwPollEvents();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
  glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
  glfwSwapBuffers(window);
  int xBall, yBall;
  xBall = player.coords.x + 32;
  yBall = player.coords.y;
  int xOldBall = player.coords.x;
  int yOldBall = player.coords.y;
  do {
    xBall -= 32;
    /*if(Moved(xBall, xOldBall, yBall, yOldBall))
    {
      for(int y = yOldBall; y >= yOldBall - 32; --y)
      {
        for(int x = xOldBall; x >= xOldBall - 32; --x)
        {
          if (backgroundColor.a > 0.1) {
            screenBuffer.PutPixel(x, y, backgroundColor);
          }
        }
      }
      xOldBall = xBall;
      yOldBall = yOldBall;
    }*/
    Image image("./resources/fireball.png");
    for(int y = yBall; y >= yBall - 32; --y)
    {
      for(int x = xBall; x >= xBall - 32; --x)
      {
        Pixel current_pixel = image.GetPixel(x, y);
        if (current_pixel.a > 0.1) {
          screenBuffer.PutPixel(x, y, current_pixel);
        }
      }
    }

    usleep(100000);
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
    glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
    glfwSwapBuffers(window);
  } while (Labirint1[xBall / 32][yBall / 32] != '#');
}

void loadSkeletonWait(GLFWwindow *window, Image &screenBuffer, int i, int j, Player &player) {
  Image skeleton1("./resources/Skeleton1.png");
  Image skeleton2("./resources/Skeleton2.png");
  Image skeleton3("./resources/Skeleton3.png");
  Image skeleton4("./resources/Skeleton4.png");
  for (int y = 0; y <= 32; y++) {
    for (int x = 0; x <= 32; x++) {
      Pixel current_pixel = skeleton1.GetPixel(32 - x, y);
      if (current_pixel.a > 0.1) {
        screenBuffer.PutPixel(j * 32 + x, i * 32 + y, skeleton1.GetPixel(32 - x, y));
      }
    }
  }
  glfwPollEvents();
  glfwSwapBuffers(window);
  if (deltaTime > 0.027) {
    for (int y = 0; y <= 32; y++) {
      for (int x = 0; x <= 32; x++) {
        Pixel current_pixel = skeleton2.GetPixel(32 - x, y);
        if (current_pixel.a > 0.1) {
          screenBuffer.PutPixel(j * 32 + x, i * 32 + y, skeleton2.GetPixel(32 - x, y));
        }
      }
    }
  }
  glfwPollEvents();
  glfwSwapBuffers(window);
  if (deltaTime > 0.031) {
    for (int y = 0; y <= 32; y++) {
      for (int x = 0; x <= 32; x++) {
        Pixel current_pixel = skeleton3.GetPixel(32 - x, y);
        if (current_pixel.a > 0.1) {
          screenBuffer.PutPixel(j * 32 + x, i * 32 + y, skeleton3.GetPixel(32 - x, y));
        }
      }
    }
  }
  glfwPollEvents();
  glfwSwapBuffers(window);
  if (deltaTime > 0.034) {
    for (int y = 0; y <= 32; y++) {
      for (int x = 0; x <= 32; x++) {
        Pixel current_pixel = skeleton4.GetPixel(32 - x, y);
        if (current_pixel.a > 0.1) {
          screenBuffer.PutPixel(j * 32 + x, i * 32 + y, skeleton4.GetPixel(32 - x, y));
        }
      }
    }
  }
  glfwPollEvents();
  glfwSwapBuffers(window);

}

void loadSkeletonWalk(GLFWwindow *window, Image &screenBuffer, int i, int j, Player &player) {
  int xSkel = j * 32;
  int ySkel = i * 32;
  Image skel1("./resources/SkeletonWalking.png");
  Image skel2("./resources/SkeletonWalking2.png");
  Image skel3("./resources/SkeletonWalking3.png");
  Image skel4("./resources/SkeletonWalking4.png");
  Image skel5("./resources/SkeletonWalking5.png");
  for (int k = 0; k < 3; k++) {
    for (int y = 0; y <= 32; y++) {
      for (int x = 0; x <= 32; x++) {
        Pixel current_pixel = skel1.GetPixel(32 - x, y);
        if (current_pixel.a > 0.1) {
          screenBuffer.PutPixel(xSkel + x, i * 32 + y, skel1.GetPixel(32 - x, y));
        }
      }
    }
    usleep(100000);
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
    glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
    glfwSwapBuffers(window);
    for (int y = 0; y <= 32; y++) {
      for (int x = 0; x <= 32; x++) {
        Pixel current_pixel = skel2.GetPixel(32 - x, y);
        if (current_pixel.a > 0.1) {
          screenBuffer.PutPixel(xSkel + x, i * 32 + y, skel2.GetPixel(32 - x, y));
        }
      }
    }
    usleep(100000);
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
    glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
    glfwSwapBuffers(window);
    for (int y = 0; y <= 32; y++) {
      for (int x = 0; x <= 32; x++) {
        Pixel current_pixel = skel3.GetPixel(32 - x, y);
        if (current_pixel.a > 0.1) {
          screenBuffer.PutPixel(xSkel + x, i * 32 + y, skel3.GetPixel(32 - x, y));
        }
      }
    }
    usleep(100000);
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
    glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
    glfwSwapBuffers(window);
    for (int y = 0; y <= 32; y++) {
      for (int x = 0; x <= 32; x++) {
        Pixel current_pixel = skel4.GetPixel(32 - x, y);
        if (current_pixel.a > 0.1) {
          screenBuffer.PutPixel(xSkel + x, i * 32 + y, skel4.GetPixel(32 - x, y));
        }
      }
    }
    usleep(100000);
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
    glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
    glfwSwapBuffers(window);
    for (int y = 0; y <= 32; y++) {
      for (int x = 0; x <= 32; x++) {
        Pixel current_pixel = skel5.GetPixel(32 - x, y);
        if (current_pixel.a > 0.1) {
          screenBuffer.PutPixel(xSkel + x, i * 32 + y, skel5.GetPixel(32 - x, y));
        }
      }
    }
    usleep(100000);
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
    glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
    glfwSwapBuffers(window);
    xSkel -= 32;
  }
}

int level1(Image &screenBuffer, GLFWwindow *window, Player player) {

  unsigned char text[1600];

  ifstream MyFile("Labirint1.txt");
  int i = 0;
  char labirint[20][80];
  while(!MyFile.eof()) {
    MyFile >> text[i];
    i++;
  }

  for (int i = 0; i < 20; i++) {
    for (int j = 0; j < 80; j++) {
      labirint[i][j] = text[i * 80 + j]; 
    }
  }

  Image enter("./resources/enter.png");
  Image wall("./resources/wall.png");
  Image floor("./resources/floor.png");
  Image doorOpen("./resources/open_door.png");
  Image doorClose("./resources/close_door.png");
  Image pent1("./resources/pent1.png");
  Image pent2("./resources/pent2.png");
  Image pent3("./resources/pent3.png");
  Image exit1("./resources/next_level.png");
  Image nextLevelTheme("./resources/next_level_theme.png");
  Image victory("./resources/victory.jpg");
  Image torch1("./resources/torch1.png");
  Image torch2("./resources/torch2.png");
  Image torch3("./resources/torch3.png");
  int xOfExit, yOfExit;

  for (int i = 0; i < 20; i++) {
    for (int j = 0; j < 80; j++) {
      if (labirint[i][j] == '@') {
        player.coords.x = j * 32;
        player.coords.y = i * 32;
        player.old_coords.x = j * 32;
        player.old_coords.y = i * 32;
        for(int y = 0; y <= 32; ++y)
        {
            for(int x = 0; x <= 32; ++x)
            {
              Pixel current_pixel = floor.GetPixel(x, 32 - y);
              if (current_pixel.r + current_pixel.g + current_pixel.b <= 300) {
                current_pixel.a = 255;
              }
              screenBuffer.PutPixel(j * 32 + x, i * 32 + y, current_pixel);
            }
        }
      }
    }
  }
  bool switcherOfDirecting = 0;
  while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
    int step = 4;
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 80; j++) {
          if (labirint[i][j] == '#') {
              for(int y = 0; y <= 32; ++y)
              {
                  for(int x = 0; x <= 32; ++x)
                  {

                    Pixel current_pixel = wall.GetPixel(x, y);
                    if (current_pixel.r + current_pixel.g + current_pixel.b <= 300) {
                      current_pixel.a = 255;
                    }
                    screenBuffer.PutPixel(j * 32 + x, i * 32 + y, current_pixel);
                  }
              }
              continue;
          }
          if (labirint[i][j] == '@') {
            //cout << deltaTime << endl;
            shimmering(screenBuffer, window, pent1, i, j);
            if (deltaTime > 0.02) {
              shimmering(screenBuffer, window, pent2, i, j);
            }
            if (deltaTime > 0.03) {
              shimmering(screenBuffer, window, pent3, i, j);
            }
            continue;
          }
          if (labirint[i][j] == '.') {
            for(int y = 0; y <= 32; ++y)
            {
              for(int x = 0; x <= 32; ++x)
              {
                Pixel current_pixel = floor.GetPixel(x, y);
                if (current_pixel.r + current_pixel.g + current_pixel.b <= 300) {
                  current_pixel.a = 255;
                }
                screenBuffer.PutPixel(j * 32 + x, i * 32 + y, current_pixel);
              }
            }
            
            continue;
          }
          if (labirint[i][j] == 'x') {
            for(int y = 0; y <= 32; ++y)
            {
                for(int x = 0; x <= 32; ++x)
                {
                  xOfExit = j;
                  yOfExit = i;
                  Pixel current_pixel = exit1.GetPixel(x, y);
                  if (current_pixel.r + current_pixel.g + current_pixel.b <= 300) {
                    current_pixel.a = 255;
                  }
                  screenBuffer.PutPixel(j * 32 + x, i * 32 + y, current_pixel);
                }
            }
            continue;
          }
          if (labirint[i][j] == 'D') {
            for(int y = 0; y <= 32; ++y)
            {
              for(int x = 0; x <= 32; ++x)
              {
                Pixel current_pixel = doorClose.GetPixel(x, y);
                if (current_pixel.r + current_pixel.g + current_pixel.b <= 300) {
                  current_pixel.a = 255;
                }
                screenBuffer.PutPixel(j * 32 + x, i * 32 + y, current_pixel);
              }
            }
          
          }
          if (labirint[i][j] == 'O') {
            for(int y = 0; y <= 32; ++y)
            {
              for(int x = 0; x <= 32; ++x)
              {
                Pixel current_pixel = doorOpen.GetPixel(x, y);
                if (current_pixel.r + current_pixel.g + current_pixel.b <= 300) {
                  current_pixel.a = 255;
                }
                screenBuffer.PutPixel(j * 32 + x, i * 32 + y, current_pixel);
              }
            }
          
          }
        }
    }
    if (((int)(float(player.coords.x) / 32)) == xOfExit && ((int)(float(player.coords.y) / 32)) == yOfExit) {
      if (nextLevelWindow(screenBuffer, window, player) == 1) {
        return 1;
      }
    }
    glfwPollEvents();

    loadTorch(window, screenBuffer, 33, 79, player);
    loadTorch(window, screenBuffer, 34, 80, player);
    loadTorch(window, screenBuffer, 33, 78, player);
    loadTorch(window, screenBuffer, 33, 85, player);
    loadTorch(window, screenBuffer, 34, 77, player);
    loadTorch(window, screenBuffer, 35, 85, player);
    loadTorch(window, screenBuffer, 15, 77, player);
    loadTorch(window, screenBuffer, 15, 83, player);
    loadTorch(window, screenBuffer, 17, 53, player);
    loadTorch(window, screenBuffer, 23, 20, player);
    loadTorch(window, screenBuffer, 23, 25, player);
    loadTorch(window, screenBuffer, 19, 25, player);
    loadTorch(window, screenBuffer, 15, 144, player);
    loadTorch(window, screenBuffer, 15, 140, player);

    loadSkeletonWait(window, screenBuffer, 15, 70, player);
    //loadSkeletonWalk(window, screenBuffer, 33, 79, player);
    if (deltaTime < 0.23) {
      processPlayerMovement(player, screenBuffer, window, labirint);
    }
    if (Input.keys[GLFW_KEY_W] || Input.keys[GLFW_KEY_UP] || Input.keys[GLFW_KEY_D] || Input.keys[GLFW_KEY_RIGHT]) {
      player.DrawRight(screenBuffer);
      switcherOfDirecting = 0;
    }
    if (Input.keys[GLFW_KEY_LEFT] || Input.keys[GLFW_KEY_A] || Input.keys[GLFW_KEY_S] || Input.keys[GLFW_KEY_DOWN]) {
      player.DrawLeft(screenBuffer);
      switcherOfDirecting = 1;
    }
    if (switcherOfDirecting == 0) {
      int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
      if (state == GLFW_PRESS)
      {
        shootingRight(screenBuffer, window, player);
      }
      state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
      if (state == GLFW_PRESS)
      {
        AttackRight(screenBuffer, window, player);
      }
      player.DrawRight(screenBuffer);
    } else {
      int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
      if (state == GLFW_PRESS)
      {
        shootingLeft(screenBuffer, window, player, labirint);

      }
      state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
      if (state == GLFW_PRESS)
      {
        AttackLeft(screenBuffer, window, player);
      }
      player.DrawLeft(screenBuffer);
    }
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
    //glRasterPos2f(-1, 1);
    //glPixelZoom(1, -1);
    glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
		glfwSwapBuffers(window);
	}
}

int main(int argc, char** argv)
{ 
	if(!glfwInit())
    return -1;

//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  Image screenBuffer(WINDOW_WIDTH, WINDOW_HEIGHT, 4);

  GLFWwindow*  window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "task1 base project", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window); 

	glfwSetKeyCallback        (window, OnKeyboardPressed);  
	glfwSetCursorPosCallback  (window, OnMouseMove); 
  glfwSetMouseButtonCallback(window, OnMouseButtonClicked);
	glfwSetScrollCallback     (window, OnMouseScroll);

	if(initGL() != 0) 
		return -1;
	
  //Reset any OpenGL errors which could be present for some reason
	GLenum gl_error = glGetError();
	while (gl_error != GL_NO_ERROR)
		gl_error = glGetError();

	Point starting_pos{.x = WINDOW_WIDTH / 2, .y = WINDOW_HEIGHT / 2};
	Player player{starting_pos};

  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);  GL_CHECK_ERRORS;
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f); GL_CHECK_ERRORS;
  //game loop
	level1(screenBuffer, window, player);

	glfwTerminate();
	return 0;
}
