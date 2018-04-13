// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonVariant::operator[]") {
  SECTION("Array") {
    DynamicJsonDocument doc;
    JsonArray& array = doc.to<JsonArray>();
    array.add("element at index 0");
    array.add("element at index 1");

    JsonVariant var = array;

    REQUIRE(2 == var.size());
    REQUIRE(std::string("element at index 0") == var[0]);
    REQUIRE(std::string("element at index 1") == var[1]);
    REQUIRE(std::string("element at index 0") ==
            var[static_cast<unsigned char>(0)]);  // issue #381
    REQUIRE_FALSE(var[666].success());
    REQUIRE_FALSE(var[3].success());
    REQUIRE_FALSE(var["0"].success());
  }

  SECTION("Object") {
    DynamicJsonDocument doc;
    JsonObject& object = doc.becomeObject();
    object["a"] = "element at key \"a\"";
    object["b"] = "element at key \"b\"";

    JsonVariant var = object;

    REQUIRE(2 == var.size());
    REQUIRE(std::string("element at key \"a\"") == var["a"]);
    REQUIRE(std::string("element at key \"b\"") == var["b"]);
    REQUIRE_FALSE(var["c"].success());
    REQUIRE_FALSE(var[0].success());
  }

  SECTION("Undefined") {
    JsonVariant var = JsonVariant();
    REQUIRE(0 == var.size());
    REQUIRE_FALSE(var["0"].success());
    REQUIRE_FALSE(var[0].success());
  }

  SECTION("String") {
    JsonVariant var = "hello world";
    REQUIRE(0 == var.size());
    REQUIRE_FALSE(var["0"].success());
    REQUIRE_FALSE(var[0].success());
  }

  SECTION("ObjectSetValue") {
    DynamicJsonDocument doc;
    JsonObject& obj = doc.becomeObject();
    JsonVariant var = obj;
    var["hello"] = "world";
    REQUIRE(1 == var.size());
    REQUIRE(std::string("world") == var["hello"]);
  }

  SECTION("ArraySetValue") {
    DynamicJsonDocument var;
    deserializeJson(var, "[\"hello\"]");
    var[0] = "world";
    REQUIRE(1 == var.size());
    REQUIRE(std::string("world") == var[0]);
  }

  SECTION("NestedObjectSetValue") {
    DynamicJsonDocument var;
    deserializeJson(var, "[{}]");
    var[0]["hello"] = "world";
    REQUIRE(1 == var.size());
    REQUIRE(1 == var[0].size());
    REQUIRE(std::string("world") == var[0]["hello"]);
  }
}
