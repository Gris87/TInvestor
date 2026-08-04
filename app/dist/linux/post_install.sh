#!/bin/bash



CURRENT_DIR=`pwd`
TINVESTOR_DESKTOP_FILE=$HOME/.local/share/applications/TInvestor.desktop
NOTIFIER_DESKTOP_FILE=$HOME/.local/share/applications/TInvestorNotifier.desktop



mkdir -p $HOME/.icons
cp icon.png $HOME/.icons/TInvestor.png
cp notifier_icon.png $HOME/.icons/TInvestorNotifier.png



mkdir -p $HOME/.local/share/applications
echo "[Desktop Entry]"                                     >  ${TINVESTOR_DESKTOP_FILE}
echo "Type=Application"                                    >> ${TINVESTOR_DESKTOP_FILE}
echo "Version=1.0"                                         >> ${TINVESTOR_DESKTOP_FILE}
echo "Name=TInvestor"                                      >> ${TINVESTOR_DESKTOP_FILE}
echo "GenericName=T-Bank Investment trading robot"         >> ${TINVESTOR_DESKTOP_FILE}
echo "Comment=T-Bank Investment trading robot"             >> ${TINVESTOR_DESKTOP_FILE}
echo "Path=${CURRENT_DIR}"                                 >> ${TINVESTOR_DESKTOP_FILE}
echo "Exec=env LD_LIBRARY_PATH=${CURRENT_DIR} ./TInvestor" >> ${TINVESTOR_DESKTOP_FILE}
echo "Icon=TInvestor"                                      >> ${TINVESTOR_DESKTOP_FILE}
echo "Terminal=false"                                      >> ${TINVESTOR_DESKTOP_FILE}
echo "Categories=Investment;"                              >> ${TINVESTOR_DESKTOP_FILE}

echo "[Desktop Entry]"                                    >  ${NOTIFIER_DESKTOP_FILE}
echo "Type=Application"                                   >> ${NOTIFIER_DESKTOP_FILE}
echo "Version=1.0"                                        >> ${NOTIFIER_DESKTOP_FILE}
echo "Name=TInvestorNotifier"                             >> ${NOTIFIER_DESKTOP_FILE}
echo "GenericName=Notifier for TInvestor"                 >> ${NOTIFIER_DESKTOP_FILE}
echo "Comment=Notifier for TInvestor"                     >> ${NOTIFIER_DESKTOP_FILE}
echo "Path=${CURRENT_DIR}"                                >> ${NOTIFIER_DESKTOP_FILE}
echo "Exec=env LD_LIBRARY_PATH=${CURRENT_DIR} ./Notifier" >> ${NOTIFIER_DESKTOP_FILE}
echo "Icon=TInvestorNotifier"                             >> ${NOTIFIER_DESKTOP_FILE}
echo "Terminal=false"                                     >> ${NOTIFIER_DESKTOP_FILE}
echo "Categories=Investment;"                             >> ${NOTIFIER_DESKTOP_FILE}

cp ${TINVESTOR_DESKTOP_FILE} $HOME/Desktop/TInvestor.desktop
cp ${NOTIFIER_DESKTOP_FILE} $HOME/Desktop/TInvestorNotifier.desktop

chmod 755 ${TINVESTOR_DESKTOP_FILE}
chmod 755 ${NOTIFIER_DESKTOP_FILE}
chmod 755 $HOME/Desktop/TInvestor.desktop
chmod 755 $HOME/Desktop/TInvestorNotifier.desktop

sudo cp russian_trusted_root_ca_pem.crt /usr/local/share/ca-certificates/
sudo cp russian_trusted_sub_ca_pem.crt /usr/local/share/ca-certificates/
sudo update-ca-certificates
