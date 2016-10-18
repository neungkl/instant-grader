#ifndef grade_h
#define grade_h

enum GradeStatus {
  Pass,
  Reject,
  FileNotFound,
  CompileFailed,
  InputIsNotFile,
  Error
};

GradeStatus gradeFile(char*);

#endif
