$ROOT_DIR=$args[0]
$PROJECT_DIR=$args[1]

$SOURCEPAWN_DEPS_DIR="$PROJECT_DIR\sourcepawn_deps"

new-item $SOURCEPAWN_DEPS_DIR -itemtype directory -Force -ErrorAction SilentlyContinue

# Install ambuild
git clone -q --depth=1 https://github.com/alliedmodders/ambuild.git $SOURCEPAWN_DEPS_DIR\ambuild

Set-Location -Path $SOURCEPAWN_DEPS_DIR\ambuild
python setup.py install

# Get submodules for SourcePawn lib
Set-Location -Path $PROJECT_DIR\sourcepawn
git submodule update

# Build SourcePawn lib
new-item $SOURCEPAWN_DEPS_DIR\sourcepawn_build -itemtype directory -Force -ErrorAction SilentlyContinue
Set-Location -Path $SOURCEPAWN_DEPS_DIR\sourcepawn_build
python $PROJECT_DIR\sourcepawn\configure.py --enable-optimize --build=core
ambuild

