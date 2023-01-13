#include "ini_manager.h"

IniManager::IniManager(){
	readIni();
}

IniManager::~IniManager(){
}

void IniManager::readIni(){
	mINI::INIFile file("assets/config.ini");
	mINI::INIStructure ini;
	file.read(ini);
	size = stoi(ini.get("window").get("size"));
}