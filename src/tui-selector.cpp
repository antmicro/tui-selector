#include <tui-menu.hpp>

int main()
{
    auto selector = std::unique_ptr<TUISelector>(new TUISelector());
    
    selector->addOption("image01");
    selector->addOption("image02");
    selector->addOption("image03");
    selector->addOption("image04");
    selector->addOption("image05");
    selector->addOption("image06");
    selector->addOption("image07");
    selector->addOption("image08");
    selector->addOption("image09");
    selector->addOption("image10");
    selector->addOption("image11");
    selector->addOption("image12");
    selector->addOption("image13");
    selector->addOption("image14");
    selector->addOption("image15");
    selector->addOption("image16");
    selector->addOption("image17");
    selector->addOption("image18");
    selector->addOption("image19");

    selector->render();
    while (!selector->shouldClose())
    {
        std::string option = "";
        selector->waitForAction(option);
    }

    selector.release();
    endwin();
    return 0;
}
