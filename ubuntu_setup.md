## install -dbgsym for systemtap
1. echo "deb http://ddebs.ubuntu.com $(lsb_release -cs) main restricted universe multiverse" | sudo tee /etc/apt/sources.list.d/ddebs.list
2. wget -O - http://ddebs.ubuntu.com/dbgsym-release-key.asc | sudo apt-key add -
3. sudo apt update
4. sudo apt-get install linux-image-$(uname -r)-dbgsym
