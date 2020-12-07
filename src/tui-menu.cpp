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

void TUISelector::removeItems()
{
    for (auto item : items)
    {
        if (item) free_item(item);
    }
    items.clear();
}

void TUISelector::render()
{
    close();

    initscr();
    start_color();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    items.clear();
    for (auto option : options)
    {
        items.push_back(new_item(option, ""));
    }
    items.push_back(NULL);

    int lines = 0;
    int cols = 0;
    getmaxyx(stdscr, lines, cols);
    helpwindow = createWindow(
            0,
            lines - (helpmessage.size() + 2),
            cols,
            helpmessage.size() + 2
    );

    menu = MenuPtr(new_menu(items.data()));

    menuwindow = createWindow(
        0,
        0,
        cols,
        lines - (helpmessage.size() + 3)
    );

    keypad(menuwindow.get(), TRUE);

    set_menu_win(menu.get(), menuwindow.get());
    int menuwindowx = 0;
    int menuwindowy = 0;
    getmaxyx(menuwindow.get(), menuwindowy, menuwindowx);
    set_menu_sub(menu.get(), derwin(menuwindow.get(), menuwindowy - 2, menuwindowx - 2, 1, 1));
    set_menu_format(menu.get(), menuwindowy - 2, 1);

    set_menu_mark(menu.get(), " > ");

    box(menuwindow.get(), 0, 0);
    box(helpwindow.get(), 0, 0);

    wtimeout(menuwindow.get(), 1000);

    for (unsigned int i = 0; i < helpmessage.size(); i++)
    {
        mvwprintw(helpwindow.get(), i + 1, 1, helpmessage[i].c_str());
    }
    
    refresh();

    wrefresh(helpwindow.get());

    post_menu(menu.get());
    wrefresh(menuwindow.get());
}

void TUISelector::close()
{
    refresh();
    clear();
    menuwindow.release();
    helpwindow.release();
    menu.release();

    removeItems();
}

bool TUISelector::shouldClose()
{
    return shouldclose;
}

void TUISelector::addOption(const char *option)
{
    options.push_back(option);
}

bool TUISelector::waitForAction(std::string &selected)
{
    wclear(helpwindow.get());
    box(helpwindow.get(), 0, 0);
    if (actionnoticed)
    {
        helpmessage[0] = "";
    }
    else
    {
        helpmessage[0] = std::string("Automatic selection (first option) in ") + std::to_string(currtimeout) + " seconds.";
    }
    for (unsigned int i = 0; i < helpmessage.size(); i++)
    {
        mvwprintw(helpwindow.get(), i + 1, 1, helpmessage[i].c_str());
    }
    refresh();
    wrefresh(helpwindow.get());
    bool isselected = false;
    switch (wgetch(menuwindow.get()))
    {
        case KEY_DOWN:
            actionnoticed = true;
            menu_driver(menu.get(), REQ_DOWN_ITEM);
            break;
        case KEY_UP:
            actionnoticed = true;
            menu_driver(menu.get(), REQ_UP_ITEM);
            break;
        case KEY_NPAGE:
            actionnoticed = true;
            menu_driver(menu.get(), REQ_SCR_DPAGE);
            break;
        case KEY_PPAGE:
            actionnoticed = true;
            menu_driver(menu.get(), REQ_SCR_UPAGE);
            break;
        case KEY_RESIZE:
            close();
            endwin();
            render();
            break;
        case KEY_F(1):
        case 27:
        case KEY_EXIT:
        case 133:
            actionnoticed = true;
            shouldclose = true;
            break;
        case 10: /* Enter */
            actionnoticed = true;
            selected = item_name(current_item(menu.get()));
            isselected = true;
            pos_menu_cursor(menu.get());
            break;
        case ERR:
            if (!actionnoticed)
            {
                currtimeout--;
                if (currtimeout == 0)
                {
                    selected = options[0];
                    isselected = true;
                }
            }
            break;
    }
    wrefresh(menuwindow.get());
    wrefresh(helpwindow.get());
    refresh();
    return isselected;
}

void TUISelector::setTimeout(int timeoutval)
{
    if (timeoutval <= 0)
    {
        actionnoticed = true;
    }
    else
    {
        currtimeout = timeoutval;
    }
}

TUISelector::~TUISelector()
{
    close();
    removeItems();
}
