#ifndef console_h
#define consele_h

enum FontStyle {
  Green = 1,
  Red = 2,
  White = 4,
  Bold = 8,
  Normal = 16
};

void console(char*, int = (White | Normal));

#endif
