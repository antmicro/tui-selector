#include <selector-parser.hpp>
#include <set>

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <fstream>

SelectorRule::SelectorRule(
        std::string searchdirectory,
        std::string regex,
        std::string command,
        std::string entryformat) :
    searchdirectory(searchdirectory),
    regex(regex),
    command(command),
    entryformat(entryformat)
{
}

SelectorParser::SelectorParser(std::string jsondir) :
    jsondir(jsondir)
{
}

int SelectorParser::loadRules(bool allowfailure)
{
    std::set<std::filesystem::path> jsonfiles;
    for (auto file : std::filesystem::directory_iterator(jsondir))
    {
        if (file.path().extension() == ".json")
        {
            jsonfiles.insert(file.path());
        }
    }
    for (auto jsonfile : jsonfiles)
    {
        std::ifstream ifs(jsonfile);
        rapidjson::IStreamWrapper isw(ifs);

        rapidjson::Document rulefile;
        rapidjson::ParseResult ok = rulefile.ParseStream(isw);

        if (!ok)
        {
            if (allowfailure)
            {
                continue;
            }
            else
            {
                printf("Failed to parse %s\n", jsonfile.c_str());
                return 1;
            }
        }

        for (auto& it : rulefile.GetArray())
        {
            if (!it.HasMember("searchdirectory") ||
                !it.HasMember("regex") ||
                !it.HasMember("command") ||
                !it.HasMember("entryformat"))
            {
                if (allowfailure)
                {
                    continue;
                }
                else
                {
                    printf("Invalid entry in %s\n", jsonfile.c_str());
                    return 2;
                }
            }
            selectorrules.push_back(
                SelectorRule(
                    it["searchdirectory"].GetString(),
                    it["regex"].GetString(),
                    it["command"].GetString(),
                    it["entryformat"].GetString()
                )
            );
        }
    }
    return 0;
}

int SelectorParser::findEntries()
{
    for (auto rule : selectorrules)
    {
        std::vector<std::filesystem::path> paths;
        for (auto &p : std::filesystem::directory_iterator(rule.searchdirectory))
        {
            if (std::regex_match(p.path().string(), rule.regex))
            {
                paths.push_back(p.path());
            }
        }
        std::sort(paths.begin(), paths.end(), std::greater<>());
        for (auto a : paths)
        {
            std::string entryname = std::regex_replace(a.string(), rule.regex, rule.entryformat);
            std::string command = std::regex_replace(a.string(), rule.regex, rule.command);
            if (selectorentries.find(entryname) != selectorentries.end())
            {
                printf("ERROR: Duplicated entry: %s\n", entryname.c_str());
                return 1;
            }
            selectorentries[entryname] = command;
            entrynames.push_back(entryname);
        }
    }
    return 0;
}

std::vector<std::string>& SelectorParser::getOptions()
{
    return entrynames;
}

std::string SelectorParser::getCommand(std::string option)
{
    return selectorentries[option];
}
