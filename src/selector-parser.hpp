#pragma once

#include <filesystem>
#include <regex>

/**
 * A struct that contains all parameters describing a single selector rule.
 */
struct SelectorRule
{
    SelectorRule(
        const std::string &searchdirectory,
        const std::string &regex,
        const std::string &command,
        const std::string &entryformat);
    std::filesystem::path searchdirectory; ///< path where the files are located
    std::regex regex; ///< filter for files in search directory
    std::string command; ///< a command to be executed for a given file
    std::string entryformat; ///< a format of entry for a rule in TUI menu
};

/**
 * Parser for JSON files describing selector rules.
 */
class SelectorParser
{
    public:
        /**
         * Constructs the parser.
         *
         * @param jsondir path to the directory with JSON files
         */
        SelectorParser(const std::string &jsondir);

        /**
         * Loads rules from JSON files located in jsondir.
         *
         * @param allowfailure if true, the function won't fail on invalid JSON file or entry, but will proceed with other files
         * @return 0 if executed successfuly
         */
        int loadRules(bool allowfailure);

        /**
         * Runs all rules and creates entries from found files.
         *
         * @return 0 if executed successfully
         */
        int findEntries();

        /**
         * Gets vector of available options.
         *
         * @return vector of available options
         */
        std::vector<std::string>& getOptions();

        /**
         * Returns command for a given option.
         *
         * @param option from getOptions
         * @return respective command
         */
        std::string getCommand(std::string option);
    private:
        std::filesystem::path jsondir; ///<Path to directory with JSON files describing rules
        std::vector<SelectorRule> selectorrules; ///<vector of parsed rules

        std::map<std::string, std::string> selectorentries; ///<vector of parsed entries
        std::vector<std::string> entrynames; ///<vector of entry names
};
