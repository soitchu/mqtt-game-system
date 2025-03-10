#include "../Display.cpp"
#include <cassert>
#include "gtest/gtest.h"

TEST(DisplayTest, test_display_initialization)
{
  Display display(10, 10);

  for (int i = 0; i < 10; i++)
  {
    for (int j = 0; j < 10; j++)
    {
      ASSERT_EQ(display.getPixel(i, j), Color::BLACK);
    }
  }
}

TEST(DisplayTest, test_display_set_pixel)
{
  Display display(10, 10);

  display.setPixel(5, 5, Color::RED);

  for (int i = 0; i < 10; i++)
  {
    for (int j = 0; j < 10; j++)
    {
      if (i == 5 && j == 5)
      {
        ASSERT_EQ(display.getPixel(i, j), Color::RED);
      }
      else
      {
        ASSERT_EQ(display.getPixel(i, j), Color::BLACK);
      }
    }
  }
}

TEST(DisplayTest, test_display_draw_square)
{
  Display display(10, 10);

  display.drawSquare(2, 2, 5, Color::GREEN);

  for (int i = 0; i < 10; i++)
  {
    for (int j = 0; j < 10; j++)
    {
      if (i >= 2 && i < 7 && j >= 2 && j < 7)
      {
        ASSERT_EQ(display.getPixel(i, j), Color::GREEN);
      }
      else
      {
        ASSERT_EQ(display.getPixel(i, j), Color::BLACK);
      }
    }
  }
}

TEST(DisplayTest, test_display_draw_square_out_of_bounds)
{
  Display display(10, 10);

  display.drawSquare(8, 8, 5, Color::GREEN);

  for (int i = 0; i < 10; i++)
  {
    for (int j = 0; j < 10; j++)
    {
      if (i >= 8 && i < 10 && j >= 8 && j < 10)
      {
        ASSERT_EQ(display.getPixel(i, j), Color::GREEN);
      }
      else
      {
        ASSERT_EQ(display.getPixel(i, j), Color::BLACK);
      }
    }
  }
}

TEST(DisplayTest, test_display_clear)
{
  Display display(10, 10);

  display.setPixel(5, 5, Color::RED);
  display.clear(Color::GREEN);

  for (int i = 0; i < 10; i++)
  {
    for (int j = 0; j < 10; j++)
    {
      ASSERT_EQ(display.getPixel(i, j), Color::GREEN);
    }
  }
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}