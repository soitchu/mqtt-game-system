#include <iostream>
#include "./Games/Snake.cpp"
#include <mqtt/async_client.h>

const std::string SERVER_ADDRESS = "tcp://localhost:1883";
const std::string CLIENT_ID = "async_client_cpp";
const std::string TOPIC = "test/topic";

Snake snake_game(64, 64);
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
        client.subscribe("input", 0);
    }

    void connection_lost(const std::string &cause) override
    {
        std::cerr << "Connection lost: " << cause << std::endl;
    }

    void message_arrived(mqtt::const_message_ptr msg) override
    {
        std::string payload = msg->to_string();

        snake_game.buttonPressed(Player::ONE, (Button)(payload[1] - '1'));
    }
};

int main()
{
    Callback cb;
    client.set_callback(cb);

    std::thread game_thread = std::thread(
        setInterval, []()
        {
            const char *payload = snake_game.getDisplayBuffer();
            mqtt::message_ptr msg = mqtt::make_message(TOPIC, payload);

            if (client.is_connected())
            {
                client.publish(msg);
                snake_game.tick();
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
