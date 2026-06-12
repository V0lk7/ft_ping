FROM	debian:12.14-slim

RUN apt-get update \
	&& DEBIAN_FRONTEND=noninteractive \
	apt-get install --no-install-recommends --assume-yes \
	wget \
	software-properties-common \
	build-essential

ENV INETUTILS=inetutils-2.0

RUN wget https://ftp.gnu.org/gnu/inetutils/${INETUTILS}.tar.gz \
	&& tar -xf ${INETUTILS}.tar.gz && cd ${INETUTILS} \
	&& ./configure --prefix=/usr --localstatedir=/var \
	&& make && make install && cd .. \
	&& rm -rf ${INETUTILS}.tar.gz && rm -rf ${INETUTILS}

RUN mkdir -p ft_ping

ENV LLVM_SCRIPT=https://apt.llvm.org/llvm.sh
ENV CLANG_VER=22

RUN wget ${LLVM_SCRIPT} && chmod +x llvm.sh && ./llvm.sh ${CLANG_VER}

CMD ["bash"]
