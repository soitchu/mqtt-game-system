#include <iostream>
#include <mqtt/async_client.h>
#include "led-matrix.h"
#include "graphics.h"
#include <semaphore>

const std::string SERVER_ADDRESS = "tcp://34.105.9.147:1883";
const std::string CLIENT_ID = "async_client_cpp_2";
const std::string TOPIC = "test/topic";
rgb_matrix::RGBMatrix *matrix = NULL;
rgb_matrix::RGBMatrix::Options matrix_options;
rgb_matrix::RuntimeOptions runtime_opt;
rgb_matrix::FrameCanvas *offscreen = NULL;
std::string display_payload;
mqtt::async_client client(SERVER_ADDRESS, CLIENT_ID);
std::counting_semaphore semaphore(0);
auto last = std::chrono::high_resolution_clock::now();

void updateDisplay()
{
    int colors[8][3] = {
        {0, 0, 0},
        {255, 0, 0},
        {0, 255, 0},
        {0, 255, 255},
        {0, 0, 255},
        {255, 0, 255},
        {255, 255, 0},
        {255, 255, 255},
    };

    for (int i = 0; i < display_payload.size(); i++)
    {
        int colorId = display_payload[i] - 'a';
        int *color = colors[colorId];

        int x = i % 64;
        int y = i / 64;

        offscreen->SetPixel(x, y, color[0], color[1], color[2]);
    }

    offscreen = matrix->SwapOnVSync(offscreen);
}

void setInterval(std::function<void()> func, int interval)
{
    while (true)
    {
        func();
        semaphore.acquire();
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
        std::cout << "Connected to MQTT broker" << std::endl;
        std::cout << "Subscribing to topic: " << TOPIC << std::endl;
        client.subscribe(TOPIC, 0);
    }

    void connection_lost(const std::string &cause) override
    {
        std::cerr << "Connection lost: " << cause << std::endl;
    }

    void message_arrived(mqtt::const_message_ptr msg) override
    {
        display_payload = msg->to_string();
        semaphore.release();
    }
};

void ini(int width, int height)
{
    matrix_options.rows = width;
    matrix_options.cols = height;
    matrix_options.hardware_mapping = "adafruit-hat";
    // runtime_opt.gpio_slowdown = 3;
    // TODO: find a workaround for this
    runtime_opt.drop_privileges = 0;

    matrix = rgb_matrix::RGBMatrix::CreateFromOptions(matrix_options, runtime_opt);
    offscreen = matrix->CreateFrameCanvas();

    offscreen->SetBrightness(100);
    // offscreen->Fill(255, 255, 255);
    // offscreen = matrix->SwapOnVSync(offscreen);
}

int main()
{
    Callback cb;
    client.set_callback(cb);

    ini(64, 64);

    // Connect options
    mqtt::connect_options conn_opts;
    conn_opts.set_keep_alive_interval(20);
    conn_opts.set_clean_session(true);

    std::thread display_thread = std::thread(
        setInterval, []()
        { updateDisplay(); },
        16);

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

    display_thread.join();

    return 0;
}
