#!/bin/bash

echo "C Development Environment for Pipes and Signals"
echo "=============================================="

# Function to cleanup FIFOs on exit
cleanup() {
    echo "Cleaning up FIFOs..."
    rm -f /tmp/fifo1 /tmp/fifo2
}

# Set trap to cleanup on exit
trap cleanup EXIT

# Create FIFOs
echo "Setting up FIFOs..."
rm -f /tmp/fifo1 /tmp/fifo2
mkfifo /tmp/fifo1
mkfifo /tmp/fifo2
echo "Created FIFOs: /tmp/fifo1 and /tmp/fifo2"

# Build the projects
echo ""
echo "Building projects..."
make

echo ""
echo "Build complete! Available commands:"
echo "  ./serveur      - Run server (in one terminal)"
echo "  ./client       - Run client (in another terminal)"
echo "  make           - Rebuild both"
echo "  make clean     - Clean build and FIFOs"
echo "  make serveur   - Build only server"
echo "  make client    - Build only client"
echo ""

# Display FIFO info
echo "FIFO locations:"
echo "  /tmp/fifo1 (server read, client write)"
echo "  /tmp/fifo2 (server write, client read)"
ls -la /tmp/fifo1 /tmp/fifo2

echo ""
echo "To test:"
echo "1. Run './serveur' in one terminal"
echo "2. Run './client' in another terminal"
echo ""

# Keep container running
exec bash