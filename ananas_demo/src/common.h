#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <string>
#include <sstream>
#include <unistd.h>
#include <cstdlib>

using namespace std;

auto show_vec = [](const std::string& name, const auto& v) {
    cout << name + " [ ";
    for (const auto& e : v)
    {
        cout << e << ", ";
    }
    cout << "]";
    cout << endl << endl;
};

auto show_map = [](const std::string& name, const auto& m) {
    cout << name << " [ ";
    for (const auto& e : m)
    {
        cout << "(" << e.first << ", " << e.second << "), ";
    }
    cout << "]";
    cout << endl << endl;
};

auto show_map_17 = [](const std::string& name, const auto& m) {
    cout << name << " [ ";
    for (const auto& [ f, s ] : m)
    {
        cout << "(" << f << ", " << s << "), ";
    }
    cout << "]";
    cout << endl << endl;
};

auto show_opt_vec = [](const std::string& name, const auto& v) {
    cout << name + " [ ";
    for (const auto& e : v)
    {
        if (e)
        {
            cout << *e << ", ";
        }
        else
        {
            cout << "null, ";
        }
    }
    cout << "]";
    cout << endl << endl;
};

 auto get_cur_time = []() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&now_c));
    std::stringstream ss;
    ss << buffer << "." << std::setw(3) << std::setfill('0') << ms.count();
    return ss.str();
};

auto show_thread_info = [](){
    std::ostringstream oss;
    oss << "[" << get_cur_time() << "] [" << getpid() << "] [" << std::this_thread::get_id() << "] ";
    std::cout << oss.str();
};
