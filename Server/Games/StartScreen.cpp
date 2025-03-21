#include "../Game.cpp"

class StartScreen : public Game
{
public:
  bool ready[2] = {false, false};
  int tickNumber = 0;

  StartScreen(int width, int height) : Game(width, height)
  {
    init();
  }

  void init() override
  {
    isGameOver = false;
    ready[0] = false;
    ready[1] = false;
    tickNumber = 0;
  }

  void buttonPressed(Player player, Button button) override
  {
    if (player == Player::ONE)
    {
      ready[0] = 1;
    }
    else
    {
      ready[1] = 1;
    }
  }

  void tick() override
  {
    display.clear(Color::BLACK);

    if (ready[0] && ready[1])
    {
      isGameOver = true;
    }
    else
    {
      drawTextCentered("PRESS ANY", {0, -6});
      drawTextCentered("BUTTON TO", {0, 0});
      drawTextCentered("JOIN", {0, 6});

      int waitingIndicatorSize = 5;
      Color indicatorColor = tickNumber % 30 < 15 ? Color::RED : Color::BLACK;

      display.drawSquare(
        {display.getWidth() / 2 - waitingIndicatorSize / 2, display.getHeight() / 2 - waitingIndicatorSize - 12},
        waitingIndicatorSize,
        ready[0] ? Color::GREEN : indicatorColor
      );

      display.drawSquare(
        {display.getWidth() / 2 - waitingIndicatorSize / 2, display.getHeight() / 2 + 12},
        waitingIndicatorSize,
        ready[1] ? Color::GREEN : indicatorColor
      );

      tickNumber = (tickNumber + 1) % 0xFFFFFFFF;
    }
  }
};