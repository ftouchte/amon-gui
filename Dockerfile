# ============================================================
# Dockerfile — Application C++ (ROOT + HIPO4 + GTKmm 4.0)
# Base : Ubuntu 24.04 LTS
# Auteur : généré pour Felix Touchte Codjo
# ============================================================

FROM ubuntu:24.04

# Évite les prompts interactifs pendant apt
ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=Europe/Paris

# ── 1. Outils de base + dépendances système ─────────────────
RUN apt-get update && apt-get install -y \
    build-essential \
    g++ \
    cmake \
    git \
    wget \
    curl \
    pkg-config \
    ca-certificates \
    # LZ4
    liblz4-dev \
    # GTKmm 4.0 + Cairomm 1.16
    libgtkmm-4.0-dev \
    libcairomm-1.16-dev \
    # ROOT dependencies
    libx11-dev \
    libxext-dev \
    libxft-dev \
    libxpm-dev \
    libssl-dev \
    libpcre3-dev \
    libglu1-mesa-dev \
    libglew-dev \
    libftgl-dev \
    libfftw3-dev \
    libcfitsio-dev \
    libgraphviz-dev \
    python3 \
    python3-pip \
    # X11 pour le GUI dans Docker
    libx11-xcb1 \
    x11-apps \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

# ── 2. Installation de ROOT (CERN) via binaire pré-compilé ──
# ROOT 6.32 — Ubuntu 24.04 — x86_64
ENV ROOT_VERSION=6.32.02
ENV ROOT_DIR=/opt/root

RUN wget -q https://root.cern/download/root_v${ROOT_VERSION}.Linux-ubuntu24.04-x86_64-gcc13.3.tar.gz \
    -O /tmp/root.tar.gz && \
    mkdir -p ${ROOT_DIR} && \
    tar -xzf /tmp/root.tar.gz -C ${ROOT_DIR} --strip-components=1 && \
    rm /tmp/root.tar.gz

# Ajoute ROOT au PATH et aux variables d'environnement
ENV ROOTSYS=${ROOT_DIR}
ENV PATH="${ROOT_DIR}/bin:${PATH}"
ENV LD_LIBRARY_PATH="${ROOT_DIR}/lib:${LD_LIBRARY_PATH}"
ENV PYTHONPATH="${ROOT_DIR}/lib:${PYTHONPATH}"

# ── 3. HIPO4 — à ajouter plus tard ─────────────────────────
# (installation à venir)

# ── 4. Répertoire de travail ─────────────────────────────────
WORKDIR /app

# Copie tout le code source dans le container
COPY . /app/

# ── 5. Compilation de l'application ──────────────────────────
# ⚠️  Désactivé pour l'instant — HIPO4 pas encore installé
# RUN bash -c "source ${ROOT_DIR}/bin/thisroot.sh && make all"

# ── 6. Variables d'environnement pour le GUI (X11) ──────────
# DISPLAY doit être passé au runtime : -e DISPLAY=$DISPLAY
ENV DISPLAY=:0

# ── 7. Point d'entrée ────────────────────────────────────────
# Lance l'application GUI directement
# Tu peux surcharger avec : docker run ... /bin/bash
CMD ["bash"]
