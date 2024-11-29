# Sample Json
A simple json parser for parsing json files.  
The parser for this is a recursive descent parser. It uses only one token to determine the next step.  
This is my attempt for the [challenge](https://codingchallenges.fyi/challenges/challenge-json-parser)

## Usage
To use the the parser use the header file ./include/json.hpp

## Build and run tests
To build the project and run tests run the following commands at the root of the repository.
```
mkdir build
cd build
cmake ..
make
./test_json
```
