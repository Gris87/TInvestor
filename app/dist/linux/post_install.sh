#!/bin/bash



CURRENT_DIR=`pwd`
DESKTOP_FILE=$HOME/.local/share/applications/TInvestor.desktop



mkdir -p $HOME/.icons
cp icon.png $HOME/.icons/TInvestor.png



mkdir -p $HOME/.local/share/applications
echo "[Desktop Entry]"                                     >  ${DESKTOP_FILE}
echo "Type=Application"                                    >> ${DESKTOP_FILE}
echo "Version=1.0"                                         >> ${DESKTOP_FILE}
echo "Name=TInvestor"                                      >> ${DESKTOP_FILE}
echo "GenericName=T-Bank Investment trading robot"         >> ${DESKTOP_FILE}
echo "Path=${CURRENT_DIR}"                                 >> ${DESKTOP_FILE}
echo "Exec=env LD_LIBRARY_PATH=${CURRENT_DIR} ./TInvestor" >> ${DESKTOP_FILE}
echo "Icon=TInvestor"                                      >> ${DESKTOP_FILE}
echo "Terminal=false"                                      >> ${DESKTOP_FILE}
echo "Categories=Investment;"                              >> ${DESKTOP_FILE}

rm -f $HOME/Desktop/TInvestor.desktop
cp ${DESKTOP_FILE} $HOME/Desktop/TInvestor.desktop

chmod 755 ${DESKTOP_FILE}
chmod 755 $HOME/Desktop/TInvestor.desktop
