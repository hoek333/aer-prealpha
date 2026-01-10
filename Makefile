glob-sources:
	python scripts/glob_sources.py


linux-gen: glob-sources
	cmake -S . -B build \
		-DCMAKE_CXX_FLAGS="-Wall -Wextra" \
		-DCMAKE_BUILD_TYPE=Debug


linux-build: linux-gen
	cmake --build build -j14


linux: linux-build
	build/yumeami


linux-release: glob-sources
	cmake -S . -B build \
		-DCMAKE_BUILD_TYPE=Release \
		-DCMAKE_CXX_FLAGS="-O2 -Wall -Wextra" \
	&& cmake --build build


win32-gen: glob-sources
	cmake -S . -B build-win32 \
		-DCMAKE_TOOLCHAIN_FILE="/usr/share/mingw/toolchain-ucrt64.cmake" \
		-DCMAKE_CXX_FLAGS="-Wall -Wextra" \
		-DCMAKE_BUILD_TYPE=Debug


win32-build: win32-gen
	cmake --build build-win32 -j14


win32-release: glob-sources
	cmake -S . -B build-win32 \
		-DCMAKE_TOOLCHAIN_FILE="/usr/share/mingw/toolchain-ucrt64.cmake" \
		-DCMAKE_BUILD_TYPE=Release \
		-DCMAKE_CXX_FLAGS="-O2 -Wall -Wextra" \
	&& cmake --build build
