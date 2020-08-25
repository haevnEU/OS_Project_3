#ifndef UTILS__COLORS__COLOR_H
#define UTILS__COLORS__COLOR_H

namespace utils::colors{
    /**
     * @brief Line ending.
     */
    const static char* endl = "\n";
    
    /**
     * @brief Resets the colorization
     */
    const static char* RESET     = "\x1B[0m";

    /**
     * @brief Colors the foreground in black
     */
    const static char* BLACK     = "\x1B[30m";
    
    /**
     * @brief Colors the foreground in red
     */
    const static char* RED       = "\x1B[31m";
    
    /**
     * @brief Colors the foreground in green
     */
    const static char* GREEN     = "\x1B[32m";
    
    /**
     * @brief Colors the foreground in yellow
     */
    const static char* YELLOW    = "\x1B[33m";
    
    /**
     * @brief Colors the foreground in blue
     */
    const static char* BLUE      = "\x1B[34m";
    
    /**
     * @brief Colors the foreground in magenta
     */
    const static char* MAGENTA   = "\x1B[35m";
    
    /**
     * @brief Colors the foreground in cyan
     */
    const static char* CYAN      = "\x1B[36m";
    
    /**
     * @brief Colors the foreground in white
     */
    const static char* WHITE     = "\x1B[37m";
    
    /**
     * @brief Enables bold font in foreground
     */
    const static char* BOLD      = "\x1B[1m";
    
    /**
     * @brief Enables underlined font in foreground
     */
    const static char* UNDERLINE = "\x1B[4m";

    /**
     * @brief Colors the background black
     */
    const static char* BACK_BLACK     = "\x1B[40m";

    /**
     * @brief Colors the background red
     */
    const static char* BACK_RED       = "\x1B[41m";
    
    /**
     * @brief Colors the background green
     */
    const static char* BACK_GREEN     = "\x1B[42m";
    
    /**
     * @brief Colors the background yellow
     */
    const static char* BACK_YELLOW    = "\x1B[43m";
    
    /**
     * @brief Colors the background blue
     */
    const static char* BACK_BLUE      = "\x1B[44m";
    
    /**
     * @brief Colors the background magenta
     */
    const static char* BACK_MAGENTA   = "\x1B[45m";
    
    /**
     * @brief Colors the background cyan
     */
    const static char* BACK_CYAN      = "\x1B[46m";
    
    /**
     * @brief Colors the background white
     */
    const static char* BACK_WHITE     = "\x1B[47m";

    /**
     * @brief Clears the terminal
     */
    const static char* CLEAR = "\e[1;1H\e[2J";

    /**
     * @brief This is accpet icon without colorization
     */
    const static char* ICON_ACCEPT_RAW = "✓";// "\xE2\x9C\x93";
    
    /**
     * @brief This is denied/decline icon without colorization
     */
    const static char* ICON_DENIED_RAW = "✗";

    /**
     * @brief This is accpet icon with green colorization
     */
    const static char* ICON_ACCEPT = "\x1B[32m✓\x1B[0m";

    /**
     * @brief This is denied/decline icon with red colorization
     */
    const static char* ICON_DENIED = "\x1B[31m✗\x1B[0m";
}

#endif //UTILS__COLORS__COLOR_H
