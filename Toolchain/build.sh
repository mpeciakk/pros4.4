# Prepare variables
export PROS_HOME=$HOME/Projects/pros4.4
export PREFIX="$PROS_HOME/Toolchain/i686"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

MAKE_JOBS="4"

DIR="$PROS_HOME/Toolchain"

BINUTILS_VERSION="2.37"
BINUTILS_NAME="binutils-$BINUTILS_VERSION"
BINUTILS_PKG="${BINUTILS_NAME}.tar.gz"
BINUTILS_BASE_URL="http://ftp.gnu.org/gnu/binutils"

GCC_VERSION="11.2.0"
GCC_NAME="gcc-$GCC_VERSION"
GCC_PKG="${GCC_NAME}.tar.gz"
GCC_BASE_URL="http://ftp.gnu.org/gnu/gcc"

YASM_VERSION="1.3.0"
YASM_NAME="yasm-$YASM_VERSION"
YASM_PKG="$YASM_NAME.tar.gz"
YASM_BASE_URL="https://www.tortall.net/projects/yasm/releases"

mkdir -p "$DIR/Tarballs"
mkdir -p "$PREFIX"
mkdir -p "$DIR/Build/binutils"
mkdir -p "$DIR/Build/gcc"
mkdir -p "$DIR/Build/yasm"

# binutils
cd "$DIR/Tarballs"
echo "Downloading binutils..."
if [ ! -e $BINUTILS_PKG ]; then
  curl -LO "$BINUTILS_BASE_URL/$BINUTILS_PKG"
else
  echo "Skipped downloading binutils"
fi

if [ -d ${BINUTILS_NAME} ]; then
  rm -rf "${BINUTILS_NAME}"
  rm -rf "$DIR/Build/$BINUTILS_NAME"
fi

echo "Extracting binutils..."
tar -xzf ${BINUTILS_PKG}

cd "$DIR/Build/binutils"
"$DIR"/Tarballs/$BINUTILS_NAME/configure \
  --target=$TARGET --prefix="$PREFIX" \
  --with-sysroot \
  --disable-nls \
  --disable-werror
make -j "$MAKE_JOBS"
make install

# gcc
cd "$DIR/Tarballs"
echo "Downloading gcc..."
if [ ! -e $GCC_PKG ]; then
  curl -LO "$GCC_BASE_URL/$GCC_NAME/$GCC_PKG"
else
  echo "Skipped downloading gcc"
fi

if [ -d ${GCC_NAME} ]; then
  rm -rf "${GCC_NAME}"
  rm -rf "$DIR/Build/$GCC_NAME"
fi

echo "Extracting gcc..."
tar -xzf $GCC_PKG

cd "$DIR/Build/gcc"
"$DIR"/Tarballs/$GCC_NAME/configure \
  --target=$TARGET \
  --prefix="$PREFIX" \
  --disable-nls \
  --enable-languages=c,c++ \
  --without-headers
make all-gcc -j "$MAKE_JOBS"
make all-target-libgcc -j "$MAKE_JOBS"
make install-gcc
make install-target-libgcc

# yasm
cd "$DIR/Tarballs"
echo "Downloading yasm..."
if [ ! -e $YASM_PKG ]; then
  curl -LO "$YASM_BASE_URL/$YASM_PKG"
else
  echo "Skipped downloading yasm"
fi

if [ -d ${YASM_NAME} ]; then
  rm -rf "${YASM_NAME}"
  rm -rf "$DIR/Build/$YASM_NAME"
fi

echo "Extracting yasm..."
tar -xzf $YASM_PKG

cd "$DIR/Build/yasm"
"$DIR"/Tarballs/$YASM_NAME/configure \
  --target=$TARGET --prefix="$PREFIX"
make -j "$MAKE_JOBS"
make install
