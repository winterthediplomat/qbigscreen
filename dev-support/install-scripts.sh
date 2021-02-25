destpath="$HOME/.config/qbigscreen/scripts"
mkdir --parents $destpath
date > $destpath/.install
cat $destpath/.install
cp ../scripts/*.sh $destpath
cp ../scripts/*.py $destpath
chmod +x $destpath/*.sh
chmod +x $destpath/*.py

layoutpath="$HOME/.config/qbigscreen/layouts"
mkdir --parents $layoutpath
cp ../layouts/*.gamecontroller.amgp $layoutpath
