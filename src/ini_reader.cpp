#include "include/ini_reader.h"

iniReader::iniReader()
{
	readIni();
}

iniReader::~iniReader()
{
}

void iniReader::readIni()
{
    inipp::Ini<char> ini;
	std::ifstream is("assets/config.ini");
	ini.parse(is);


	inipp::get_value(ini.sections["window"], "size", size);
	inipp::get_value(ini.sections["crt"], "enabled", crt_enabled);
}