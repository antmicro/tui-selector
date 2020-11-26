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

        int loadRules(bool allowfailure);
        int findEntries();
        std::vector<std::string>& getOptions();
        std::string getCommand(std::string option);
    private:
        std::filesystem::path jsondir;
        std::vector<SelectorRule> selectorrules;

        std::map<std::string, std::string> selectorentries;
        std::vector<std::string> entrynames;
};
