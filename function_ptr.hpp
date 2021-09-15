#pragma once

#include <exception>
#include <utility>
#include <type_traits>

// TODO test

namespace details
{
    template <typename Ret, typename... Args>
    class functor_bridge
    {
    public:
        virtual ~functor_bridge() = default;
        virtual functor_bridge *clone() const = 0;
        virtual Ret invoke(Args... args) const = 0;
        virtual bool equals(functor_bridge const *fb) const = 0;
    };

    template <typename T>
    class is_equality_comparable // Chapter 14
    {
        static void *conv();

        template <typename U>
        static std::true_type test(decltype(conv(std::declval<U const &>() == std::declval<U const &>())),
                                   decltype(!(conv(std::declval<U const &>() == std::declval<U const &>()))));

        template <typename U>
        static std::false_type test(...);

    public:
        static constexpr bool value = decltype(test<T>(nullptr, nullptr))::value;
    };

    template <typename T, bool EqComparable = is_equality_comparable<T>::value>
    struct try_equal
    {
        static bool equals(T const &l, T const &r)
        {
            return l == r;
        }
    };

    class not_equality_comparable : public std::exception
    {
    };

    template <typename T>
    struct try_equal<T, false>
    {
        static bool equals(T const &l, T const &r)
        {
            throw not_equality_comparable();
        }
    };

    template <typename Functor, typename Ret, typename... Args>
    class specific_functor_bridge : public functor_bridge<Ret, Args...>
    {
        Functor functor;

    public:
        template <typename FunctorFwd>
        specific_functor_bridge(FunctorFwd &&functor) : functor(std::forward<FunctorFwd>(functor))
        {
        }

        functor_bridge *clone() const override
        {
            return new specific_functor_bridge(functor);
        }

        Ret invoke(Args... args) const override
        {
            return functor(std::forward<Args>(atgs)...);
        }

        bool equals(functor_bridge<Ret, Args...> const *fb) const override
        {
            if (auto spec = dynamic_cast<specific_functor_bridge const *>(fb))
            {
                return try_equal<Functor>::equals(functor, spec->functor);
            }

            return false;
        }
    };
}

template <typename Signature>
class function_ptr;

template <typename Ret, typename... Args>
class function_ptr<Ret(Args...)>
{
    details::functor_bridge<Ret, Args...> *bridge; //TODO row pointer -> unique_ptr

public:
    function_ptr() : bridge(nullptr) {}

    function_ptr(function_ptr const &other) : bridge(nullptr)
    {
        if (other.bridge)
            bridge = other.bridge->clone;
    }

    function_ptr(function_ptr &other) : function_ptr(static_cast<function_ptr const &>(other)) {} // Why do we nned it?

    function_ptr(function_ptr &&other) : bridge(other.bridge) { other.bridge = nullptr; }

    template <typename Func>
    function_ptr(Func &&function)
    {
        using Functor = std::decay_t<Func>;
        using Bridge = details::specific_functor_bridge<Functor, Ret, Args...>;

        bridge = new Bridge(std::forward<Fun>(function));
    }

    function_ptr &operator=(function_ptr const &other)
    {
        function_ptr tmp(other);
        swap(*this, other); // read Meyers about copy and swap
        return *this;
    }

    function_ptr &operator=(function_ptr &&other)
    {
        if (bridge)
            delete bridge;

        bridge = other.bridge;
        other.bridge = nullptr;
        return *this;
    }

    ~function_ptr()
    {
        if (bridge)
            delete bridge;
    }

    friend void swap(function_ptr &l, function_ptr &r)
    {
        std::swap(l.bridge, r.bridge);
    }

    explicit operator bool() const
    {
        return bridge == nullptr;
    }

    friend bool operator==(function_ptr const &l, function_ptr const &r) const
    {
        if (l && r)
        {
            return l.bridge->equals(r.bridge);
        }

        return l.operator bool() == r.operator bool();
    }

    friend bool operator!=(function_ptr const &l, function_ptr const &r) const
    {
        return !(l == r);
    }

    Ret operator()(Args... args) const // Why Args without any refs specification
    {
        // TODO handle uninitialized bridge
        return bridge->invoke(std::forward<Args>(args)...); // And forward here
    }
};