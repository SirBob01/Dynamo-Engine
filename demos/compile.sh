# Automatically compile all demo programs
SRC_FILES="$(find ../src -name '*.cpp')";

shopt -s globstar
for  i in **/*.cpp;
do
    echo "Compiling $i...";
    g++ -g -O0 $i $SRC_FILES -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_net -lvorbis -lvorbisfile -o "${i%.cpp}";
done