all:
	cmake --build Build --config Debug -j

init:
	cmake -B Build -D CMAKE_BUILD_TYPE=Debug