#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "invoke.hpp"
namespace inv = invoke_hpp;

namespace
{
    void simple_static_function() {
    }

    int simple_static_function_r() {
        return 42;
    }

    int simple_static_function_r_with_arg(int v) {
        return v;
    }

    const int& simple_static_function_r_with_ref_arg(const int& v) {
        return v;
    }

    class obj_t {
    public:
        int value = 42;
        const int value_c = 42;

        void member() {
        }

        int member_r() {
            return 42;
        }

        int member_r_with_arg(int v) {
            return v;
        }

        const int& member_r_with_ref_arg(const int& v) {
            return v;
        }
    };
}

TEST_CASE("invoke"){
    SECTION("invoke_functions"){
        inv::invoke(simple_static_function);
        REQUIRE(inv::invoke(simple_static_function_r) == 42);
        REQUIRE(inv::invoke(simple_static_function_r_with_arg, 42) == 42);
        {
            int v = 42;
            REQUIRE(&inv::invoke(simple_static_function_r_with_ref_arg, v) == &v);
        }
    }
    SECTION("invoke_members"){
        obj_t o;

        inv::invoke(&obj_t::member, o);
        inv::invoke(&obj_t::member, &o);
        inv::invoke(&obj_t::member, std::ref(o));

        REQUIRE(inv::invoke(&obj_t::member_r, o) == 42);
        REQUIRE(inv::invoke(&obj_t::member_r, &o) == 42);
        REQUIRE(inv::invoke(&obj_t::member_r, std::ref(o)) == 42);

        REQUIRE(inv::invoke(&obj_t::member_r_with_arg, o, 42) == 42);
        REQUIRE(inv::invoke(&obj_t::member_r_with_arg, &o, 42) == 42);
        REQUIRE(inv::invoke(&obj_t::member_r_with_arg, std::ref(o), 42) == 42);

        {
            int v = 42;
            REQUIRE(&inv::invoke(&obj_t::member_r_with_ref_arg, o, std::ref(v)) == &v);
            REQUIRE(&inv::invoke(&obj_t::member_r_with_ref_arg, &o, std::ref(v)) == &v);
            REQUIRE(&inv::invoke(&obj_t::member_r_with_ref_arg, std::ref(o), std::ref(v)) == &v);
        }
    }
    SECTION("invoke_member_objects"){
        obj_t o;

        REQUIRE(inv::invoke(&obj_t::value, o) == 42);
        REQUIRE(inv::invoke(&obj_t::value, &o) == 42);
        REQUIRE(inv::invoke(&obj_t::value, std::ref(o)) == 42);

        REQUIRE(inv::invoke(&obj_t::value_c, o) == 42);
        REQUIRE(inv::invoke(&obj_t::value_c, &o) == 42);
        REQUIRE(inv::invoke(&obj_t::value_c, std::ref(o)) == 42);
    }
}

TEST_CASE("invoke_result"){
    SECTION("invoke_result_functions"){
        static_assert(
            std::is_same<
                void,
                inv::invoke_result_t<decltype(simple_static_function)>>::value,
            "unit test fail");
        static_assert(
            std::is_same<
                int,
                inv::invoke_result_t<decltype(simple_static_function_r)>>::value,
            "unit test fail");
    }
    SECTION("invoke_result_members"){
        static_assert(
            std::is_same<void,
                inv::invoke_result_t<decltype(&obj_t::member), obj_t>>::value,
            "unit test fail");
        static_assert(
            std::is_same<void,
                inv::invoke_result_t<decltype(&obj_t::member), obj_t*>>::value,
            "unit test fail");
        static_assert(
            std::is_same<void,
                inv::invoke_result_t<decltype(&obj_t::member), std::reference_wrapper<obj_t>>>::value,
            "unit test fail");

        static_assert(
            std::is_same<int,
                inv::invoke_result_t<decltype(&obj_t::member_r), obj_t>>::value,
            "unit test fail");
        static_assert(
            std::is_same<int,
                inv::invoke_result_t<decltype(&obj_t::member_r), obj_t*>>::value,
            "unit test fail");
        static_assert(
            std::is_same<int,
                inv::invoke_result_t<decltype(&obj_t::member_r), std::reference_wrapper<obj_t>>>::value,
            "unit test fail");

        static_assert(
            std::is_same<int,
                inv::invoke_result_t<decltype(&obj_t::member_r_with_arg), obj_t, int>>::value,
            "unit test fail");
        static_assert(
            std::is_same<int,
                inv::invoke_result_t<decltype(&obj_t::member_r_with_arg), obj_t*, int>>::value,
            "unit test fail");
        static_assert(
            std::is_same<int,
                inv::invoke_result_t<decltype(&obj_t::member_r_with_arg), std::reference_wrapper<obj_t>, int>>::value,
            "unit test fail");
    }
}

TEST_CASE("apply"){
    SECTION("apply_functions"){
        inv::apply(simple_static_function, std::make_tuple());
        REQUIRE(inv::apply(simple_static_function_r, std::make_tuple()) == 42);
        REQUIRE(inv::apply(simple_static_function_r_with_arg, std::make_tuple(42)) == 42);
        {
            int v = 42;
            REQUIRE(&inv::apply(simple_static_function_r_with_ref_arg, std::make_tuple(std::ref(v))) == &v);
        }
    }
    SECTION("apply_members"){
        obj_t o;

        inv::apply(&obj_t::member, std::make_tuple(o));
        inv::apply(&obj_t::member, std::make_tuple(&o));
        inv::apply(&obj_t::member, std::make_tuple(std::ref(o)));

        REQUIRE(inv::apply(&obj_t::member_r, std::make_tuple(o)) == 42);
        REQUIRE(inv::apply(&obj_t::member_r, std::make_tuple(&o)) == 42);
        REQUIRE(inv::apply(&obj_t::member_r, std::make_tuple(std::ref(o))) == 42);

        REQUIRE(inv::apply(&obj_t::member_r_with_arg, std::make_tuple(o, 42)) == 42);
        REQUIRE(inv::apply(&obj_t::member_r_with_arg, std::make_tuple(&o, 42)) == 42);
        REQUIRE(inv::apply(&obj_t::member_r_with_arg, std::make_tuple(std::ref(o), 42)) == 42);

        {
            int v = 42;
            REQUIRE(&inv::apply(&obj_t::member_r_with_ref_arg, std::make_tuple(o, std::ref(v))) == &v);
            REQUIRE(&inv::apply(&obj_t::member_r_with_ref_arg, std::make_tuple(&o, std::ref(v))) == &v);
            REQUIRE(&inv::apply(&obj_t::member_r_with_ref_arg, std::make_tuple(std::ref(o), std::ref(v))) == &v);
        }
    }
    SECTION("apply_member_objects"){
        obj_t o;

        REQUIRE(inv::apply(&obj_t::value, std::make_tuple(o)) == 42);
        REQUIRE(inv::apply(&obj_t::value, std::make_tuple(&o)) == 42);
        REQUIRE(inv::apply(&obj_t::value, std::make_tuple(std::ref(o))) == 42);

        REQUIRE(inv::apply(&obj_t::value_c, std::make_tuple(o)) == 42);
        REQUIRE(inv::apply(&obj_t::value_c, std::make_tuple(&o)) == 42);
        REQUIRE(inv::apply(&obj_t::value_c, std::make_tuple(std::ref(o))) == 42);
    }
}