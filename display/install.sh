#!/bin/bash 

pip install -r requirements.txt

if [[ -d "$HOME/.fonts" ]] 
then
    cp ./fonts/* $HOME/.fonts
elif [[ -d "$HOME/.local/share/fonts" ]]
then
    cp ./fonts/* $HOME/.local/share/fonts
else
    mkdir $HOME/.local/share/fonts
    cp ./fonts/* $HOME/.local/share/fonts
fi

fc-cache
