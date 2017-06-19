#pragma once

// addresses used for public data access
// #define switch_checksum              CHECKSUM("switch")
// #define fan_checksum                 CHECKSUM("fan")
// #define state_checksum               CHECKSUM("state")
// #define value_checksum               CHECKSUM("value")

struct pad_switch {
    int name;
    bool state;
    float value;
};
