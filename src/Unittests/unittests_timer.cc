
#include <gtest/gtest.h>
#include <OpenMesh/Tools/Utils/Timer.hh>


TEST(Timer, AsString_SecondsFormat) {
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(0.0, OpenMesh::Utils::Timer::Seconds), "0.000s");
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(1.0, OpenMesh::Utils::Timer::Seconds), "1.000s");
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(123.456, OpenMesh::Utils::Timer::Seconds), "123.456s");
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(-5.0, OpenMesh::Utils::Timer::Seconds), "-5.000s");
}

TEST(Timer, AsString_HundredthsFormat) {
    // HSeconds = hundredths of seconds (cs)
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(1.0, OpenMesh::Utils::Timer::HSeconds), "100.000cs");
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(0.5, OpenMesh::Utils::Timer::HSeconds), "50.000cs");
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(-2.345, OpenMesh::Utils::Timer::HSeconds), "-234.500cs");
}

TEST(Timer, AsString_MillisecondsFormat) {
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(1.0, OpenMesh::Utils::Timer::MSeconds), "1000.000ms");
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(0.5, OpenMesh::Utils::Timer::MSeconds), "500.000ms");
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(0.001, OpenMesh::Utils::Timer::MSeconds), "1.000ms");
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(-3.5, OpenMesh::Utils::Timer::MSeconds), "-3500.000ms");
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(999999999.0, OpenMesh::Utils::Timer::MSeconds), "999999999000.000ms");
}

TEST(Timer, AsString_MicrosecondsFormat) {
    // MicroSeconds uses \xb5 (single-byte micro sign) — format "%.1f\xb5s"
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(1.0, OpenMesh::Utils::Timer::MicroSeconds), "1000000.0\xb5s");
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(0.001, OpenMesh::Utils::Timer::MicroSeconds), "1000.0\xb5s");
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(-2.5, OpenMesh::Utils::Timer::MicroSeconds), "-2500000.0\xb5s");
}

TEST(Timer, AsString_NanosecondsFormat) {
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(1.0, OpenMesh::Utils::Timer::NanoSeconds), "1000000000.0ns");
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(0.5, OpenMesh::Utils::Timer::NanoSeconds), "500000000.0ns");
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(-3.14159, OpenMesh::Utils::Timer::NanoSeconds), "-3141590000.0ns");
}

TEST(Timer, AsString_HoursFormat) {
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string( 3605.0, OpenMesh::Utils::Timer::Hours),  "01h:00m:05s");
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string( 7261.5, OpenMesh::Utils::Timer::Hours),  "02h:01m:01s");
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(    0.0, OpenMesh::Utils::Timer::Hours),  "00h:00m:00s");
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(-3661.0, OpenMesh::Utils::Timer::Hours), "-01h:01m:01s");
}

TEST(Timer, AsString_MinutesFormat) {
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string( 125.0, OpenMesh::Utils::Timer::Minutes), "02m:05.00s");
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(  65.0, OpenMesh::Utils::Timer::Minutes), "01m:05.00s");
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(3600.0, OpenMesh::Utils::Timer::Minutes), "00m:00.00s"); // TODO: is this behavior desired?
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string( -65.0, OpenMesh::Utils::Timer::Minutes), "-01m:05.00s");
}

TEST(Timer, AsString_LongFormat) {
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string( 3661.5, OpenMesh::Utils::Timer::Long),  "01h:01m:01.500000000000s");
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(    0.0, OpenMesh::Utils::Timer::Long),  "00h:00m:00.000000000000s");
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(-3661.5, OpenMesh::Utils::Timer::Long), "-01h:01m:01.500000000000s");
}

TEST(Timer, AsString_Hours) {
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(45.0, OpenMesh::Utils::Timer::Hours), "00h:00m:45s");
}
TEST(Timer, AsString_Automatic) {
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(45.0, OpenMesh::Utils::Timer::Automatic), "45.000s");
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(120.0, OpenMesh::Utils::Timer::Automatic), "02m:00s");
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(3601.0, OpenMesh::Utils::Timer::Automatic), "01h:00m:01s");
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(0.5, OpenMesh::Utils::Timer::Automatic), "50.000cs");
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(0.001, OpenMesh::Utils::Timer::Automatic), "1.000ms");
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(0.000001, OpenMesh::Utils::Timer::Automatic), "1.0\xb5s");
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(-30.0, OpenMesh::Utils::Timer::Automatic), "-30.000s");
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(1e-2, OpenMesh::Utils::Timer::Automatic), "1.000cs");
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(1e-3, OpenMesh::Utils::Timer::Automatic), "1.000ms");
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(1e-4, OpenMesh::Utils::Timer::Automatic), "100.0\xb5s");
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(1e-5, OpenMesh::Utils::Timer::Automatic), "10.0\xb5s");
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(1e-7, OpenMesh::Utils::Timer::Automatic), "100.0ns");
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(1e-8, OpenMesh::Utils::Timer::Automatic), "10.0ns");
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(1e-9, OpenMesh::Utils::Timer::Automatic), "1.0ns");
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(1e-10, OpenMesh::Utils::Timer::Automatic), "100.0ps");
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(1e-11, OpenMesh::Utils::Timer::Automatic), "10.0ps");
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(1e-12, OpenMesh::Utils::Timer::Automatic), "1.0ps");
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(1e6, OpenMesh::Utils::Timer::Automatic), "277h:46m:40s");
}


TEST(Timer, AsString_ZeroAllFormats) {
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(0.0, OpenMesh::Utils::Timer::Automatic), "0.0ps");
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(0.0, OpenMesh::Utils::Timer::Long), "00h:00m:00.000000000000s");
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(0.0, OpenMesh::Utils::Timer::Hours), "00h:00m:00s");
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(0.0, OpenMesh::Utils::Timer::Minutes), "00m:00.00s");
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(0.0, OpenMesh::Utils::Timer::Seconds), "0.000s");
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(0.0, OpenMesh::Utils::Timer::HSeconds), "0.000cs");
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(0.0, OpenMesh::Utils::Timer::MSeconds), "0.000ms");
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(0.0, OpenMesh::Utils::Timer::MicroSeconds), "0.0\xB5s");
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(0.0, OpenMesh::Utils::Timer::NanoSeconds), "0.0ns");
}


TEST(Timer, DISABLED_AsString_EdgeCases) {
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(65.994, OpenMesh::Utils::Timer::Minutes), "01m:05.99s");
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(65.995, OpenMesh::Utils::Timer::Minutes), "01m:05.99s");
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(65.00, OpenMesh::Utils::Timer::Minutes), "01m:05.00s");
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(123.999, OpenMesh::Utils::Timer::Minutes), "02m:03.99s");
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(3661.999999999999, OpenMesh::Utils::Timer::Long), "01h:01m:01.999999999999s");
    EXPECT_EQ(OpenMesh::Utils::Timer::as_string(3661.0, OpenMesh::Utils::Timer::Long), "01h:01m:01.000000000000s");
}
