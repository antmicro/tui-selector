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

class TUISelector
{
    public:
        TUISelector() {}

        void render();
        void close();
        bool shouldClose();

        void addOption(const char *option);

        bool waitForAction(std::string &selected);

        ~TUISelector();
    private:
        bool shouldclose = false;
        std::vector<const char *> options;
        std::vector<ITEM *> items;

        WindowPtr menuwindow;
        MenuPtr menu;

        WindowPtr helpwindow;

        WindowPtr createWindow(int x, int y, int width, int height);
        void removeItems();

        std::vector<std::string> helpmessage =
            {"Use arrows, PGUP and PGDOWN to move in the view.",
             "Use Enter to select the option.",
             "Use F1 or ESC to exit."};
        
};
