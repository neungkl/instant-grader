#ifndef config_h
#define config_h

enum ConfigProperty {
  TEST_INPUT_DELIMITER,
  TEST_OUTPUT_DELIMITER,
  LANGUAGE,
  CLI_VERBOSE,
  COMPILER_CPP,
  COMPILER_C,
  COMPILER_JAVA
};

void setConf(ConfigProperty, char*);
void setConf(ConfigProperty, int);
int   confi(ConfigProperty);
char* conf(ConfigProperty);

void setLanguage(char*);

void  setCompileCmd(char*, char*);
char* getCompileCmd(char*);

void setRunCmd(char*, char*);
char* getRunCmd(char*);

#endif
