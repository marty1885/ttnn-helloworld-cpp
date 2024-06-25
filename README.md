# ttnn-helloworld-cpp

Well annotated, minimal example that demonstrates how to use TTNN (Tenstorrent's neural netwoek library) in C++.

**NOTE:** TTNN is still a moving target. The code in this repo may break anytime.

## How to use

* Build [tt-metal](https://github.com/tenstorrent/tt-metal/) locally
* Make sure you have the correct variables set up (Eg: `TT_METAL_HOME`, `ARCH_NAME`)

```
mkdir build
cmake ..
make
```