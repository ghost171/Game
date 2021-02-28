#include "common.h"
#include "Image.h"
#include "Player.h"
#include <iostream>
#include <fstream>
#include <unistd.h>  

using namespace std;

#define GLFW_DLL
#include <GLFW/glfw3.h>

char BUFFER_TILE = '.';

float threshold = 0.02;

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
unsigned int microsecond = 100000;


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

void moving(Player &player, Image &screenBuffer, GLFWwindow *window, Image &imageForMoving) {

  for(int y = player.coords.y; y <= player.coords.y + 32; ++y)
  {
    for(int x = player.coords.x; x <= player.coords.x + 32; ++x)
    {
      screenBuffer.PutPixel(x, y, imageForMoving.GetPixel((x - player.coords.x), (y - player.coords.y)));
    }
  }
  //usleep(microsecond);
  glfwPollEvents();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
  glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
  glfwSwapBuffers(window);
}

void processPlayerMovement(Player &player, Image &screenBuffer, GLFWwindow* window, char labirint[20][80])
{
  cout << "DELTATIME" << endl;
  cout << deltaTime << endl;
  cout << "DELTATIME" << endl;
  if (Input.keys[GLFW_KEY_E]) {
    for (int i = 0; i < 20; i++) {
      for (int j = 0; j < 80; j++) {
        if (labirint[i][j] == '@') {
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
        }
      }
    }
  }
  if (Input.keys[GLFW_KEY_W] || Input.keys[GLFW_KEY_UP]) {
    for (int i = 0; i < 20; i++) {
      for (int j = 0; j < 80; j++) {
        if (labirint[i][j] == '@') {
          if (i != 0 && labirint[i - 1][j] != '#' && labirint[i - 1][j] != 'D') {
            if (deltaTime < threshold) {
              labirint[i][j] = BUFFER_TILE;
              BUFFER_TILE = labirint[i - 1][j];
              labirint[i - 1][j] = '@';
            }
            moving(player, screenBuffer, window, player.imageForMoving1);
            moving(player, screenBuffer, window, player.imageForMoving2);
            moving(player, screenBuffer, window, player.imageForMoving3);
            moving(player, screenBuffer, window, player.imageForMoving2);
            moving(player, screenBuffer, window, player.imageForMoving1);
            player.ProcessInput(MovementDir::DOWN);
          }
          return;
        }
      }
    } 
  }
  else if (Input.keys[GLFW_KEY_S] || Input.keys[GLFW_KEY_DOWN]) {
    for (int i = 0; i < 20; i++) {
      for (int j = 0; j < 80; j++) {
        if (labirint[i][j] == '@') {
          if (i != 19 && labirint[i + 1][j] != '#' && labirint[i + 1][j] != 'D') {
            if (deltaTime < threshold) {
              labirint[i][j] = BUFFER_TILE;
              BUFFER_TILE = labirint[i + 1][j];
              labirint[i + 1][j] = '@';
            }
            moving(player, screenBuffer, window, player.imageForMoving1);
            moving(player, screenBuffer, window, player.imageForMoving2);
            moving(player, screenBuffer, window, player.imageForMoving3);
            moving(player, screenBuffer, window, player.imageForMoving2);
            moving(player, screenBuffer, window, player.imageForMoving1);
            player.ProcessInput(MovementDir::UP);
          }
          return;
        }
      }
    }
  }
  if (Input.keys[GLFW_KEY_A] || Input.keys[GLFW_KEY_LEFT]) {
    for (int i = 0; i < 20; i++) {
      for (int j = 0; j < 80; j++) {
        if (labirint[i][j] == '@') {
          if (j != 0 && labirint[i][j - 1] != '#' && labirint[i][j - 1] != 'D') {
            if (deltaTime < threshold) {
              labirint[i][j] = BUFFER_TILE;
              BUFFER_TILE = labirint[i][j - 1];
              labirint[i][j - 1] = '@';
            }
            moving(player, screenBuffer, window, player.imageForMoving1);
            moving(player, screenBuffer, window, player.imageForMoving2);
            moving(player, screenBuffer, window, player.imageForMoving3);
            moving(player, screenBuffer, window, player.imageForMoving2);
            moving(player, screenBuffer, window, player.imageForMoving1);
            player.ProcessInput(MovementDir::LEFT);
          }
          return;
        }
      }
    }
  }
  else if (Input.keys[GLFW_KEY_D] || Input.keys[GLFW_KEY_RIGHT]) {
    for (int i = 0; i < 20; i++) {
      for (int j = 0; j < 80; j++) {
        if (labirint[i][j] == '@') {
          if (j != 79 && labirint[i][j + 1] != '#' && labirint[i][j + 1] != 'D') {
            if (deltaTime < threshold) {
              labirint[i][j] = BUFFER_TILE;
              BUFFER_TILE = labirint[i][j + 1];
              labirint[i][j + 1] = '@';
            }
            moving(player, screenBuffer, window, player.imageForMoving1);
            moving(player, screenBuffer, window, player.imageForMoving2);
            moving(player, screenBuffer, window, player.imageForMoving3);
            moving(player, screenBuffer, window, player.imageForMoving2);
            moving(player, screenBuffer, window, player.imageForMoving1);
            player.ProcessInput(MovementDir::RIGHT);
          }
          return;
        }
      }
    }
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
  std::cout << "press ESC to exit" << std::endl;

	return 0;
}

int main(int argc, char** argv)
{
	if(!glfwInit())
    return -1;

  unsigned char text[1600];

  ifstream MyFile("Labirint.txt");
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
  for (int i = 0; i < 20; i++) {
    for (int j = 0; j < 80; j++) {
      cout << labirint[i][j]; 
    }
    cout << endl;
  }
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

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

	Image screenBuffer(WINDOW_WIDTH, WINDOW_HEIGHT, 4);
  Image wall("./resources/wall.png");
  Image floor("./resources/floor.png");
  Image doorOpen("./resources/open_door.png");
  Image doorClose("./resources/close_door.png");
  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);  GL_CHECK_ERRORS;
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f); GL_CHECK_ERRORS;

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

  //game loop
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
    processPlayerMovement(player, screenBuffer, window, labirint); 
    cout << "LLLLLLLLLLLLL" << endl; 
    for (int i = 0; i < 20; i++) {
       for (int j = 0; j < 80; j++) {
         cout << labirint[i][j];
       }
       cout << endl;
    }
    cout << "LLLLLLLLLLLLL" << endl;
    glfwPollEvents();
    player.Draw(screenBuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
    glRasterPos2f(-1, 1);
    glPixelZoom(1, -1);
    glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}
