#define SQR(x)             ((x)*(x))
#define LENGTH(arr)        (sizeof(arr)/sizeof(arr[0]))
#define CLAMP(x, min, max) ((x < min ? min : (x > max ? max : x)))

void die(const char *fmt, ...);
char *extension(char *path);
