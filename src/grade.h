#ifndef grade_h
#define grade_h

enum GraderStatus {
  Pass,
  FileNotFound,
  CompileFailed,
  InputIsNotFile,
  Error
};

GraderStatus gradeFile(char*);

#endif
