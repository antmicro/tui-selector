#include <tui-menu.hpp>

int main()
{
    TUISelector selector;
    
    selector.addOption("image1", "image1");
    selector.addOption("image2", "image2");
    selector.addOption("image3", "image3");

    selector.init();
    while (!selector.shouldClose())
    {
        std::string option = "";
        selector.waitForAction(option);
    }

    endwin();
    return 0;
}
