#ifndef PROS_STRONGALIAS_HPP
#define PROS_STRONGALIAS_HPP

template<typename BaseType, typename TypeName>
struct StrongAlias {
    explicit constexpr StrongAlias(BaseType const& value) : value(value) {}
    explicit operator BaseType&() noexcept { return value; }
    constexpr explicit operator BaseType const&() const noexcept { return value; }

    BaseType value;
};

#define STRONG_ALIAS(BASE_TYPE, TYPE_NAME)                   \
struct TYPE_NAME : StrongAlias<BASE_TYPE, TYPE_NAME>  \
{                                                            \
    using StrongAlias::StrongAlias;                        \
};

#endif
