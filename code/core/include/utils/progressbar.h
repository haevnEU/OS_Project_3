#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include <iostream>
#include <fstream>

extern "C"{
    #include <sys/ioctl.h>
    #include <unistd.h>
    #include <unistd.h>
}

/**
    * @brief This class contains a progressbar for CLI
    * @details This class will handle a progessbar.
    *          The object will only work for a command line interface.
    *          The progressbar will look like
    *          \code [====================>                    ] (50%) \endcode
    *          There are different visual states of the progressbar
    *          <table>
    *              <caption id="colore_codes">Possible color codes</caption>
    *              <tr><th>Color</th><th>Description</th></tr>
    *              <tr><td>Green</td><td>Progressbar finished and the operation associated should be successfull</td></tr>
    *              <tr><td>Yellow</td><td></td>The operation was canceled</tr>
    *              <tr><td>Red</td><td>The operation failed or was aborted</td></tr>
    *
    *              <tr><td>White</td><td>Default foreground color</td></tr>
    *              <tr><td>Blue</td><td>Custom foreground color</td></tr>
    *              <tr><td>Cyan</td><td>Custom foreground color</td></tr>
    *              <tr><td>Purple</td><td>Custom foreground color</td></tr>
    *          </table>
    * @author Nils Milewski(nils.milewski@outlook.de)
    * @version 1.0
    * @todo Add windows support
    */
class ProgressBar{
    const bool experimental_width = false;


private:

    /**
     * @brief Resets the terminal to default
     */
    const char* RESET  = "\x1B[0m";

    /**
     * @brief Sets the progressbar color a red
     * @details This attributes i internally used to represent an error state
     */
    const char* RED    = "\x1B[31m";

    /**
     * @brief Sets the progressbar color yellow
     * @details This attributes i internally used to represent a canceled state
     */
    const char* YELLOW = "\x1B[33m";

    /**
     * @brief Sets the progressbar color green
     * @details This attributes i internally used to represent a finish state
     */
    const char* GREEN  = "\x1B[32m";


    /**
     * @brief Sets the progressbar color to blue
     */
    const char* BLUE   = "\x1B[34m";

    /**
     * @brief Sets the progressbar color to purple
     */
    const char* PURPLE = "\x1B[35m";

    /**
     * @brief Sets the progressbar color to cyan
     */
    const char* CYAN   = "\x1B[36m";

    /**
     * @brief Sets the progressbar color to white
     */
    const char* WHITE  = "\x1B[37m";

    /**
     * @brief This attribute is used internally to represent the progressbar color
     */
    const char* FOREGROUND[4] = {WHITE, BLUE, PURPLE, CYAN};

    /**
     * @brief The current value of the progressbar
     * @details This attribute should never be manipulated by hand
     */
    double progress_bar_value;

    /**
     * @brief This attribute describes the 100% value
     */
    int maximum_m;

    /**
     * @brief This attribute is the progressbar width.
     * @details Important note this value should never be edited by hand. In a future release it will be set to half window size
     */
    int bar_width = 50; // DO NOT CHANGE

    /**
     * @brief This attribute indicates if the progressbar reached 100%
     * @details If this attribute is true no operation, except reset will be possible
     */
    bool done = false;

    /**
     * @brief This attribute describes where the progressbar should be drawn.
     * @details Default is std::cout but it can be manipulated to another stream.
     */
    std::ostream& out_stream;

    /**
     * @brief This attributes is used for a smoother rendering
     * @details This attributes ensures that the progressbar will be rendered
     *          that many steps. It is used to create a smoother visual effect.
     *          Higher value => Slower progressbar
     */
    int smooth_render_steps = 0;

    /**
     * @brief This attribute describes the progressbar color
     */
    unsigned char user_color = 0;

public:
    /**
     * @brief Construct a new progressbar object
     * @param os Stream where the progressbar should be drawn, default std::cout
     */
    ProgressBar(std::ostream & os = std::cout): out_stream(os) {
        if(experimental_width){
            struct winsize size;
            ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

            bar_width = size.ws_col * .5;
        }
    }

    /**
     * @brief updates the value
     * @details Updates the progressbar by \p step . Default is 1
     * @param step Step to increment, default 1. If negative the progressbar will rewind
     */
    void update(double step = 1) {
        progress_bar_value += step;
        if(progress_bar_value >= maximum()){
            finish();
        }else{
            for(int i = 0; i < 10; ++i){
                out_stream << FOREGROUND[user_color];
                render();
                out_stream << RESET;
            }
        }
    }

    /**
     * @brief This method sets the color of the progressbar.
     * @details The method required an argument as index, if the index is greater than 3
     *          a modulo operation will be done to ensure the index is in range of the color array.
     *          There a 4 different possible colors: <br>
     *          \p White <br>
     *          \p Blue <br>
     *          \p Purple <br>
     *          \p Cyan <br>
     * @param color Color index in range of 0 - 3
     */
    void progressbar_color(unsigned char color){
        user_color = color % 4;

    }

    /**
     * @brief Sets the maximum value
     * @param max New maximum
     */
    void maximum(int max){
        maximum_m = max;
    }

    /**
     * @brief Gets the maximum value
     * @return int Maximum value of the progressbar
     */
    int maximum(){
        return maximum_m;
    }

    /**
     * @brief Gets the current value
     * @return int Current value of the progressbar
     */

    int value(){
        return progress_bar_value;
    }

    /**
     * @brief Cancels the progressbar
     * @details This method cancel the progressbar. The progressbar will be rendered in yellow indicating a cancel state.
     *          This operation aborts if the progressbar is allready at 100%.
     */
    void cancel(){
        if(done){
            return;
        }
        out_stream << YELLOW;
        force_print();
        out_stream << RESET;
        done = true;
    }

    /**
     * @brief Aborts the progressbar
     * @details This method aborts the progressbar. The progressbar will be rendered in red indicating a abort state.
     *          This operation aborts if the progressbar is allready at 100%.
     *          Note this method can be used as a error indication
     */
    void abort(){
        if(done){
            return;
        }
        out_stream << RED;
        force_print();
        out_stream << RESET;
        done = true;
    }

    /**
     * @brief Finishes the progressbar
     * @details This method finishes the progressbar. The progressbar will be rendered in green indicating a successfull state
     *          This operation aborts if the progressbar is allready at 100%
     */
    void finish(){
        if(done){
            return;
        }
        out_stream << GREEN;
        force_print();
        out_stream << RESET << std::endl;
        done = true;
    }

    /**
     * @brief This method resets the progressbar
     * @details The current value will be set to 0.
     *          If the progressbar was finished, all methods will be available again.
     *          It will also render the progressbar empty
     */
    void reset(){
        done = false;
        progress_bar_value = 0;
        force_print();
    }

private:

    /**
     * @brief Internal usage.
     * @details This method forces to render the progressbar.
     *          This method is deprecated, use render
     * @deprecated since 1.0
     */
    void force_print() {
        render();
    }

    /**
     * @brief This method renders the progressbar
     * @details The method will calculate how wide the progessbar should be rendered.
     *          Later it will override the previous progressbar with a new one based on previous previous calculated width.
     */
    void render(){
        int amountOfFiller = (int)((progress_bar_value / maximum())*(double)bar_width);

        out_stream << "\r[";
        for (int a = 0; a < amountOfFiller; a++) {
            out_stream << "=";
        }
        out_stream << ">";
        for (int b = 0; b < bar_width - amountOfFiller; b++) {
            out_stream << " ";
        }
        out_stream << "] (" << (int)(100*(progress_bar_value / maximum())) << "%)" << std::flush;

    }
};

#endif // PROGRESSBAR_H
