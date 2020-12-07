#pragma once

#include <vector>
#include <string>
#include <memory>

#include <ncurses.h>
#include <menu.h>

/**
 * A wrapper for non-standard destructors for unique_ptr
 */
template <typename T, int delfunc(T *)>
struct Destroy
{
    void operator()(T *obj)
    {
        if (obj) delfunc(obj);
    }
};

/**
 * Runs all methods required to release MENU structures.
 */
int freeMenu(MENU *menu);

using WindowPtr = std::unique_ptr<WINDOW, Destroy<WINDOW, &delwin>>;
using MenuPtr = std::unique_ptr<MENU, Destroy<MENU, &freeMenu>>;

/**
 * ncurses-based wrapper for TUI simple menu.
 */
class TUISelector
{
    public:
        TUISelector() {}

        /**
         * Initializes ncurses and draws windows.
         */
        void render();

        /**
         * Releases all temporary ncurses objects.
         */
        void close();

        /**
         * Tells if the TUI should be closed.
         *
         * @return true if should be closed
         */
        bool shouldClose();

        /**
         * Adds new entry to TUI menu
         *
         * @param option option name
         */
        void addOption(const char *option);

        /**
         * Waits for a keystroke or window resize.
         *
         * @param selected reference to a string variable in which the selected entry will be stored
         * @return true if one of the entries was selected, false otherwise
         */
        bool waitForAction(std::string &selected);

        ~TUISelector();
    private:
        int currtimeout = 3; ///<seconds before automatic action
        bool actionnoticed = false; ///<true if any keyboard movement was detected
        bool shouldclose = false; ///<tells if window should be closed
        std::vector<const char *> options; ///<vector of available options
        std::vector<ITEM *> items; ///<menu entries

        WindowPtr menuwindow; ///<window holding menu
        MenuPtr menu; ///<menu object

        WindowPtr helpwindow; ///<window holding help

        /**
         * Creates a new window object.
         *
         * @param x x coordinate in terminal
         * @param y y coordinate in terminal
         * @param width width of the window
         * @param height height of the window
         * @return unique_ptr to WINDOW object
         */
        WindowPtr createWindow(int x, int y, int width, int height);
        void removeItems();

        /// help message for the TUI menu
        std::vector<std::string> helpmessage =
            {"",
             "Use arrows, PGUP and PGDOWN to move in the view.",
             "Use Enter to select the option.",
             "Use F1 or ESC to exit."};
        
};
