#include <tui-menu.hpp>
#include <selector-parser.hpp>

int main(int argc, char *argv[])
{
    auto selector = std::unique_ptr<TUISelector>(new TUISelector());
    
    auto selpar = SelectorParser(argv[1]);
    int res = selpar.loadRules(false);
    if (res)
    {
        return res;
    }
    res = selpar.findEntries();
    if (res)
    {
        return res;
    }

    for (auto &option : selpar.getOptions())
    {
        selector->addOption(option.c_str());
    }

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
        system(selpar.getCommand(option).c_str());
    }

    return 0;
}
