FROM ubuntu:latest

RUN apt-get update
RUN apt-get install -q -y build-essential
RUN apt-get install -q -y bpfcc-tools
RUN apt-get install -q -y libbpfcc
RUN apt-get install -q -y libbpfcc-dev
RUN apt-get install -q -y linux-headers-$(uname -r)
WORKDIR /work
COPY . .
RUN make
