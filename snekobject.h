typedef enum {
  INTEGER,
} snek_object_kind_t;

typedef union {
  int v_int;
} snek_object_data_t;

typedef struct {
  snek_object_kind_t kind;
  snek_object_data_t data;
} snek_object_t;
