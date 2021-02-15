#include "TConsole.h"
#include "Common.h"
#include "Compat.h"

#include <ctime>
#include <sstream>

std::string GetDate() {
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    time_t tt = std::chrono::system_clock::to_time_t(now);
    tm local_tm {};
#ifdef WIN32
    localtime_s(&local_tm, &tt);
#else // unix
    localtime_r(&tt, &local_tm);
#endif // WIN32
    std::stringstream date;
    int S = local_tm.tm_sec;
    int M = local_tm.tm_min;
    int H = local_tm.tm_hour;
    std::string Secs = (S > 9 ? std::to_string(S) : "0" + std::to_string(S));
    std::string Min = (M > 9 ? std::to_string(M) : "0" + std::to_string(M));
    std::string Hour = (H > 9 ? std::to_string(H) : "0" + std::to_string(H));
    date
        << "["
        << local_tm.tm_mday << "/"
        << local_tm.tm_mon + 1 << "/"
        << local_tm.tm_year + 1900 << " "
        << Hour << ":"
        << Min << ":"
        << Secs
        << "] ";
    /* TODO
    if (Debug) {
        date << ThreadName()
             << " ";
    }
    */
    return date.str();
}

TConsole::TConsole() {
    _Commandline.enable_history();
    _Commandline.set_history_limit(20);
    _Commandline.set_prompt("> ");
    _Commandline.on_command = [this](Commandline& c) {
        auto cmd = c.get_command();
        _Commandline.write("> " + cmd);
        if (cmd == "exit") {
            info("gracefully shutting down");
            Application::GracefullyShutdown();
        } else if (cmd == "clear" || cmd == "cls") {
            // TODO: clear screen
        } else {
            // TODO: execute as lua
        }
    };
}

void TConsole::Write(const std::string& str) {
    auto ToWrite = GetDate() + str;
    _Commandline.write(ToWrite);
    // TODO write to logfile, too
}

