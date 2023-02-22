FROM ghcr.io/usdot-fhwa-stol/carma-builds:latest

COPY ./include /home/carma-time-lib/include/
COPY ./src /home/carma-time-lib/src/
COPY ./test /home/carma-time-lib/test/
COPY ./build.sh /home/carma-time-lib/build.sh
COPY ./CMakeLists.txt /home/carma-time-lib/CMakeLists.txt
COPY ./Config.cmake.in /home/carma-time-lib/Config.cmake.in
COPY ./Doxyfile /home/carma-time-lib/Doxyfile
COPY ./README.md /home/carma-time-lib/README.md
COPY ./install_dependencies.sh /home/carma-time-lib/install_dependencies.sh
WORKDIR /home/carma-time-lib/

#RUN ./install_dependencies.sh
RUN ./build.sh -d
