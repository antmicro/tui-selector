#include <tui-menu.hpp>
#include <selector-parser.hpp>
#include <cxxopts.hpp>

int main(int argc, char *argv[])
{
    std::string rulesdirectory = "";
    bool allowfailure = false;
    int timeout = 3;

    cxxopts::Options options(argv[0], "Executable TUI selector");

    options.add_options()
        ("rules_directory", "Path to the directory with JSON rules files", cxxopts::value<std::string>(rulesdirectory))
        ("allow-failure", "Ignore invalid entries in JSON rules files", cxxopts::value<bool>(allowfailure))
        ("timeout", "Set timeout for user action (negative value for no timeout", cxxopts::value<int>(timeout))
        ("h,help", "Show help");

    options.parse_positional({"rules_directory"});
    options.positional_help("RULES_DIRECTORY");

    auto result = options.parse(argc, argv);

    if (result.count("help"))
    {
        printf("%s\n", options.help().c_str());
        return 0;
    }

    auto selector = std::unique_ptr<TUISelector>(new TUISelector());
    
    if (rulesdirectory.empty())
    {
        printf("rules_directory is not provided!\n%s\n", options.help().c_str());
        return 1;
    }

    auto selectorparser = SelectorParser(rulesdirectory);
    int res = selectorparser.loadRules(allowfailure);
    if (res)
    {
        return res;
    }
    res = selectorparser.findEntries();
    if (res)
    {
        return res;
    }

    for (auto &option : selectorparser.getOptions())
    {
        selector->addOption(option.c_str());
    }

    selector->setTimeout(timeout);
    selector->render();

    std::string option = "";
    while (!selector->shouldClose())
    {
        if (selector->waitForAction(option))
        {
            break;
        }
    }

    selector.release();
    endwin();

    if (option != "")
    {
        return system(selectorparser.getCommand(option).c_str());
    }

    return 0;
}
