# test

A simple Irrlicht application with Oculus Rift support.

## To build

### Windows

    open cmake-gui
    select root dir (where the CMakeList.txt located) as source code dir
    select Build\ as build dir
    click 'Configure'
    click 'Generate'
    open Build\test.sln with MCVS
    Build ALL_BUILD
    Build INSTALL

### Linux

    cd Build/
    cmake ../

    make
    make install
    cd ../

## To run

### Windows

    double click ./Binary/test.exe

### Linux

    ./Binary/test

## License
