// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

using namespace Catch::Matchers;

TEST_CASE("deserializeJson(StaticJsonDocument&)") {
  SECTION("Array") {
    StaticJsonDocument<JSON_ARRAY_SIZE(2)> doc;

    char input[] = "[1,2]";
    JsonError err = deserializeJson(doc, input);

    REQUIRE(err == JsonError::Ok);
    REQUIRE(doc.is<JsonArray>());
    REQUIRE(doc[0] == 1);
    REQUIRE(doc[1] == 2);
    REQUIRE(doc.memoryUsage() == JSON_ARRAY_SIZE(2));
  }

  SECTION("Should clear the JsonDocument") {
    StaticJsonDocument<JSON_ARRAY_SIZE(2)> doc;
    char input[] = "[1,2]";
    deserializeJson(doc, input);

    JsonError err = deserializeJson(doc, "{}");

    REQUIRE(err == JsonError::Ok);
    REQUIRE(doc.is<JsonObject>());
    REQUIRE(doc.memoryUsage() == JSON_OBJECT_SIZE(0));
  }
}
