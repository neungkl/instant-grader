#include "config.h"
#include <map>

using namespace std;

map<char*, char*> config;

void setConf(char* key, char* value) {
  config[key] = value;
}

char* conf(char* key) {
  auto obj = config.find(key);
  if(obj == config.end()) return NULL;
  return obj->second;
}
