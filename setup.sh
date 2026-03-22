#!/bin/bash

echo "Checking for gcc..."

if ! command -v gcc &> /dev/null
then
    echo "gcc not found. Installing..."
    sudo apt update
    sudo apt install -y build-essential
else
    echo "gcc already installed ✅"
fi

echo "Checking for ncurses..."

if ! dpkg -s libncurses5-dev &> /dev/null
then
    echo "ncurses not found. Installing..."
    sudo apt install -y libncurses5-dev libncursesw5-dev
else
    echo "ncurses already installed ✅"
fi

echo "Setup complete!"
