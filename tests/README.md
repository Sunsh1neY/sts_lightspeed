# Combat regression tests

These tests exercise the C++ game and combat APIs directly, without Python bindings.
They use explicit checks that remain active in release builds.

```sh
git submodule update --init json
cmake -S tests -B cmake-build-regressions -DCMAKE_BUILD_TYPE=Debug
cmake --build cmake-build-regressions --parallel 2
ctest --test-dir cmake-build-regressions --output-on-failure
```

On GCC versions that no longer transitively include `<algorithm>`, configure with
`-DCMAKE_CXX_FLAGS="-include algorithm -Wno-shift-count-overflow"`. This works around
existing upstream include dependencies and suppresses the same shift warnings as
the root build; it does not change the combat logic under test.
