# Builds a python interpreter sandbox to be able to execute
# cpp exercises safely. Reads from stdin the python (shelldoctest with in/out tests)
# The input code is expected to compile the cpp program.
#
# Build with: sudo docker build -t lua .
#
# It requires proper cgroups limits configuration
# Ex: sudo /bin/cat prova.py | docker run -i -m=5m -n=false -c=10 python

FROM emscripten/emsdk:3.1.60
MAINTAINER Aran Roig <aran@aranroig.com>

USER root

RUN apt-get -y update
RUN apt-get -y install build-essential cmake 
RUN apt-get -y clean
RUN apt-get -y update --fix-missing
RUN apt-get -y install python2 python2.7-dev libffi-dev libssl-dev curl
RUN curl https://bootstrap.pypa.io/pip/2.7/get-pip.py --output get-pip.py
RUN ln -s /usr/bin/python2 /usr/bin/python
RUN python get-pip.py
RUN pip install shelldoctest

COPY . /src
RUN /src/build.sh

ENTRYPOINT ["timeout", "15", "python", "-"]

