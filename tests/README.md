# Combat regression tests

These tests use the C++ combat APIs directly, without Python bindings.

```sh
git submodule update --init json
cmake -S tests -B cmake-build-regressions -DCMAKE_BUILD_TYPE=Debug
cmake --build cmake-build-regressions --parallel 2
ctest --test-dir cmake-build-regressions --output-on-failure
```

On GCC versions that do not transitively include `<algorithm>`, configure with
`-DCMAKE_CXX_FLAGS="-include algorithm -Wno-shift-count-overflow"`. This works
around existing upstream include dependencies and does not change the mechanism.
