#ifndef GAME
#define GAME

#include "Display.cpp"
#include "Font.cpp"
#include <string>
#include <cmath>

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
  virtual void init() = 0;

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

  void drawTextCentered(std::string text, Position offset = {0, 0})
  {
    int x = (display.getWidth() - text.length() * 4) / 2;
    int y = (display.getHeight() - 5)/ 2;

    drawText({x + offset.x, y + offset.y}, text);
  }

  std::vector<char> getDisplayBuffer()
  {
    int totalPixels = display.getWidth() * display.getHeight();
    Color* displayPixels = display.pixels;

    int numberOfBitColors = 3;
    int colorMask = 0b11111111 >> (8 - numberOfBitColors);
    int size = ceil((totalPixels * numberOfBitColors) / 8.0);
    std::vector<char> pixels(size, 0);

    for(int i = 0; i < totalPixels; i++) {
      int byteIndex = (i * numberOfBitColors) / 8;
      int bitOffset = (i * numberOfBitColors) % 8;

      if(bitOffset <= 5) {
        pixels[byteIndex] |= (displayPixels[i] & colorMask) << bitOffset;
      } else {
        pixels[byteIndex] |= (displayPixels[i] & colorMask) << bitOffset;
        pixels[byteIndex + 1] |= (displayPixels[i] & colorMask) >> (8 - bitOffset);
      }
    }

    return pixels;
  }
};

#endif 