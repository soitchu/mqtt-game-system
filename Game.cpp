#include "Display.cpp"
#include "Font.cpp"
#include <string>

enum Button
{
  ACTION_1,
  ACTION_2,
  ACTION_3,
};

enum Player
{
  ONE,
  TWO,
};

class Game
{
private:
  int TICK_RATE_HZ = 100;
  std::unordered_map<char, std::vector<int>> fonts = getFonts();

public:
  Display display = Display(0, 0);
  bool isGameOver = false;

  virtual void buttonPressed(Player player, Button button) = 0;
  virtual void tick() = 0;

  Game(int height, int width)
  {
    display = Display(height, width);
  }

  void drawText(Position pos, std::string text)
  {
    int x = pos.x;
    int y = pos.y;

    for (int i = 0; i < text.length(); i++)
    {
      std::vector<int> font = fonts[text[i]];

      for (int j = 0; j < font.size(); j++)
      {
        int fontPixel = font[j];
        int fontX = j % 3;
        int fontY = j / 3;

        if (fontPixel == 1)
        {
          display.setPixel({x + fontX, y + fontY}, Color::WHITE);
        }
      }

      x += 4;
    }
  }

  char *getDisplayBuffer()
  {
    int width = display.getWidth();
    int height = display.getHeight();
    char *pixels = new char[width * height + 1];

    for (int i = 0; i < height * width; i++)
    {
      pixels[i] = 'a' + display.pixels[i];
    }

    pixels[width * height] = '\0';

    return pixels;
  }
};