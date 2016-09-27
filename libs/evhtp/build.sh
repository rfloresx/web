#!/bin/sh

PROJECT="libevhtp"

DIR=$(pwd)
PLATFORM=`uname -s`"-"`uname -m`
INC_TARGET=${DIR}/include
LIB_TARGET=${DIR}/install/${PLATFORM}/lib
SOURCE_DIR=${DIR}/${PROJECT}
INC_SOURCE_DIR=${SOURCE_DIR}

#Setup Include Directory
if [ -d ${INC_TARGET} ];
then
  rm -r ${INC_TARGET}/*
else
  echo "Make include folder directory [${INC_TARGET}]"
  mkdir -p ${INC_TARGET}
fi

#Setup Lib directory
if [ -d ${LIB_TARGET} ];
then
  rm -f ${LIB_TARGET}/*
else
  echo "Make lib folder directory [${LIB_TARGET}]"
  mkdir -p ${LIB_TARGET}
fi


cd ${SOURCE_DIR}
echo "Enter project src dir [$(pwd)]"

#init and update submodule
echo "Update project src [$(pwd)]"
git submodule init
git submodule update

echo "Configure project"

echo "Run CMake"
cmake -DEVHTP_BUILD_SHARED:STRING=ON .

#copy headers
cp -r ${INC_SOURCE_DIR}/*.h ${INC_TARGET}
cp -r ${INC_SOURCE_DIR}/oniguruma/*.h ${INC_TARGET}

echo "Make Project"
make

# Copy Built Libs to Target Install Directory
echo "Build succeess"
find . -name "*.so*" -exec cp {} ${LIB_TARGET} \;

make clean
cd ..
