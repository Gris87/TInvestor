if [ -z "$1" ]; then
    echo "Usage: ./commit.sh MESSAGE"

    exit 0
fi

git add .
git commit -a -m "$1"
./push.sh
