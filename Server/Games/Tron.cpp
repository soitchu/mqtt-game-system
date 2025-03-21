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

enum Result
{
  DRAW,
  PLAYER_1,
  PLAYER_2,
};

class Tron : public Game
{
public:
  Result result = Result::DRAW;
  std::vector<Position> playerPositions[2] = {{}, {}};
  Direction lastDirections[2] = {Direction::RIGHT, Direction::LEFT};
  Direction directions[2] = {Direction::RIGHT, Direction::LEFT};

  Tron(int width, int height) : Game(width, height)
  {
    init();
  }

  void init() override
  {
    isGameOver = false;
    result = Result::DRAW;
    playerPositions[0].clear();
    playerPositions[1].clear();
    playerPositions[0].push_back({display.getWidth() / 4, display.getHeight() / 2});
    playerPositions[1].push_back({3 * display.getWidth() / 4, display.getHeight() / 2});
    directions[0] = Direction::RIGHT;
    directions[1] = Direction::LEFT;
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
  }

  void tick() override
  {
    display.clear(Color::BLACK);

    if (isGameOver)
    {
      if (result == Result::PLAYER_1)
      {
        drawTextCentered("PLAYER 1 WINS");
      }
      else if (result == Result::PLAYER_2)
      {
        drawTextCentered("PLAYER 2 WINS");
      }
      else
      {
        drawTextCentered("DRAW");
      }
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
      collided[0] = true;
      collided[1] = true;
    }
    else
    {
      for (int i = 0; i < 2; i++)
      {
        auto &playerPosition = playerPositions[i];

        if (
            playerPosition.back().x < 0 ||
            playerPosition.back().x >= display.getWidth() ||
            playerPosition.back().y < 0 ||
            playerPosition.back().y >= display.getHeight())
        {
          collided[i] = true;
          continue;
        }

        // Check if the snake has collided with itself
        for (int j = 0; j < playerPosition.size() - 1; j++)
        {
          if (playerPosition[j] == playerPosition.back())
          {
            collided[i] = true;
            break;
          }
        }

        if (collided[i])
          break;

        for (int j = 0; j < 2; j++)
        {
          if (i == j)
            continue;

          auto &otherPlayerPosition = playerPositions[j];

          for (int k = 0; k < otherPlayerPosition.size(); k++)
          {
            if (otherPlayerPosition[k] == playerPosition.back())
            {
              collided[i] = true;
              break;
            }
          }
        }
      }
    }

    // FIXME: Can possibly be a draw
    if (collided[0] || collided[1])
    {
      if (collided[0] && collided[1])
      {
        result = Result::DRAW;
      }
      else if (collided[0])
      {
        result = Result::PLAYER_2;
      }
      else
      {
        result = Result::PLAYER_1;
      }
      isGameOver = true;
    }
  }

  void buttonPressed(Player player, Button button) override
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