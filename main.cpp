#include <iostream>
#include "./Games/Tron.cpp"
#include <mqtt/async_client.h>

const std::string SERVER_ADDRESS = "tcp://localhost:1883";
const std::string CLIENT_ID = "async_client_cpp";
const std::string TOPIC = "test/topic";

Tron game = Tron(64, 64);
mqtt::async_client client(SERVER_ADDRESS, CLIENT_ID);

void setInterval(std::function<void()> func, int interval)
{
  while (true)
  {
    auto start = std::chrono::high_resolution_clock::now();

    func();

    auto end = std::chrono::high_resolution_clock::now();
    std::this_thread::sleep_for(std::chrono::milliseconds(interval) - (end - start));
  }
}

class Callback : public virtual mqtt::callback, public virtual mqtt::iaction_listener
{
public:
  void on_failure(const mqtt::token &tok) override
  {
    std::cerr << "Connection attempt failed" << std::endl;
  }

  void on_success(const mqtt::token &tok) override
  {
    std::cout << "Connection successful" << std::endl;
  }

  void connected(const std::string &cause) override
  {
    client.subscribe("input_1", 0);
    client.subscribe("input_2", 0);
  }

  void connection_lost(const std::string &cause) override
  {
    std::cerr << "Connection lost: " << cause << std::endl;
  }

  void message_arrived(mqtt::const_message_ptr msg) override
  {
    if(game.isGameOver) {
      game = Tron(64, 64);
      return;
    }
    std::string payload = msg->to_string();
    std::string topic = msg->get_topic();
    Button pressedButton = (Button)(payload[0] - '1');

    if (topic == "input_1")
    {
      game.buttonPressed(Player::ONE, pressedButton);
    }
    else if (topic == "input_2")
    {
      game.buttonPressed(Player::TWO, pressedButton);
    }
  }
};

int main()
{
  Callback cb;
  client.set_callback(cb);

  std::thread game_thread = std::thread(
      setInterval, []()
      {

            if (game.isGameOver) {
              return;
            }
            game.tick();

            const char *payload = game.getDisplayBuffer();
            mqtt::message_ptr msg = mqtt::make_message(TOPIC, payload);

            if (client.is_connected())
            {
                client.publish(msg);
            } },
      40);

  // Connect options
  mqtt::connect_options conn_opts;
  conn_opts.set_keep_alive_interval(20);
  conn_opts.set_clean_session(true);

  try
  {
    std::cout << "Connecting to MQTT broker..." << std::endl;
    client.connect(conn_opts)->wait(); // Async connect
  }
  catch (const mqtt::exception &e)
  {
    std::cerr << "MQTT Error: " << e.what() << std::endl;
    return 1;
  }

  // Clean up game thread
  game_thread.join();

  return 0;
}
