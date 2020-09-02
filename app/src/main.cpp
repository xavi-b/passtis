#include "passtis.h"

#include <iostream>
#include <exception>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

int main(int argc, char* argv[])
{
    try
    {
        auto logger = spdlog::basic_logger_mt("filelogger", "logs/passtis.txt");
    }
    catch (const spdlog::spdlog_ex& ex)
    {
        std::cerr << "sodlog init failed: " << ex.what() << std::endl;
        return 1;
    }

    spdlog::get("filelogger")->info("passtis");
#ifdef DEBUG
    spdlog::set_level(spdlog::level::debug);
    spdlog::debug("DEBUG");
#endif

    try
    {
        Passtis::instance()->init(argc, argv);
        return Passtis::instance()->exec();
    }
    catch (const std::exception& e)
    {
        spdlog::critical(e.what());
        return 1;
    }
}
