#define PRINT_TOKEN(token) printf(#token " is %d\n", token)
#define DEFINE_STRUCT(type) \
  struct type##_st { \
    int a; \
  };
