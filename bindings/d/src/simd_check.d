module despacer.simd_check;

/** Check for simd in general  */
static bool version_d_simd()
{
  version (D_SIMD)
  {
    return true;
  }
  else
  {
    return false;
  }
}

/** Check for sse3  */
static bool supports_sse3()
{
  version (LDC)
  {
    return __traits(targetHasFeature, "sse3");
  }
  else
  {
    return version_d_simd();
  }
}

/** Check for sse4_1  */
static bool supports_sse4_1()
{
  version (LDC)
  {
    return __traits(targetHasFeature, "sse4.1");
  }
  else
  {
    return version_d_simd();
  }
}

/** Check for sse4_2  */
static bool supports_sse4_2()
{
  version (LDC)
  {
    return __traits(targetHasFeature, "sse4.2");
  }
  else
  {
    return version_d_simd();
  }
}

/** Check for avx2  */
static bool supports_avx2()
{
  version (LDC)
  {
    return __traits(targetHasFeature, "avx2");
  }
  else
  {
    version (D_AVX2)
    {
      return true;
    }
    else
    {
      return false;
    }
  }
}
