#pragma once

#include "ini.h"
#include <fstream>

class IniManager
{
    public:

    	IniManager();
	    ~IniManager();

        int size;
        int crt_enabled;

    private:
        void readIni();


};