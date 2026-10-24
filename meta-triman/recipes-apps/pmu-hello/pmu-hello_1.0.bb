SUMMARY = "Counts CPU instructions for a loop with perf_event_open"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://pmu-hello.cpp"
S = "${WORKDIR}"

do_compile() {
    ${CXX} ${CXXFLAGS} ${LDFLAGS} ${S}/pmu-hello.cpp -o ${B}/pmu-hello
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 ${B}/pmu-hello ${D}${bindir}/pmu-hello
}
