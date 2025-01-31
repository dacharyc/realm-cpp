#include "../main.hpp"
#include "test_objects.hpp"
#include <cpprealm/internal/type_info.hpp>
using namespace realm;

template <size_t N, typename T>
void check_nulls(const T& obj) {
    if constexpr (N + 1 >= std::tuple_size<decltype(T::schema.properties)>{}) {
        return;
    } else {
        using Property = std::decay_t<decltype(std::get<N>(T::schema.properties))>;
        if constexpr (internal::type_info::is_optional<typename Property::Result>::value) {
            typename Property::Result val = *(obj.*Property::ptr);
            CHECK(val == std::nullopt);
        }
        return check_nulls<N + 1>(obj);
    }
}
TEST_CASE("optional") {
    realm_path path;
    db_config config;
    config.set_path(path);
    SECTION("unmanaged_managed_optional_get_set") {
        auto realm = realm::open<AllTypesObject, AllTypesObjectLink, AllTypesObjectEmbedded>(std::move(config));
        {
            auto obj = AllTypesObject();
            check_nulls<0>(obj);
            realm.write([&realm, &obj] {
                realm.add(obj);
            });
            check_nulls<0>(obj);
            realm.write([&realm, &obj] {
                realm.remove(obj);
            });
        }
        {
            auto obj = AllTypesObject();
            obj.opt_int_col = 42;
            obj.opt_str_col = "hello world";
            obj.opt_uuid_col = realm::uuid();
            obj.opt_enum_col = AllTypesObject::Enum::one;
            obj.opt_obj_col = AllTypesObjectLink{};

            realm.write([&realm, &obj] {
                realm.add(obj);
            });

            CHECK(obj.opt_int_col == static_cast<int64_t>(42));
            CHECK(obj.opt_str_col == "hello world");
            CHECK(obj.opt_uuid_col == realm::uuid());
            CHECK(obj.opt_enum_col == AllTypesObject::Enum::one);
            CHECK(*obj.opt_obj_col);

            realm.write([&obj] {
                obj.opt_int_col = std::nullopt;
                obj.opt_str_col = std::nullopt;
                obj.opt_uuid_col = std::nullopt;
                obj.opt_enum_col = std::nullopt;
                obj.opt_obj_col = std::nullopt;
            });
            check_nulls<0>(obj);
        }
    }
}
