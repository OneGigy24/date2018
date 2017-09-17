FROM mariobarbareschi/clang_llvm391

MAINTAINER Gigy One <one.gigy24@gmail.com>

RUN  pacman --noconfirm -Sy git zsh libedit libffi wget libtar doxygen boost-libs
#pacman --noconfirm -Syu &&


# Copy install script
ADD . /opt

SHELL ["/bin/bash", "-c"]

RUN /opt/install-toolnumberone
RUN /opt/install-paradiseo
RUN /opt/install-toolnumbertwo


