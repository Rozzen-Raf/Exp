git submodule init
git submodule update
cd external
cd Catch2
cmake -Bbuild -H. -DBUILD_TESTING=OFF
sudo cmake --build build/ --target install
cd ../../
mkdir build
cd build
cmake ..
cmake --build .
