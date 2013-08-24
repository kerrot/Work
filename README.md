# test

A simple Irrlicht application with Oculus Rift support.

## To build

    cd Build/
    cmake ../

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

    make
    make install
    cd ../

## To run

### Windows

    run.bat

### Linux

    ./Binary/test

## License
