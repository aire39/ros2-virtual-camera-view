#!/bin/bash

source /opt/ros/humble/setup.bash
source /usr/share/gazebo-11/setup.bash
source /home/sean/scout_ws/install/setup.bash

export MESA_GL_VERSION_OVERRIDE=4.6
export MESA_GLSL_VERSION_OVERRIDE=460

mkdir build_release
cd build_release
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .

cd ..
