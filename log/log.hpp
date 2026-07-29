#pragma once

#include <iostream>
#include <fstream>
#include <chrono>

static const std::string logPath = "./resources/log/";
static const std::string configPath = "./resources/config/";

class Log {
    public:
        static const int TRIM_LINES = 400;

        
        static std::string getTimestamp()
        {
            auto now = std::chrono::system_clock::now();
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                now.time_since_epoch()
            ) % 1000;

            auto time = std::chrono::system_clock::to_time_t(now);

            std::tm tm{};
            localtime_s(&tm, &time);

            return std::format("{:04}/{:02}/{:02}/{:02}:{:02}:{:02}.{:03}",
                tm.tm_year + 1900,
                tm.tm_mon + 1,
                tm.tm_mday,
                tm.tm_hour,
                tm.tm_min,
                tm.tm_sec,
                ms.count()
            );
        }

        static void info(const std::string& message) {
            logIfEnabled("info", message);
        }

        static void warning(const std::string& message) {
            logIfEnabled("warning", message);
        }

        static void error(const std::string& message) {
            logIfEnabled("error", message);
        }

        static void logIfEnabled(const std::string& level, const std::string& message) {
            if(loggingEnabled()) {
                log(level, message);
            }
        }

        static void log(std::string level, const std::string& message) {
            std::string levelStamp = "";
            for (char& c : level) {
                levelStamp += std::toupper(c);
            }

            std::string stamp = "[" + getTimestamp() + "] [" + levelStamp + "] ";

            std::string fileName = logPath + level + ".log";

            trimLogFile(fileName);
            std::ofstream log(fileName, std::ios::app);
            std::cout << stamp << message << std::endl; // log
            log << stamp << message << std::endl; // log to file
        }

        static void trimLogFile(const std::string& filename);

    private:

        static bool loggingEnabled() {
            // check if logging is enabled.
            static bool enabled = Log::readLoggingEnabledFromConfig(); // this is static, so file read only occurs on first function run, then the value is cached for future calls.
            return enabled; 
        }

        // TODO: implement a class to handle config for efficiency and versatility.
        static bool readLoggingEnabledFromConfig() {

            log("logger", "Making first time logging check...");

            std::ifstream config(configPath + "logger_config.txt");
            std::string line;

            if (!config) {
                log("logger", "Failed to open logger_config.txt. Logging will be disabled.");
                log("logger", "Path: " + configPath);
                return false;
            }

            while (std::getline(config, line)) {
                
                log("logger", "reading config line: " + line);
                
                if (line == "enabled=true") {
                    log("logger", "Logging is enabled.");
                    return true;
                }
            }

            log("logger", "Logging is disabled.");

            return false;
        }
};