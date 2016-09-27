#!/bin/sh
set -x
PROJECT="libevent-2.1.5-beta"

DIR=$(pwd)
PLATFORM=`uname -s`"-"`uname -m`
INC_TARGET=${DIR}/include/event2
LIB_TARGET=${DIR}/install/${PLATFORM}/lib
SOURCE_DIR=${DIR}/${PROJECT}
INC_SOURCE_DIR=${SOURCE_DIR}/include/event2

#Setup Include Directory
if [ -d ${INC_TARGET} ];
then
  rm ${INC_TARGET}/*
else
  echo "Make include folder directory [${INC_TARGET}]"
  mkdir -p ${INC_TARGET}
fi

#Setup Lib directory
if [ -d ${LIB_TARGET} ];
then
  rm ${LIB_TARGET}/*
else
  echo "Make lib folder directory [${LIB_TARGET}]"
  mkdir -p ${LIB_TARGET}
fi

cp ${INC_SOURCE_DIR}/*.h ${INC_TARGET}

cd ${SOURCE_DIR}
echo "Enter project src dir [$(pwd)]"
./autogen.sh
echo "Configure project"
./configure
echo "Make Project"
make

# Copy Built Libs to Target Install Directory
echo "Build succeess"
find . -name "*.so*" -exec cp {} ${LIB_TARGET} \;

make clean
cd ..
