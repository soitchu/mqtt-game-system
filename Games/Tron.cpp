#include "../Game.cpp"
#include "../Utils/CircularArray.cpp"
#include <iostream>
#include <algorithm>

enum Direction
{
  UP,
  DOWN,
  LEFT,
  RIGHT,
};

class Tron : public Game
{
public:
  int score = 0;
  std::vector<Position> playerPositions[2] = {{}, {}};
  Direction lastDirections[2] = {Direction::LEFT, Direction::UP};
  Direction directions[2] = {Direction::LEFT, Direction::UP};

  Tron(int width, int height) : Game(width, height)
  {
    playerPositions[0].push_back({width / 4, height / 2});
    playerPositions[1].push_back({3 * width / 4, height / 2});
  }

  void updatePosition(int &snakePositionX, int &snakePositionY, Direction direction)
  {
    // Move a position in the direction of the snake
    if (direction == Direction::UP)
    {
      snakePositionY--;
    }
    else if (direction == Direction::DOWN)
    {
      snakePositionY++;
    }
    else if (direction == Direction::LEFT)
    {
      snakePositionX--;
    }
    else if (direction == Direction::RIGHT)
    {
      snakePositionX++;
    }

    // Wrap around the screen
    if (snakePositionX < 0)
    {
      snakePositionX = display.getWidth() - 1;
    }
    else
    {
      snakePositionX = snakePositionX % display.getWidth();
    }

    if (snakePositionY < 0)
    {
      snakePositionY = display.getHeight() - 1;
    }
    else
    {
      snakePositionY = snakePositionY % display.getHeight();
    }
  }

  void tick()
  {
    display.clear(Color::BLACK);

    if (isGameOver)
    {
      drawText({display.getWidth() / 2 - 20, display.getHeight() / 2}, "GAME OVER");
      return;
    }

    for (int i = 0; i < 2; i++)
    {
      auto &playerPosition = playerPositions[i];
      Position snakePosition = playerPosition.back();
      Direction direction = this->directions[i];
      int snakePositionX = snakePosition.x;
      int snakePositionY = snakePosition.y;

      // Update the position of the snake
      updatePosition(snakePositionX, snakePositionY, direction);

      // Update the last direction
      lastDirections[i] = direction;

      // Add the new position to the snake position history
      playerPosition.push_back({snakePositionX, snakePositionY});

      Color color = i == 0 ? Color::RED : Color::BLUE;

      for (int i = 0; i < playerPosition.size(); i++)
      {
        Position snakePart = playerPosition[i];
        display.setPixel(snakePart, color);
      }
    }

    bool collided[2] = {false, false};

    // FIXME: This is a draw
    if (playerPositions[0].back() == playerPositions[1].back())
    {
      isGameOver = true;
    }

    for (int i = 0; i < 2; i++)
    {
      auto &playerPosition = playerPositions[i];
      
      // Check if the snake has collided with itself
      for(int j = 0; j < playerPosition.size() - 1; j++)
      {
        if(playerPosition[j] == playerPosition.back())
        {
          collided[i] = true;
          break;
        }
      }

      if(collided[i]) break;

      for(int j = 0; j < 2; j++) {
        if(i == j) continue;

        auto &otherPlayerPosition = playerPositions[j];

        for(int k = 0; k < otherPlayerPosition.size(); k++)
        {
          if(otherPlayerPosition[k] == playerPosition.back())
          {
            std::cout << i << "collided with " << j << std::endl;
            collided[i] = true;
            break;
          }
        }
      }
    }

    // FIXME: Can possibly be a draw
    if (collided[0] || collided[1])
    {
      isGameOver = true;
    }
  }

  void buttonPressed(Player player, Button button)
  {
    // We just have one player in this game, so we ignore the player parameter
    unordered_map<Direction, Direction> directionClockWiseMap = {
        {Direction::UP, Direction::RIGHT},
        {Direction::DOWN, Direction::LEFT},
        {Direction::LEFT, Direction::UP},
        {Direction::RIGHT, Direction::DOWN},
    };

    unordered_map<Direction, Direction> directionCounterClockWiseMap = {
        {Direction::UP, Direction::LEFT},
        {Direction::DOWN, Direction::RIGHT},
        {Direction::LEFT, Direction::DOWN},
        {Direction::RIGHT, Direction::UP},
    };

    unordered_map<Direction, Direction> directionOppositeMap = {
        {Direction::UP, Direction::DOWN},
        {Direction::DOWN, Direction::UP},
        {Direction::LEFT, Direction::RIGHT},
        {Direction::RIGHT, Direction::LEFT},
    };

    if (button == ACTION_1)
    {

      Direction direction = directions[player];
      Direction oppositeDirection = directionOppositeMap[direction];
      Direction newDirection = directionCounterClockWiseMap[direction];
      Direction lastDirection = lastDirections[player];

      if (oppositeDirection == lastDirection)
      {
        return;
      }

      directions[player] = newDirection;
    }
    else if (button == ACTION_2)
    {
      Direction direction = directions[player];
      Direction oppositeDirection = directionOppositeMap[direction];
      Direction newDirection = directionClockWiseMap[direction];
      Direction lastDirection = lastDirections[player];

      if (oppositeDirection == lastDirection)
      {
        return;
      }

      directions[player] = newDirection;
    }
  }

};