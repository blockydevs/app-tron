#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <string.h>
#include <cmocka.h>
#include <sys/types.h>
#include "helpers.h"
#include "parse.h"
#include "app_errors.h"

// sprawdzamy tylko logikę formatowania
static void test_adjust_decimals(void **state) {
    (void) state;

    char out[64];

    // 0 -> "0"
    assert_true(adjustDecimals("0", 1, out, sizeof(out), 6));
    assert_string_equal(out, "0");

    // 1 przy 6 miejscach -> "0.000001"
    assert_true(adjustDecimals("1", 1, out, sizeof(out), 6));
    assert_string_equal(out, "0.000001");

    // 42 przy 6 miejscach -> "0.000042"
    assert_true(adjustDecimals("42", 2, out, sizeof(out), 6));
    assert_string_equal(out, "0.000042");

    // 1234567 przy 6 miejscach -> "1.234567"
    assert_true(adjustDecimals("1234567", 7, out, sizeof(out), 6));
    assert_string_equal(out, "1.234567");

    // brak miejsca w buforze
    assert_false(adjustDecimals("1234567", 7, out, 4, 6));
}

static void test_print_amount(void **state) {
    (void) state;

    char out[64];

    // 0 -> "0.000000"
    print_amount(0, out, sizeof(out), SUN_DIG);
    assert_string_equal(out, "0.000000");

    // 1 SUN -> "0.000001"
    print_amount(1, out, sizeof(out), SUN_DIG);
    assert_string_equal(out, "0.000001");

    // 1 TRX -> "1"
    print_amount(1000000, out, sizeof(out), SUN_DIG);
    assert_string_equal(out, "1");

    // 42.5 TRX -> "42.5"
    print_amount(42500000, out, sizeof(out), SUN_DIG);
    assert_string_equal(out, "42.5");
}

static void test_set_contract_type(void **state) {
    (void) state;

    char out[64];

    assert_true(setContractType(ACCOUNTUPDATECONTRACT, out, sizeof(out)));
    assert_string_equal(out, "Account Update");

    assert_true(setContractType(UNFREEZEBALANCECONTRACT, out, sizeof(out)));
    assert_string_equal(out, "Unfreeze Balance");

    assert_true(setContractType(UNKNOWN_CONTRACT, out, sizeof(out)));
    assert_string_equal(out, "Unknown Type");

    // nieznany enum -> false
    assert_false(setContractType((contractType_e)255, out, sizeof(out)));
}

static void test_bytes_to_string(void **state) {
    (void) state;

    char out[64];
    uint8_t data[4] = {0xDE, 0xAD, 0xBE, 0xEF};

    assert_int_equal(bytes_to_string(out, sizeof(out), data, sizeof(data)), 0);
    assert_string_equal(out, "0xdeadbeef");

    // zbyt mały bufor
    assert_int_equal(bytes_to_string(out, 2, data, sizeof(data)), -1);
}

static void write_u32_be(uint8_t *dst, uint32_t v) {
    dst[0] = (uint8_t)((v >> 24) & 0xFF);
    dst[1] = (uint8_t)((v >> 16) & 0xFF);
    dst[2] = (uint8_t)((v >> 8) & 0xFF);
    dst[3] = (uint8_t)(v & 0xFF);
}

static void test_read_bip32_path_ok(void **state) {
    (void) state;

    uint8_t buf[1 + 3 * 4];
    buf[0] = 3;  // length
    write_u32_be(&buf[1], 44 | 0x80000000);
    write_u32_be(&buf[5], 195);
    write_u32_be(&buf[9], 0);

    bip32_path_t path;
    off_t off = read_bip32_path(buf, sizeof(buf), &path);

    assert_int_equal(off, 1 + 3 * 4);
    assert_int_equal(path.length, 3);
    assert_int_equal(path.indices[0], 44 | 0x80000000);
    assert_int_equal(path.indices[1], 195);
    assert_int_equal(path.indices[2], 0);
}

static void test_read_bip32_path_errors(void **state) {
    (void) state;

    bip32_path_t path;

    uint8_t buf1[] = {1};
    assert_int_equal(read_bip32_path(buf1, sizeof(buf1), &path), -1);

    uint8_t buf2[] = {0};
    assert_int_equal(read_bip32_path(buf2, sizeof(buf2), &path), -1);

    uint8_t buf3[] = {MAX_BIP32_PATH + 1};
    assert_int_equal(read_bip32_path(buf3, sizeof(buf3), &path), -1);

    uint8_t buf4[] = {2, 0, 0, 0, 1};
    assert_int_equal(read_bip32_path(buf4, sizeof(buf4), &path), -1);
}

static void test_bytes_to_string_edge(void **state) {
    (void) state;

    char out[8];
    uint8_t data[1] = {0xAB};

    assert_int_equal(bytes_to_string(out, sizeof(out), data, 1), 0);
    assert_string_equal(out, "0xab");

    assert_int_equal(bytes_to_string(out, 2, data, 1), -1);
}

static void test_exchange_contract_detail(void **state) {
    (void) state;

    char out[32];

    assert_true(setExchangeContractDetail(EXCHANGECREATECONTRACT, out, sizeof(out)));
    assert_string_equal(out, "create");

    assert_true(setExchangeContractDetail(EXCHANGEINJECTCONTRACT, out, sizeof(out)));
    assert_string_equal(out, "inject");

    assert_true(setExchangeContractDetail(EXCHANGEWITHDRAWCONTRACT, out, sizeof(out)));
    assert_string_equal(out, "withdraw");

    assert_true(setExchangeContractDetail(EXCHANGETRANSACTIONCONTRACT, out, sizeof(out)));
    assert_string_equal(out, "transaction");

    assert_false(setExchangeContractDetail(TRANSFERCONTRACT, out, sizeof(out)));
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_adjust_decimals),
        cmocka_unit_test(test_print_amount),
        cmocka_unit_test(test_set_contract_type),
        cmocka_unit_test(test_bytes_to_string),
        cmocka_unit_test(test_read_bip32_path_ok),
        cmocka_unit_test(test_read_bip32_path_errors),
        cmocka_unit_test(test_bytes_to_string_edge),
        cmocka_unit_test(test_exchange_contract_detail),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}