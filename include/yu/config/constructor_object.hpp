#define YU_CONSTRUCTOR_OBJECT(...)                                                                     \
    []<typename... Args>(Args&&... args) noexcept(noexcept(__VA_ARGS__{static_cast<Args&&>(args)...})) \
        requires requires { __VA_ARGS__{static_cast<Args&&>(args)...}; }                               \
    { return __VA_ARGS__{static_cast<Args&&>(args)...}; }
