#include <tui-menu.hpp>

int freeMenu(MENU *menu)
{
    unpost_menu(menu);
    return free_menu(menu);
}

WindowPtr TUISelector::createWindow(int x, int y, int width, int height)
{
    auto win = WindowPtr(newwin(height, width, y, x));
    box(win.get(), 0, 0);
    wrefresh(win.get());
    return win;
}

void TUISelector::init()
{
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    std::vector<ITEM *> items;

    helpwindow = createWindow(
            0,
            LINES - (helpmessage.size() + 2),
            COLS,
            helpmessage.size() + 2
    );

    for (auto &item : entries)
    {
        items.push_back(item.get());
    }

    menu = MenuPtr(new_menu(items.data()));

    menuwindow = createWindow(
        0,
        0,
        COLS,
        LINES - (helpmessage.size() + 3)
    );

    keypad(menuwindow.get(), TRUE);

    set_menu_win(menu.get(), menuwindow.get());

    set_menu_format(menu.get(), LINES - (helpmessage.size() + 5), 15);

    set_menu_mark(menu.get(), " > ");

    box(menuwindow.get(), 0, 0);
    box(helpwindow.get(), 0, 0);

    mvwaddch(menuwindow.get(), 2, 0, ACS_LTEE);
    mvwhline(menuwindow.get(), 2, 1, ACS_HLINE, 38);
    mvwaddch(menuwindow.get(), 2, 0, ACS_RTEE);
    refresh();

    for (unsigned int i = 0; i < helpmessage.size(); i++)
    {
        mvwprintw(helpwindow.get(), i + 1, 1, helpmessage[i].c_str());
    }
    
    wrefresh(helpwindow.get());

    post_menu(menu.get());
    wrefresh(menuwindow.get());
}

bool TUISelector::shouldClose()
{
    return shouldclose;
}

void TUISelector::addOption(std::string name, std::string description)
{
    entries.push_back(ItemPtr(new_item(name.c_str(), description.c_str())));
}

bool TUISelector::waitForAction(std::string &selected)
{
    switch (wgetch(menuwindow.get()))
    {
        case KEY_DOWN:
            menu_driver(menu.get(), REQ_DOWN_ITEM);
            return false;
        case KEY_UP:
            menu_driver(menu.get(), REQ_UP_ITEM);
            return false;
        case KEY_NPAGE:
            menu_driver(menu.get(), REQ_SCR_DPAGE);
            return false;
        case KEY_PPAGE:
            menu_driver(menu.get(), REQ_SCR_UPAGE);
            return false;
        case KEY_RESIZE:
            return false;
        case KEY_F(1):
        case 27:
        case 133:
            shouldclose = true;
            return false;
    }
    wrefresh(menuwindow.get());
    return false;
}
