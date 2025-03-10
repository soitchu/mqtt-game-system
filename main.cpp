#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <mqtt/client.h>
#include "Games/Sample.cpp"

const std::string BROKER_ADDRESS{"tcp://localhost:1883"};
const std::string CLIENT_ID{"paho_cpp_client"};
const std::string TOPIC{"test/topic"};

int main(int argc, char *argv[])
{
  mqtt::client client(BROKER_ADDRESS, CLIENT_ID);
  mqtt::connect_options connOpts;
  connOpts.set_keep_alive_interval(20);
  connOpts.set_clean_session(true);

  try
  {
    client.connect(connOpts);
    std::cout << "Connected to MQTT broker" << std::endl;

    int message_count = 0;
    
    Snake sample(64, 64);

    while (true)
    {
      // Create a message
      std::string payload = sample.getDisplayBuffer();
      mqtt::message msg(TOPIC, payload, 1, false);
      client.publish(msg);

      sample.tick();

      std::this_thread::sleep_for(std::chrono::milliseconds(16));

      message_count++;
    }

    client.disconnect();
    std::cout << "Disconnected from MQTT broker" << std::endl;
  }
  catch (const mqtt::exception &exc)
  {
    std::cerr << exc.what() << std::endl;
    return 1;
  }

  return 0;
}