#pragma once

#include <vector>
#include <string>
#include <memory>

#include <ncurses.h>
#include <menu.h>

template <typename T, int delfunc(T *)>
struct Destroy
{
    void operator()(T *obj)
    {
        if (obj) delfunc(obj);
    }
};

int freeMenu(MENU *menu);

using WindowPtr = std::unique_ptr<WINDOW, Destroy<WINDOW, &delwin>>;
using MenuPtr = std::unique_ptr<MENU, Destroy<MENU, &freeMenu>>;
using ItemPtr = std::unique_ptr<ITEM, Destroy<ITEM, &free_item>>;

class TUISelector
{
    public:
        void init();
        bool shouldClose();

        void addOption(std::string name,std::string description);

        bool waitForAction(std::string &selected);
    private:
        bool shouldclose = false;
        std::vector<ItemPtr> entries;

        WindowPtr menuwindow;
        MenuPtr menu;

        WindowPtr helpwindow;

        WindowPtr createWindow(int x, int y, int width, int height);

        std::vector<std::string> helpmessage =
            {"Use arrows, PGUP and PGDOWN to move in the view.",
             "Use Enter to select the option.",
             "Use Q or ESC to exit."};
        
};
