#include "wren_manager.h"
#include <iostream>
#include <string>
#include <fstream>

void WrenManager::writeFn(WrenVM* vm, const char* text) {
  printf("%s", text);
}

void WrenManager::errorFn(WrenVM* vm, WrenErrorType errorType,
             const char* module, const int line,
             const char* msg)
{
  switch (errorType)
  {
    case WREN_ERROR_COMPILE:
    {
      printf("[%s line %d] [Error] %s\n", module, line, msg);
    } break;
    case WREN_ERROR_STACK_TRACE:
    {
      printf("[%s line %d] in %s\n", module, line, msg);
    } break;
    case WREN_ERROR_RUNTIME:
    {
      printf("[Runtime Error] %s\n", msg);
    } break;
  }
}

WrenManager::WrenManager(){

   std::string line,text;
   std::ifstream in("assets/test.wren");
   while(std::getline(in, line))
   {
       text += line + "\n";
   }
   const char* script = text.c_str();


    wrenInitConfiguration(&wConfig);
        wConfig.writeFn = &writeFn;
        wConfig.errorFn = &errorFn;

    WrenVM* vm = wrenNewVM(&wConfig);
    const char* module = "main";

    WrenInterpretResult result = wrenInterpret(vm, module, script);

    switch (result) {
        case WREN_RESULT_COMPILE_ERROR:
        { printf("Compile Error!\n"); } break;
        case WREN_RESULT_RUNTIME_ERROR:
        { printf("Runtime Error!\n"); } break;
        case WREN_RESULT_SUCCESS:
        { printf("Success!\n"); } break;
    }
    wrenFreeVM(vm);
}
WrenManager::~WrenManager(){}




