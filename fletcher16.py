#!/usr/bin/python3
# -*- coding: utf-8 -*-


class Fletcher16Checksum:
    def __init__(self, value: bytearray | bytes | int = None):
        if isinstance(value, bytearray) or isinstance(value, bytes):
            mod256 = value[0]
            sum_of_sums = value[1]
        elif isinstance(value, bytes):
            mod256 = (value % 0xFF) if value else 0
            sum_of_sums = ((value >> 8) % 0xFF) if value else 0
        else:
            mod256 = sum_of_sums = 0

        self.mod256 = mod256
        self.sum_of_sums = sum_of_sums

    @property
    def checksum(self):
        # Local_u16Checksum = (Local_u16SumOfSums << 8) | Local_u16Mod256Sum
        return bytearray([self.mod256, self.sum_of_sums])

    @property
    def check_bytes(self):
        # Local_u16CheckBytesLSB = 0xFF - ((Local_u16ChecksumLSB + Local_u16ChecksumMSB) % 0xFF);
        # Local_u16CheckBytesMSB = 0xFF - ((Local_u16ChecksumLSB + Local_u16CheckBytesLSB) % 0xFF);
        check_lsb = 0xFF - (self.mod256 + self.sum_of_sums) % 0xFF
        check_msb = 0xFF - (self.mod256 + check_lsb) % 0xFF
        return bytearray([check_lsb, check_msb])

    def calculate(self, data: bytearray):
        for data_byte in data:
            self.mod256 = (self.mod256 + data_byte) % 0xFF
            self.sum_of_sums = (self.sum_of_sums + self.mod256) % 0xFF

        return self.checksum

    def reset(self):
        self.mod256 = 0
        self.sum_of_sums = 0

    @staticmethod
    def validate(data: bytearray) -> bool:
        checksum = Fletcher16Checksum()
        return checksum.calculate(data) == bytearray([0, 0])


if __name__ == "__main__":
    # static TestVector_t Test_asVectors [] = {
    #     {
    #         .data = "",
    #         .len = 1,
    #         .expected_checksum = 0x00
    #     },
    #     {
    #         .data = "abcde",
    #         .len = 5,
    #         .expected_checksum = 0xC8F0
    #     },
    #     {
    #         .data = "abcdef",
    #         .len = 6,
    #         .expected_checksum = 0x2057
    #     },
    #     {
    #         .data = "abcdefgh",
    #         .len = 8,
    #         .expected_checksum = 0x0627
    #     },
    # };
    from dataclasses import dataclass

    @dataclass
    class TestVector:
        __test__ = False

        def __init__(self, data: bytearray, checksum: bytearray):
            self.data = data
            self.checksum = checksum

    test_vectors = [
        TestVector([0], bytearray([0, 0])),
        TestVector(bytearray("abcde", encoding="utf-8"), bytearray([0xF0, 0xC8])),
        TestVector(bytearray("abcdef", encoding="utf-8"), bytearray([0x57, 0x20])),
        TestVector(bytearray("abcdefgh", encoding="utf-8"), bytearray([0x27, 0x06])),
    ]

    def test_fletcher16():
        for test_vector in test_vectors:
            # test data checksum
            checksum = Fletcher16Checksum()
            actual = checksum.calculate(test_vector.data)
            expected = test_vector.checksum
            assert actual == expected, f"expected {expected} but was {actual}"

            # test check bytes
            check_bytes = checksum.check_bytes
            signed_data = test_vector.data[:]
            signed_data.extend(check_bytes)
            checksum.reset()
            remainder = checksum.calculate(signed_data)
            assert remainder == bytearray(2)
