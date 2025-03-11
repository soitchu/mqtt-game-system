with import <nixpkgs> {};
mkShell {
  buildInputs = [
    paho-mqtt-c
    paho-mqtt-cpp
    gcc
  ];
}