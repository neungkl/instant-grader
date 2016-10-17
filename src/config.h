#ifndef config_h
#define config_h

enum ConfigProperty {
  TEST_INPUT_DELIMITER,
  TEST_OUTPUT_DELIMITER,
  LANGUAGE,
  COMPILER_CPP,
  COMPILER_C
};

void setConf(ConfigProperty, char*);
char* conf(ConfigProperty);

#endif
