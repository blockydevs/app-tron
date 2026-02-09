#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include "uint256.h"

static uint64_t u128_upper(const uint128_t *x) { return x->elements[0]; }
static uint64_t u128_lower(const uint128_t *x) { return x->elements[1]; }

static uint64_t u256_uu(const uint256_t *x) { return x->elements[0].elements[0]; }
static uint64_t u256_ul(const uint256_t *x) { return x->elements[0].elements[1]; }
static uint64_t u256_lu(const uint256_t *x) { return x->elements[1].elements[0]; }
static uint64_t u256_ll(const uint256_t *x) { return x->elements[1].elements[1]; }

static void u128_set_u64(uint128_t *x, uint64_t v) {
    x->elements[0] = 0;
    x->elements[1] = v;
}

static void u256_set_u64(uint256_t *x, uint64_t v) {
    x->elements[0].elements[0] = 0;
    x->elements[0].elements[1] = 0;
    x->elements[1].elements[0] = 0;
    x->elements[1].elements[1] = v;
}

static void assert_u128_eq_u64(const uint128_t *x, uint64_t v) {
    assert_int_equal(u128_upper(x), 0);
    assert_int_equal(u128_lower(x), v);
}

static void assert_u256_eq_u64(const uint256_t *x, uint64_t v) {
    assert_true(zero128((uint128_t *)&x->elements[0]));
    assert_int_equal(u256_lu(x), 0);
    assert_int_equal(u256_ll(x), v);
}

static void test_readu128_be(void **state) {
    (void) state;
    uint8_t buf[16] = {
        0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,
        0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10
    };
    uint128_t v;
    readu128BE(buf, &v);
    assert_int_equal(u128_upper(&v), 0x0102030405060708ULL);
    assert_int_equal(u128_lower(&v), 0x090A0B0C0D0E0F10ULL);
}

static void test_readu256_be(void **state) {
    (void) state;
    uint8_t buf[32] = {
        0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,
        0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,
        0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,
        0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,0x20
    };
    uint256_t v;
    readu256BE(buf, &v);

    assert_int_equal(u256_uu(&v), 0x0102030405060708ULL);
    assert_int_equal(u256_ul(&v), 0x090A0B0C0D0E0F10ULL);
    assert_int_equal(u256_lu(&v), 0x1112131415161718ULL);
    assert_int_equal(u256_ll(&v), 0x191A1B1C1D1E1F20ULL);
}

static void test_zero_bits_shift(void **state) {
    (void) state;

    uint256_t v;
    clear256(&v);
    assert_true(zero256(&v));
    assert_int_equal(bits256(&v), 0);

    u256_set_u64(&v, 1);
    assert_false(zero256(&v));
    assert_int_equal(bits256(&v), 1);

    uint256_t out;
    shiftl256(&v, 8, &out);
    assert_int_equal(bits256(&out), 9);

    shiftr256(&out, 8, &v);
    assert_int_equal(bits256(&v), 1);

    shiftl256(&v, 256, &out);
    assert_true(zero256(&out));
    shiftr256(&v, 256, &out);
    assert_true(zero256(&out));
}

static void test_add_minus_mul_divmod(void **state) {
    (void) state;

    uint256_t a, b, res, div, mod;
    u256_set_u64(&a, 3);
    u256_set_u64(&b, 5);

    add256(&a, &b, &res);
    assert_u256_eq_u64(&res, 8);

    minus256(&b, &a, &res);
    assert_u256_eq_u64(&res, 2);

    mul256(&a, &b, &res);
    assert_u256_eq_u64(&res, 15);

    u256_set_u64(&a, 20);
    u256_set_u64(&b, 6);
    divmod256(&a, &b, &div, &mod);
    assert_u256_eq_u64(&div, 3);
    assert_u256_eq_u64(&mod, 2);
}

static void test_compare_or(void **state) {
    (void) state;

    uint256_t a, b, res;
    u256_set_u64(&a, 5);
    u256_set_u64(&b, 7);

    assert_false(gt256(&a, &b));
    assert_true(gt256(&b, &a));
    assert_true(gte256(&b, &a));
    assert_true(gte256(&b, &b));
    assert_true(equal256(&b, &b));

    u256_set_u64(&a, 0xF0);
    u256_set_u64(&b, 0x0F);
    or256(&a, &b, &res);
    assert_u256_eq_u64(&res, 0xFF);
}

static void test_carry_add(void **state) {
    (void) state;

    uint256_t a, b, res;
    clear256(&a);
    clear256(&b);

    a.elements[1].elements[1] = UINT64_MAX;
    u256_set_u64(&b, 1);

    add256(&a, &b, &res);
    assert_int_equal(u256_ll(&res), 0);
    assert_int_equal(u256_lu(&res), 1);
}

static void test_tostring(void **state) {
    (void) state;

    char out[128];
    uint256_t v;

    u256_set_u64(&v, 0);
    assert_true(tostring256(&v, 10, out, sizeof(out)));
    assert_string_equal(out, "0");

    u256_set_u64(&v, 255);
    assert_true(tostring256(&v, 16, out, sizeof(out)));
    assert_string_equal(out, "ff");

    u256_set_u64(&v, 1);
    shiftl256(&v, 8, &v);  // 0x100
    assert_true(tostring256(&v, 16, out, sizeof(out)));
    assert_string_equal(out, "100");
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_readu128_be),
        cmocka_unit_test(test_readu256_be),
        cmocka_unit_test(test_zero_bits_shift),
        cmocka_unit_test(test_add_minus_mul_divmod),
        cmocka_unit_test(test_compare_or),
        cmocka_unit_test(test_carry_add),
        cmocka_unit_test(test_tostring),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}