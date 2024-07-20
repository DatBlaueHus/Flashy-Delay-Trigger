#ifndef DEBUG_PRINT_HPP
#define DEBUG_PRINT_HPP

// Uncomment the following line to enable debug prints
//#define DEBUG_PRINT

#ifdef DEBUG_PRINT
  #define PRINT(...) debugPrintlnWrapper(__VA_ARGS__)

  // Base case: single argument, no formatting
  template<typename T>
  void debugPrintlnWrapper(T value) {
    Serial.println(value);
  }

  // Specialized case: handling formatting for integers and floats
  void debugPrintlnWrapper(int value, int format);
  void debugPrintlnWrapper(unsigned int value, int format);
  void debugPrintlnWrapper(long value, int format);
  void debugPrintlnWrapper(unsigned long value, int format);
  void debugPrintlnWrapper(float value, int format);
  void debugPrintlnWrapper(double value, int format);

  // Variadic template function to handle multiple arguments
  template<typename T, typename... Args>
  void debugPrintlnWrapper(T value, Args... args) {
    Serial.print(value);
    Serial.print(' ');
    debugPrintlnWrapper(args...);
  }
#else
  #define PRINT(...) // Do nothing
#endif

#ifdef DEBUG_PRINT
// Implementation of specialized functions

void debugPrintlnWrapper(int value, int format) {
  Serial.println(value, format);
}

void debugPrintlnWrapper(unsigned int value, int format) {
  Serial.println(value, format);
}

void debugPrintlnWrapper(long value, int format) {
  Serial.println(value, format);
}

void debugPrintlnWrapper(unsigned long value, int format) {
  Serial.println(value, format);
}

void debugPrintlnWrapper(float value, int format) {
  Serial.println(value, format);
}

void debugPrintlnWrapper(double value, int format) {
  Serial.println(value, format);
}

#endif // DEBUG_PRINT

#endif // DEBUG_PRINT_HPP
