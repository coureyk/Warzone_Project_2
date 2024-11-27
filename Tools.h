//#include <time.h>
#include <windows.h>

class Tools{

public:
    inline static void waitForSeconds(double seconds){
        // time_t initialTime = time(&initialTime);
        // time_t finalTime;

        // while(time(&finalTime)-initialTime < seconds){
            
        // }

        Sleep(seconds*1000);
    }

};