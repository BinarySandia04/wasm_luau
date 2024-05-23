# Builds a python interpreter sandbox to be able to execute
# cpp exercises safely. Reads from stdin the python (shelldoctest with in/out tests)
# The input code is expected to compile the cpp program.
#
# Build with: sudo docker build -t cpp .
#
# It requires proper cgroups limits configuration
# Ex: sudo /bin/cat prova.py | docker run -i -m=5m -n=false -c=10 python

FROM emscripten/emsdk:3.1.60
MAINTAINER Aran Roig <aran@aranroig.com>

RUN apt-get -y update
RUN apt-get -y install build-essential cmake 
RUN apt-get -y clean
RUN apt-get -y update --fix-missing

COPY . /src
RUN /src/build.sh
