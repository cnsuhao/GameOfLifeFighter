#ifndef GAMEOFLIFEFIGHTERTRACE_H
#define GAMEOFLIFEFIGHTERTRACE_H

#include <string>
#include <vector>

#ifdef NDEBUG
  #define START_TRACE() ((void)0)
  #define TRACE(x) ((void)0)
#else
  #include <fstream>
  #include <iostream>
  #include <stdexcept>

  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Weffc++"
  #pragma GCC diagnostic ignored "-Wunused-local-typedefs"
  //#include <boost/lexical_cast.hpp>
  #pragma GCC diagnostic pop

  #define START_TRACE()        \
  {                            \
    std::ofstream f_trace1496( \
    "trace_out.txt");          \
    f_trace1496.close();       \
  }                            \


  #define TRACE(x)             \
  {                            \
    std::ofstream f_314_xyz(   \
      "trace_out.txt",         \
      std::ios::app);          \
    f_314_xyz                  \
      << "TRACE \'"            \
      <<  std::string(#x )     \
      <<  "\' line "           \
      <<  (__LINE__)           \
      <<  " in file '"         \
      <<  __FILE__             \
      <<  "': '"               \
      <<  boost::lexical_cast< \
          std::string>(x)      \
      <<  "'"                  \
      << std::endl;            \
    f_314_xyz.close();         \
    std::clog                  \
      << "TRACE \'"            \
      <<  std::string (#x)     \
      <<  "\' line "           \
      <<  (__LINE__)           \
      <<  " in file '"         \
      <<  __FILE__             \
      <<  "': '"               \
      <<  boost::lexical_cast< \
          std::string>(x)      \
      <<  "'"                  \
      << std::endl;            \
  }                            \

#endif

#endif // GAMEOFLIFEFIGHTERTRACE_H
