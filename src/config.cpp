#include "config.h"
#include <map>

using namespace std;

map<ConfigProperty, char*> config;

void setConf(ConfigProperty key, char* value) {
  config[key] = value;
}

char* conf(ConfigProperty key) {
  auto obj = config.find(key);
  if(obj == config.end()) return NULL;
  return obj->second;
}
