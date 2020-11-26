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

int SelectorParser::loadRules()
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
        rulefile.ParseStream(isw);

        for (auto& it : rulefile.GetArray())
        {
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
