# set up paths, the paths move around on repl
cd src
cdate=$(date +%Y%m%dT%H%M%s)
backuptargetname=$target_file$cdate
backupsourcename=$source_file$cdate

# name your files here - this is your own thing to edit/complile in C++ 
source_file=$PWD/'main.cpp'
cd ../bin
target_file=$PWD/'compiled'
cd ../src
# check if freeglut3-dev is installed on host
# install it if one of the library files isnt missing
# see output of >ldd triangle< from shell for more info
if [ ! -f ~/.apt/usr/lib/x86_64-linux-gnu/libGLEW.so ]
then
  # if using replit
  # install-pkg libglew-dev libglfw3-dev libglm-dev

  # if using ubuntu
  sudo apt install libglew-dev libglfw3-dev libglm-dev
fi

# performs versioning, checks mtime on file to see
# if cpp src file is modified newer than compiled binary
#if so then move it to backup dir and remake with g++
if [ "$source_file" -nt "$target_file" ]
then
    g++ $source_file  -lGL -lGLEW -lglfw -o $target_file
    # chmod u+x .$target_file
fi

if [ $? -eq 0 ]; then
   chmod u+x $target_file
   $target_file
else
   echo "Compile failed"
fi



# if we made it this far, start your program
# $target_file