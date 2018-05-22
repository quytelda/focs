# Maintainer: Quytelda Kahja <quytelda@tamalin.org>
pkgname=focs
pkgver='0.0.0.rc1'
_focsver='0.0.0-rc1'
pkgrel=1
pkgdesc="A C data structures library with a functional bent."
arch=('i686', 'x86_64')
url="https://quytelda.github.io/focs"
license=('GPL3')
depends=('gcc' 'glibc')
makedepends=('make' 'gcc')
checkdepends=('check')
provides=('libfocs.so')
source=('https://github.com/quytelda/focs/releases/download/v0.0.0-rc1/focs-0.0.0-rc1.tar.xz')
md5sums=('63214cf48dd0ea86227839e2f58d855a')

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
