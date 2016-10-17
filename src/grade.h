#ifndef grade_h
#define grade_h

enum GradeStatus {
  Pass,
  Reject,
  FileNotFound,
  CompileFailed
};

GradeStatus gradeFile(char*);

#endif
