#include "include/ini_manager.h"

iniReader::iniReader(){
	readIni();
}

iniReader::~iniReader(){
}

void iniReader::readIni(){
	mINI::INIFile file("assets/config.ini");
	mINI::INIStructure ini;
	file.read(ini);
	size = stoi(ini.get("window").get("size"));
}