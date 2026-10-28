#if not defined(YU_CONFIG_DISABLE_STATIC_CALL_OPERATOR)
#  if defined(__cpp_static_call_operator) && __cpp_static_call_operator >= 202207l
#    define YU_STATELESS_CALL_OPERATOR(...) static operator()(__VA_ARGS__)
#  endif
#endif

#if not defined(YU_STATELESS_CALL_OPERATOR)
#  define YU_STATELESS_CALL_OPERATOR(...) operator()(__VA_ARGS__) const
#endif