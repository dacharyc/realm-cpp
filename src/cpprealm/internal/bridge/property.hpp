#ifndef CPP_REALM_BRIDGE_PROPERTY_HPP
#define CPP_REALM_BRIDGE_PROPERTY_HPP

#include <functional>
#include <string>
#include <cpprealm/internal/bridge/utils.hpp>

namespace realm {
    struct Property;
}

namespace realm::internal::bridge {
    struct col_key;

    struct property {
        enum class type {
            Int = 0,
            Bool = 1,
            String = 2,
            Data = 3,
            Date = 4,
            Float = 5,
            Double = 6,
            Object = 7,         // currently must be either Array xor Nullable
            LinkingObjects = 8, // currently must be Array and not Nullable

            Mixed = 9,
            ObjectId = 10,
            Decimal = 11,
            UUID = 12,

            // Flags which can be combined with any of the above types except as noted
            Required = 0,
            Nullable = 64,
            Array = 128,
            Set = 256,
            Dictionary = 512,

            Collection = Array | Set | Dictionary,
            Flags = Nullable | Collection
        };

        property();
        property(const property& other) ;
        property& operator=(const property& other);
        property(property&& other) noexcept;
        property &operator=(property &&other) noexcept;
        ~property();
        property(const Property&); //NOLINT(google-explicit-constructor)
        property(const std::string& name,
                 type type,
                 bool is_primary_key);
        property(const std::string& name,
                 type type,
                 const std::string& object_name);
        operator Property() const; //NOLINT(google-explicit-constructor)
        void set_object_link(const std::string&);
        void set_origin_property_name(const std::string&);
        void set_type(type);
//        void set_is_computed(bool);
        void set_primary_key(bool);
        bool is_primary() const;
        std::string name() const;
        [[nodiscard]] col_key column_key() const;
    private:
#ifdef CPPREALM_HAVE_GENERATED_BRIDGE_TYPES
        storage::Property m_property[1];
#else
        std::shared_ptr<Property> m_property;
#endif
    };

    namespace {
        template <typename E>
        constexpr auto to_underlying(E e)
        {
            return static_cast<typename std::underlying_type<E>::type>(e);
        }
    }
    inline constexpr property::type operator|(property::type a, property::type b)
    {
        return static_cast<property::type>(to_underlying(a) | to_underlying(b));
    }
    inline constexpr property::type operator&(property::type a, property::type b)
    {
        return static_cast<property::type>(to_underlying(a) & to_underlying(b));
    }
}

#endif //CPP_REALM_BRIDGE_PROPERTY_HPP
