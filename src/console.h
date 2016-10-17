#ifndef console_h
#define consele_h

enum FontStyle {
  Green = 1,
  Red = 2,
  White = 4,
  Bold = 8,
  Normal = 16,
  Check = 32,
  Cross = 64
};

void console(const char*, int = (White | Normal));
void consoleln(const char* = "", int = (White | Normal));
void consoleSpliter(char);

#endif
