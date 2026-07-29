#include <fstream>
#include <deque>
#include <string>

#include "log.hpp"




void Log::trimLogFile(const std::string& filename)
{
    std::ifstream in(filename);
    if (!in.is_open()) return;

    std::deque<std::string> lines;
    std::string line;

    while (std::getline(in, line))
    {
        lines.push_back(line);
    }
    in.close();

    if (lines.size() <= TRIM_LINES)
        return;

    std::ofstream out(filename, std::ios::trunc);

    for (size_t i = lines.size() - TRIM_LINES; i < lines.size(); ++i)
    {
        out << lines[i] << '\n';
    }
}