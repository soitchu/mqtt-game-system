#include "../Game.cpp"
#include "../Utils/CircularArray.cpp"
#include <iostream>

enum Direction
{
  UP,
  DOWN,
  LEFT,
  RIGHT,
};

class SnakeCircularArray : public CircularArray<Position>
{
public:
  SnakeCircularArray(int cap) : CircularArray<Position>(cap) {}

  /**
   * Checks if the snake has collided with itself.
   */
  bool hasCollided(Position value)
  {
    for (int i = 0; i < buffer.size(); i++)
    {
      // i != tail ensures that we don't return true
      // for the latest position of the snake, since it will
      // be the same as the value we are checking
      if (buffer[i] == value && i != tail)
      {
        return true;
      }
    }

    return false;
  }

  void display()
  {
    for (int i = head; i < buffer.size(); i++)
    {
      int index = i % buffer.size();
      std::cout << "X : " << buffer[index].x << " Y:" << buffer[index].y << std::endl;
    }
  }
};

class Snake : public Game
{
public:
  Position foodPosition = {0, 0};
  Direction direction = Direction::RIGHT;
  int lastDirection = Direction::RIGHT;
  SnakeCircularArray snakePositions = SnakeCircularArray(0);
  int score = 0;

  Snake(int width, int height) : Game(width, height)
  {
    snakePositions.changeCapacity(score + 1, {width / 2, height / 2});
  }

  void updatePosition(int &snakePositionX, int &snakePositionY)
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
      snakePositionX = Snake::display.getWidth() - 1;
    }
    else
    {
      snakePositionX = snakePositionX % Snake::display.getWidth();
    }

    if (snakePositionY < 0)
    {
      snakePositionY = Snake::display.getHeight() - 1;
    }
    else
    {
      snakePositionY = snakePositionY % Snake::display.getHeight();
    }
  }

  void eatFood()
  {
    // Generate a new food position
    foodPosition.x = rand() % Snake::display.getWidth();
    foodPosition.y = rand() % Snake::display.getHeight();

    // Increase the score
    this->score++;

    // Increase the capacity of the snake by 1
    snakePositions.changeCapacity(score + 1, {-1, -1});
  }

  void tick()
  {
    Snake::display.clear(Color::BLACK);

    Position snakePosition = snakePositions.getTailItem();
    int snakePositionX = snakePosition.x;
    int snakePositionY = snakePosition.y;

    // Check if the snake has eaten the food
    if (snakePosition == foodPosition)
    {
      eatFood();
    }

    // Update the position of the snake
    updatePosition(snakePositionX, snakePositionY);

    // Check if the snake has collided with itself
    if (snakePositions.hasCollided({snakePositionX, snakePositionY}))
    {

      snakePositions.display();
      snakePositions.changeCapacity(1, {Snake::display.getWidth() / 2,
                                        Snake::display.getHeight() / 2});
      score = 0;
    }

    // Update the last direction
    lastDirection = direction;

    // Add the new position to the snake position history
    snakePositions.push({snakePositionX, snakePositionY});

    // Draw the snake, food, and score
    Snake::drawText({0, 0}, "SCORE: " + std::to_string(score));

    for (int i = 0; i < snakePositions.buffer.size(); i++)
    {
      Position snakePart = snakePositions.buffer[i];
      Snake::display.setPixel(snakePart, Color::GREEN);
    }

    Snake::display.setPixel(foodPosition, Color::RED);
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

      Direction oppositeDirection = directionOppositeMap[direction];
      Direction newDirection = directionCounterClockWiseMap[direction];

      if (oppositeDirection == lastDirection)
      {
        return;
      }

      direction = newDirection;
    }
    else if (button == ACTION_2)
    {
      Direction oppositeDirection = directionOppositeMap[direction];
      Direction newDirection = directionClockWiseMap[direction];

      if (oppositeDirection == lastDirection)
      {
        return;
      }

      direction = newDirection;
    }
  }
};