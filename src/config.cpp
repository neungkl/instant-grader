#include "config.h"
#include "util.h"
#include <string.h>
#include <map>

using namespace std;

bool stringComp(char* a, char* b) {
  return strcmp(a,b) == 0;
}

map<ConfigProperty, char*> config;
map<ConfigProperty, int> configI;
map<string, string> compileCmd;
map<string, string> runCmd;

void setConf(ConfigProperty key, char* value) {
  config[key] = value;
}

void setConf(ConfigProperty key, int value) {
  configI[key] = value;
}

int confi(ConfigProperty key) {
  auto obj = configI.find(key);
  if(obj == configI.end()) return -1;
  return obj->second;
}

char* conf(ConfigProperty key) {
  auto obj = config.find(key);
  if(obj == config.end()) return "";
  return obj->second;
}

void setCompileCmd(char* key, char* value) {
  compileCmd[string(key)] = string(value);
}

char* getCompileCmd(char* key) {
  map<string,string>::iterator obj = compileCmd.find(string(key));
  if(obj == compileCmd.end()) {
    return getCompileCmd("default");
  }

  return (char*) obj->second.c_str();
}

void setRunCmd(char* key, char* value) {
  runCmd[string(key)] = string(value);
}

char* getRunCmd(char* key) {
  map<string,string>::iterator obj = runCmd.find(string(key));
  if(obj == runCmd.end()) {
    return getRunCmd("default");
  }

  return (char*) obj->second.c_str();
}

/*
  To define language to compiler depends on extension of file
  @param ext extension of file
*/
void setLanguage(char* ext) {

  if(conf(LANGUAGE) != NULL && strcmp(conf(LANGUAGE), "custom") == 0) return ;

  if(strcmp(ext, "c") == 0) {
    setConf(LANGUAGE, "c");
  } else if(strcmp(ext, "cpp") == 0) {
    setConf(LANGUAGE, "c++");
  } else if(strcmp(ext, "java") == 0) {
    setConf(LANGUAGE, "java");
  } else if(strcmp(ext, "py") == 0) {
    setConf(LANGUAGE, "python");
  } else if(strcmp(ext, "hs") == 0) {
    setConf(LANGUAGE, "haskell");
  } else if(strcmp(ext, "rb") == 0) {
    setConf(LANGUAGE, "ruby");
  } else {
    // Use C as defailt language
    setConf(LANGUAGE, "c");
  }
}
