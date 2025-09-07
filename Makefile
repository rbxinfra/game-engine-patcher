
build-rcc:
	@echo "Building Windows x86 Release RCC"

ifdef STANDALONE
	xmake f -p windows -a x86 -m release-rcc -vy --standalone=y
else
	xmake f -p windows -a x86 -m release-rcc -vy
endif

	xmake -vy

build-client:
	@echo "Building Windows x86 Release Client"

ifdef STANDALONE
	xmake f -p windows -a x86 -m release-client -vy --standalone=y
else
	xmake f -p windows -a x86 -m release-client -vy
endif

	xmake -vy

build: build-rcc build-client
