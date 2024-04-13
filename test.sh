if [ $# -eq 0 ]
  then
    echo "Usage: sh test.sh <output tty number>"
    exit 1
fi
tput clear > /dev/ttys$1
echo "2>/dev/ttys"$1 > /dev/ttys$1
./cj test.txt 2>/dev/ttys$1
stty sane
