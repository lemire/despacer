/**
* remove spaces (in-place) from string bytes (UTF-8 or ASCII) containing
* "howmany"
* characters (not counting the null character if a C string is used), returns
* the new number of characters
* this function does add the null character, you have to do it yourself if you
* need it.
*/
size_t despace (char * bytes, size_t howmany);

/// make the store unconditional
// credit: https://github.com/aqrit
inline size_t despace_branchless (void * dst_void, void * src_void, size_t length);

/// explicit branchless
// credit: https://github.com/aqrit
inline size_t despace_cmov (void * dst_void, void * src_void, size_t length);

// credit: https://github.com/aqrit
inline size_t despace_table (void * dst_void, void * src_void, size_t length);

inline size_t despace32 (char * bytes, size_t howmany);

inline size_t faster_despace (char * bytes, size_t howmany);

inline size_t faster_despace32 (char * bytes, size_t howmany);

inline size_t countspaces (char const * bytes, size_t howmany);

inline size_t countspaces32 (char const * bytes, size_t howmany);

inline size_t despace64 (char * bytes, size_t howmany);

/**
* remove spaces from string bytes (UTF-8 or ASCII) containing "howmany"
* characters (not counting the null character if a C string is used), returns
* the new number of characters
* this function does add the null character, you have to do it yourself if you
* need it.
* Result is stored in "out", where the caller is responsible for memory
* allocation.
*/
inline size_t despace_to (char const * bytes, size_t howmany, char * out);
