Debian

digitalcoin-qt.protocol (KDE)
This directory contains files used to package digitalcoind/digitalcoin-qt
for Debian-based Linux systems. If you compile digitalcoind/digitalcoin-qt yourself, there are some useful files here.

## digitalcoin: URI support ##


digitalcoin-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install digitalcoin-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your digitalcoin-qt binary to `/usr/bin`
and the `../../share/pixmaps/digitalcoin128.png` to `/usr/share/pixmaps`

digitalcoin-qt.protocol (KDE)

