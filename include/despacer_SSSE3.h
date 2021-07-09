#ifdef __SSSE3__

// credit: https://github.com/aqrit
size_t despace_ssse3_cumsum( void* dst_void, void* src_void, size_t length );

// credit: https://github.com/aqrit
size_t despace_ssse3_lut_1kb( void* dst_void, void* src_void, size_t length );

// credit: https://github.com/aqrit
size_t despace_ssse3_lut_512kb( void* dst_void, void* src_void, size_t length );

#endif
