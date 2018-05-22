# Maintainer: Quytelda Kahja <quytelda@tamalin.org>
pkgname=focs
pkgver='0.0.0.rc1'
_focsver='0.0.0-rc1'
pkgrel=1
pkgdesc="A C data structures library with a functional bent."
arch=('i686', 'x86_64')
url="https://quytelda.github.io/focs"
license=('GPL')
groups=()
depends=()
makedepends=()
checkdepends=()
optdepends=()
provides=()
conflicts=()
replaces=()
backup=()
options=()
install=
changelog=
source=("$pkgname-$pkgver.tar.gz"
        "$pkgname-$pkgver.patch")
noextract=()
md5sums=()
validpgpkeys=()

prepare() {
	cd "$pkgname-$pkgver"
	patch -p1 -i "$srcdir/$pkgname-$pkgver.patch"
}

build() {
	cd "$pkgname-$pkgver"
	./configure --prefix=/usr
	make
}

check() {
	cd "$pkgname-$pkgver"
	make -k check
}

package() {
	cd "$pkgname-$pkgver"
	make DESTDIR="$pkgdir/" install
}
