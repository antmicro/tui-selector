#pragma once

#include <filesystem>
#include <regex>

struct SelectorRule
{
    SelectorRule(
        std::string searchdirectory,
        std::string regex,
        std::string command,
        std::string entryformat);
    std::filesystem::path searchdirectory;
    std::regex regex;
    std::string command;
    std::string entryformat;
};

class SelectorParser
{
    public:
        SelectorParser(std::string jsondir);

        int loadRules();
    private:
        std::filesystem::path jsondir;
        std::vector<SelectorRule> selectorrules;
};
