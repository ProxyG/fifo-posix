FROM ubuntu:22.04

# Avoid interactive prompts
ENV DEBIAN_FRONTEND=noninteractive

# Install essential packages
RUN apt-get update && apt-get install -y \
    build-essential \
    gcc \
    gdb \
    make \
    valgrind \
    vim \
    libgtk-3-dev \
    && rm -rf /var/lib/apt/lists/*
# Set working directory
WORKDIR /workspace

# Copy source code
COPY . .

# Make entrypoint script executable
RUN chmod +x /workspace/scripts/entrypoint.sh

# Default command
CMD ["/workspace/scripts/entrypoint.sh"]