#include <iostream>

enum Color
{
  BLACK,
  RED,
  GREEN,
  CYAN,
  BLUE,
  MAGENTA,
  YELLOW,
  WHITE,
};

struct Position
{
  int x;
  int y;

  bool operator==(const Position &other) const
  {
    return x == other.x && y == other.y;
  }
};

class Display
{
private:
  int width;
  int height;

public:
  Color *pixels;

  Display(int width, int height)
  {
    this->width = width;
    this->height = height;
    this->pixels = new Color[width * height];

    for (int i = 0; i < width * height; i++)
    {
      pixels[i] = Color::BLACK;
    }
  }

  /**
   * Sets the pixel at the given coordinates to the given color.
   */
  void setPixel(Position pos, Color color)
  {
    int x = pos.x;
    int y = pos.y;

    if (x < 0 || x >= width || y < 0 || y >= height)
    {
      return;
    }

    pixels[y * width + x] = color;
  }

  /**
   * Gets the color of the pixel at the given coordinates.
   */
  Color getPixel(Position pos)
  {
    int x = pos.x;
    int y = pos.y;

    if (x < 0 || x >= width || y < 0 || y >= height)
    {
      return Color::BLACK;
    }

    return pixels[y * width + x];
  }

  /**
   * Draws a square at the given coordinates with the given size and color.
   */
  void drawSquare(Position pos, int size, Color color)
  {
    int x = pos.x;
    int y = pos.y;

    for (int i = x; i < x + size && i < Display::width; i++)
    {
      for (int j = y; j < y + size && i < Display::height; j++)
      {

        setPixel({i, j}, color);
      }
    }
  }

  /**
   * Clears the display with the given color.
   */
  void clear(Color color)
  {
    for (int i = 0; i < width * height; i++)
    {
      pixels[i] = color;
    }
  }

  /**
   * Gets the width of the display.
   */
  int getWidth()
  {
    return width;
  }

  /**
   * Gets the height of the display.
   */
  int getHeight()
  {
    return height;
  }
};