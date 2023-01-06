#pragma once

#include "ini.h"
#include <fstream>

class iniReader
{
    public:

    	iniReader();
	    ~iniReader();

        int size;
        int crt_enabled;

    private:
        void readIni();


};