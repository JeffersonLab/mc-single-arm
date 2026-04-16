FROM rootproject/root:6.30.02-alma9

LABEL org.opencontainers.image.title="mc-single-arm"
LABEL org.opencontainers.image.description="Containerized Hall C mc-single-arm with browser configuration UI and JSROOT viewer"
LABEL org.opencontainers.image.source="https://github.com/JeffersonLab/mc-single-arm"

RUN dnf install -y gcc-gfortran gcc-c++ make python3 pcre pcre-devel \
    && dnf clean all

WORKDIR /opt/mc-single-arm
COPY . /opt/mc-single-arm

RUN chmod +x /opt/mc-single-arm/mc-single-arm /opt/mc-single-arm/run_mc_single_arm /opt/mc-single-arm/run_mc_single_arm_tree /opt/mc-single-arm/run_mc_single_arm_ntup \
    && mkdir -p /opt/mc-single-arm/share /data/infiles /data/outfiles /data/runout /data/worksim \
    && cp -r /opt/mc-single-arm/infiles /opt/mc-single-arm/share/infiles \
    && rm -rf /opt/mc-single-arm/infiles /opt/mc-single-arm/outfiles /opt/mc-single-arm/runout /opt/mc-single-arm/worksim \
    && ln -s /data/infiles /opt/mc-single-arm/infiles \
    && ln -s /data/outfiles /opt/mc-single-arm/outfiles \
    && ln -s /data/runout /opt/mc-single-arm/runout \
    && ln -s /data/worksim /opt/mc-single-arm/worksim \
    && rm -rf /opt/mc-single-arm/static/jsroot \
    && mkdir -p /opt/mc-single-arm/static/jsroot \
    && ROOT_PREFIX="$(root-config --prefix)" \
    && JSROOT_SRC="$ROOT_PREFIX/js" \
    && if [ ! -f "$JSROOT_SRC/modules/gui.mjs" ]; then JSROOT_SRC="$ROOT_PREFIX/share/root/js"; fi \
    && test -f "$JSROOT_SRC/modules/gui.mjs" \
    && cp -r "$JSROOT_SRC/." /opt/mc-single-arm/static/jsroot/ \
    && test -f /opt/mc-single-arm/static/jsroot/modules/gui.mjs \
    && make -C /opt/mc-single-arm/src clean all \
    && make -C /opt/mc-single-arm/util/root_tree clean all ROOTSYS="$ROOT_PREFIX" \
    && ln -s /opt/mc-single-arm/mc-single-arm /usr/local/bin/mc-single-arm

ENV MC_SINGLE_ARM_DATA_ROOT=/data

EXPOSE 8080

CMD ["mc-single-arm", "serve"]
